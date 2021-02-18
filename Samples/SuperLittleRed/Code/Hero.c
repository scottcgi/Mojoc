/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2015-9-17
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Application/Input.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Extension/Font.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Application/Application.h"
#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Application/Platform/Vibrator.h"

#include "Hero.h"
#include "GameMap.h"
#include "Config.h"
#include "Tool.h"
#include "HUD.h"
#include "EnemyAI.h"
#include "UI.h"
#include "GameData.h"
#include "AudioTool.h"
#include "Enemy.h"
#include "GameActor.h"


static DrawAtlas*          arrowPathDrawAtlas;
static const int           arrowPathCount       = 10;
static bool                isArrowPathShow      = false;
static SubMesh*            arrowPath[arrowPathCount];
static PhysicsBody         arrowPathBody[1];

static const float         arrowTailOpacity     =  0.6f;
static const float         arrowTailOpacityTime = -0.06f;

static ArrayIntSet(Arrow*) arrowSet [1] = AArrayIntSet_Init(Arrow*, 25);
static ArrayList  (Arrow*) arrowList[1] = AArrayList_Init  (Arrow*, 25);

#ifdef APP_DEBUG
static Drawable            debugCollisionBoundingBox[1];
#endif


static void Update(Component* component, float deltaSeconds)
{
    ASkeletonAnimationPlayer_Update(AHero->player,   deltaSeconds);
    ASkeletonAnimationPlayer_Update(AHero->hitFloor, deltaSeconds);
    ADrawAtlas_Draw(arrowPathDrawAtlas);

    if (AHero->component->curState->id < HeroState_Die)
    {
        AHero->roundTime += deltaSeconds;
    }

    if (AHero->collisionBoxBody != NULL)
    {
        if (AHero->collisionBoxBody->state != PhysicsBodyState_Freeze)
        {
            ATool->UpdateBox(AHero->collisionBoxBody, AHero->collisionBoxDrawable);
        }

        #ifdef APP_DEBUG
        ADrawable->Draw(debugCollisionBoundingBox);
        #endif
    }

    for (int i = 0; i < arrowSet->elementList->size; ++i)
    {
        Arrow*       arrow    = AArrayList_Get(arrowSet->elementList, i, Arrow*);
        PhysicsBody* body     = arrow->body;
        Drawable*    drawable = arrow->sprite->drawable;

        if (body->state != PhysicsBodyState_Freeze)
        {
            ADrawable_SetRotationZ(drawable, ADrawable->GetFlipRotationZ(drawable, body->rotationZ));
            ADrawable_SetPosition2(drawable, body->positionX, body->positionY);
            ATool->UpdateArrowTail(arrow, deltaSeconds);
        }

        ADrawable->Draw(drawable);
        ADrawable->Draw(arrow->tailDrawable);
    }
}


static void ResetArrow(Arrow* arrow)
{
    ADrawable_SetInvisible(arrow->sprite->drawable);

    ASprite->DeformByIndex
    (
        arrow->sprite,

        AArray_Make
        (
            float,
            2,
            arrow->deformX, arrow->deformX
        ),

        AArray_Make
        (
            float,
            2,
            arrow->deformU, arrow->deformU
        ),

        AArray_Make
        (
            int,
            2,
            4, 6
        )
    );

    // reset body to born state
    APhysicsBody->ResetVertices(arrow->body);

    AArrayList_Add(arrowList, arrow);
}


static void ArrowOnComplete(Arrow* arrow)
{
    if (arrow->hitType == ArrowHitType_Ground)
    {
        AArrayIntSet->TryRemove(arrowSet, (intptr_t) arrow);
    }
    else
    {
        AArrayIntSet->TryRemove(arrow->enemy->arrowSet, (intptr_t) arrow);
    }

    ResetArrow(arrow);
}


static void ArrowActionOnComplete(TweenAction* tweenAction)
{
    ArrowOnComplete(AUserData_GetSlotPtrWithType(tweenAction->userData, 0, Arrow*));
}


static void FreezeOnEnemy(Scheduler* scheduler, float deltaSeconds)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(scheduler->userData, 0, Arrow*);

    AArrayIntSet->TryRemove(arrowSet,               (intptr_t) arrow);
    AArrayIntSet->TryAdd   (arrow->enemy->arrowSet, (intptr_t) arrow);

    float fadeTime;

    if (arrow->enemy->hp < 0)
    {
        fadeTime = 1.0f;
    }
    else
    {
        fadeTime = 3.0f;
    }

    ATweenTool->AddFadeTo     (0.0f, fadeTime)
              ->SetRelative   (false)
              ->SetUserDataPtr(0, arrow)
              ->SetOnComplete (ArrowActionOnComplete)
              ->RunActions    (arrow->sprite->drawable);

    ADrawable->ConvertToParent
    (
        arrow->sprite->drawable,
        arrow->enemy->collisionBoxDrawable
    );

    arrow->body->state = PhysicsBodyState_Freeze;
}


static void FreezeOnOther(Scheduler* scheduler, float deltaSeconds)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(scheduler->userData, 0, Arrow*);

    ATweenTool->AddFadeTo     (0.0f, 5.0f)
              ->SetRelative   (false)
              ->SetUserDataPtr(0, arrow)
              ->SetOnComplete (ArrowActionOnComplete)
              ->RunActions    (arrow->sprite->drawable);

    arrow->body->state = PhysicsBodyState_Freeze;
}



static Color tailColors[StoreWeapon_Length] =
{
    {174 / 255.0f, 161 / 255.0f, 73  / 255.0f, 1.0f},
    {211 / 255.0f, 198 / 255.0f, 186 / 255.0f, 1.0f},
    {228 / 255.0f, 202 / 255.0f, 12  / 255.0f, 1.0f},
    {216 / 255.0f, 33  / 255.0f, 123 / 255.0f, 1.0f},
    {191 / 255.0f, 192 / 255.0f, 224 / 255.0f, 1.0f},
    {51  / 255.0f, 190 / 255.0f, 193 / 255.0f, 1.0f},
    {138 / 255.0f, 104 / 255.0f, 48  / 255.0f, 1.0f},
    {182 / 255.0f, 165 / 255.0f, 155 / 255.0f, 1.0f},
    {150 / 255.0f, 152 / 255.0f, 171 / 255.0f, 1.0f},
    {69  / 255.0f, 150 / 255.0f, 49  / 255.0f, 1.0f},
};


static inline void ArrowTail(Arrow* arrow, float opacity)
{
    Color* color = tailColors + AGameData->weaponIndex;
    color->a     = opacity;

    AGLPrimitive->RenderLines
    (
        AArray_MakeWithData(arrow->tailPos, arrow->tailPosCurCount << 1),
        AGameMap->beforeDrawable->mvpMatrix,
        color,
        3.8f
    );
}



static void ArrowTailRender(Drawable *drawable)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(drawable->userData, 0, Arrow*);

    #ifdef APP_DEBUG
    AGLPrimitive->RenderPolygon
    (
        arrow->body->transformedVertexArr,
        AGameMap->beforeDrawable->mvpMatrix,
        COLOR_GREED_ARRAY,
        1.0f
    );
    #endif

    ArrowTail(arrow, arrowTailOpacity);
}


static void ArrowTailDisappearRender(Drawable *drawable)
{
    Arrow* arrow   = AUserData_GetSlotPtrWithType(drawable->userData, 0, Arrow*);
    float  opacity = AUserData_GetSlotFloat(arrow->tailDrawable->userData, 1);

    if (opacity > 0.0f)
    {
        ArrowTail(arrow, opacity);
    }
}


static void ArrowTailDisappearDraw(Drawable* tailDrawable)
{
    AUserData_AddSlotFloat(tailDrawable->userData, 1, arrowTailOpacityTime);
}


static void SetDirection(float touchX)
{
    Drawable* heroDrawable = AHero_GetDrawable();
    
    if (touchX * heroDrawable->scaleX < 0)
    {
        ADrawable_SetScaleX(heroDrawable, -heroDrawable->scaleX);
    }
}


static void OnHit(PhysicsBody* self, PhysicsBody* other, float deltaSeconds)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(self->userData,  0, Arrow*);
    Enemy* enemy = AUserData_GetSlotPtrWithType(other->userData, 0, Enemy*);

    if (arrow->hitType != ArrowHitType_Null)
    {
        return;
    }
    else if (enemy != NULL)
    {
        if
        (
            arrow->enemy                   != NULL  ||
            enemy->collisionBoxDrawable    == NULL  ||
            enemy->component->curState->id == EnemyState_DieOver
        )
        {
            return;
        }

        arrow->enemy   = enemy;
        arrow->hitType = ArrowHitType_Enemy;
        
        // need to delay processing collision
        AUserData_SetSlotPtr(AScheduler->ScheduleOnce(FreezeOnEnemy, 0)->userData, 0, arrow);
    }
    else
    {
        arrow->enemy   = NULL;
        arrow->hitType = ArrowHitType_Ground;

        // need to delay processing collision
        AUserData_SetSlotPtr(AScheduler->ScheduleOnce(FreezeOnOther, 0)->userData, 0, arrow);

        ++AHero->roundMissCount;
        AAudioTool->Play(AudioID_HurtGround);
    }

    float deform   = AMath_RandomFloat(0.12f, 0.22f);
    arrow->deformX = deform * arrow->sprite->drawable->width;
    arrow->deformU = deform * arrow->sprite->uvWidth;

    ASprite->DeformByIndex
    (
        arrow->sprite,
        
        AArray_Make
        (
            float,
            2,
            -arrow->deformX, -arrow->deformX
        ),

        AArray_Make
        (
            float,
            2,
            -arrow->deformU, -arrow->deformU
        ),

        AArray_Make
        (
            int,
            2,
            4, 6
        )
    );

    ++AHero->roundArrowCount;

    arrow->tailDrawable->Render = ArrowTailDisappearRender;
    arrow->tailDrawable->Draw   = ArrowTailDisappearDraw;
}


static void StartArrowPath(const char* arrowName, Drawable* arrowDrawable)
{
    if (isArrowPathShow == false)
    {
        isArrowPathShow = true;

        for (int i = 0; i < arrowPathCount; ++i)
        {
            SubMesh* subMesh = ADrawAtlas->GetQuad(arrowPathDrawAtlas, arrowName);
            arrowPath[i]     = subMesh;
            // move to outside screen
            ADrawable_SetPosition2(subMesh->drawable, -10.0f, -10.0f);
        }
    }

}


static void DisplayArrowPath()
{
    if (isArrowPathShow)
    {
        for (int i = 0; i < arrowPathCount; ++i)
        {
            SubMesh* subMesh = arrowPath[i];

            // simulate path
            APhysicsBody->Update(arrowPathBody, 0.07f);

            if (arrowPathBody->positionY < AGameMap->groundY)
            {
                // hide when underground
                ADrawable_SetInvisible(subMesh->drawable);
            }
            else
            {
                ADrawable_SetVisible(subMesh->drawable);

                ADrawable_SetPosition2
                (
                    subMesh->drawable,
                    arrowPathBody->positionX, arrowPathBody->positionY
                );

                ADrawable_SetRotationZ
                (
                    subMesh->drawable,
                    ADrawable->GetFlipRotationZ(subMesh->drawable, arrowPathBody->rotationZ)
                );
            }
        }
    }
}


static void ReleaseArrowPath()
{
    if (isArrowPathShow)
    {
        isArrowPathShow = false;

        for (int i = 0; i < arrowPathCount; ++i)
        {
            ADrawAtlas->ReleaseQuad(arrowPathDrawAtlas, arrowPath[i]);
        }
    }
}


static float strength;

static float touchDownX;
static float touchDownY;
static int   touchDownID    = -1;

static float touchDownPreX;
static float touchDownPreY;
static int   touchDownPreID = -1;


static void FireArrow(Component* component, bool isMultiTouch, bool isUpdateState)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(AHero->component->userData, 0, Arrow*);

    if (AHUD->CostPower(AGameData->arrowCostEnergy))
    {
        if (arrow != NULL)
        {
            if (isMultiTouch == false)
            {
                ReleaseArrowPath();
            }

            Drawable* arrowDrawable = arrow->sprite->drawable;
            AUserData_SetSlotPtr(component->userData, 0, NULL);
            ADrawable_SetScaleY (arrowDrawable, 1.0f);

            // body under beforeDrawable matrix
            ADrawable->ConvertToParent(arrowDrawable, AGameMap->beforeDrawable);

            arrow->body->rotationZ = ADrawable->GetWorldRotationZ(arrowDrawable);
            arrow->body->velocityX = AMath_Cos(arrow->body->rotationZ) * strength;
            arrow->body->velocityY = AMath_Sin(arrow->body->rotationZ) * strength;
            arrow->body->positionX = arrowDrawable->positionX;
            arrow->body->positionY = arrowDrawable->positionY;
            arrow->body->state     = PhysicsBodyState_Normal;

            if (strength > 2.0f)
            {
                AAudioTool->Play(AudioID_ShootFast);
            }
            else
            {
                AAudioTool->Play(AudioID_Shoot);
            }
        }
        else
        {
            AAudioTool->Play(AudioID_NoArrow);
        }
    }

    if (isUpdateState && AHero->component->curState->id < HeroState_Die)
    {
        if (isMultiTouch == false)
        {
            AComponent->SetState(AHero->component, HeroState_Shoot);
        }
        else
        {
            AComponent->SetState(AHero->component, HeroState_Shoot_Fast);
        }
    }
}


static void ReadyMove(Component* component, float touchX, float touchY)
{
    Arrow* arrow = AUserData_GetSlotPtrWithType(component->userData, 0, Arrow*);

    if (arrow != NULL && AHero->roundEnergy >= AGameData->arrowCostEnergy)
    {
        float     deltaX        = (touchDownX - touchX) * (AHero_GetDrawable()->scaleX > 0 ? 1 : -1);
        float     deltaY        =  touchDownY - touchY;
        Drawable* arrowDrawable =  arrow->sprite->drawable;

        ADrawable_SetRotationZ(arrowDrawable, AMath_Atan2(deltaX, deltaY));
        ADrawable_SetScaleY   (arrowDrawable, 1.0f - fabsf(deltaX) * 0.25f);

        strength = AVector2_Length2
        (
            deltaX * 1.15f * AGameData->arrowSpeed,
            deltaY * 0.75f * AGameData->arrowSpeed
        );

        arrowPathBody->rotationZ = ADrawable->GetWorldRotationZ(arrowDrawable);
        arrowPathBody->velocityX = AMath_Cos(arrowPathBody->rotationZ) * strength;
        arrowPathBody->velocityY = AMath_Sin(arrowPathBody->rotationZ) * strength;

        // body path pos under beforeDrawable matrix
        Vector2 outLocal[1];

        ADrawable->ConvertBetweenLocalPositionV2
        (
            arrowDrawable->parent,
            AVector2_Make(arrowDrawable->positionX, arrowDrawable->positionY),
            AGameMap->beforeDrawable,
            outLocal
        );

        arrowPathBody->positionX = outLocal->x;
        arrowPathBody->positionY = outLocal->y;

        DisplayArrowPath();
    }
}


static void Ready(Component* component, bool isMultiTouch)
{
    if (AHero->roundEnergy >= AGameData->arrowCostEnergy)
    {
        Arrow*      arrow     = AArrayList_Pop(arrowList, Arrow*);
        const char* arrowName = AUIStore->weaponNames[AGameData->weaponIndex];
        char        buff[22];
        sprintf(buff, "Texture/%s.png", arrowName);

        if (arrow == NULL)
        {
            arrow        = malloc(sizeof(Arrow));
            arrow->enemy = NULL;
            arrow->body  = APhysicsWorld->AddBody
                           (
                               PhysicsShape_Line,
                               AArray_Make
                               (
                                   float, 4,
                                   0.0f, 0.0f,
                                   AGLTool_ToGLWidth(20.0f), 0.0f
                               )
                           );

            APhysicsBody_SetCollisionGroup
            (
                arrow->body,
                CollisionGroup_HeroAttack | CollisionGroup_ArrowAttack
            );

            AUserData_SetSlotPtr(arrow->body->userData, 0, arrow);
            arrow->body->OnCollision = OnHit;

            ASprite->InitWithFile(buff, arrow->sprite);
            arrow->halfWidth = arrow->sprite->drawable->width / 2.6f;
        }
        else
        {
            arrow->sprite->texture = ATexture->Get(buff);
            ADrawable_SetVisible(arrow->sprite->drawable);
            ADrawable_SetScale2 (arrow->sprite->drawable, 1.0f, 1.0f);
        }

        ATool->InitBox(arrow->body, 2.0f, -2.0f);
        arrow->body->state = PhysicsBodyState_Freeze;
        ADrawable_SetParent   (arrow->sprite->drawable, AHero->bowHandDrawable);
        ADrawable_SetPosition2(arrow->sprite->drawable, AGLTool_ToGLWidth(22), 0.0f);
        ADrawable_SetOpacity  (arrow->sprite->drawable, 0.0f);

        ADrawable_SetRotationZ
        (
            arrow->sprite->drawable,
            AMath_Atan2
            (
                touchDownX * (AHero_GetDrawable()->scaleX > 0 ? 1 : -1),
                touchDownY
            )
        );

        ATweenTool->AddFadeTo  (1.0f, 0.3f)
                  ->SetEaseType(TweenEaseType_SineInOut)
                  ->RunActions (arrow->sprite->drawable);

        arrow->hitType         = ArrowHitType_Null;
        arrow->enemy           = NULL;
        arrow->tailPosCurCount = 0;
        arrow->tailPosCurTime  = 0.0f;

        ADrawable->Init       (arrow->tailDrawable);
        AUserData_SetSlotPtr  (arrow->tailDrawable->userData, 0, arrow);
        AUserData_SetSlotFloat(arrow->tailDrawable->userData, 1, arrowTailOpacity);
        arrow->tailDrawable->Render = ArrowTailRender;
        arrow->tailDrawable->Draw   = NULL;

        AUserData_SetSlotPtr(component->userData, 0, arrow);
        AArrayIntSet->TryAdd(arrowSet, (intptr_t) arrow);

        if (isMultiTouch == false)
        {
            StartArrowPath(arrowName, arrow->sprite->drawable);
        }
        else
        {
            ReadyMove(component, touchDownPreX, touchDownPreY);
        }
    }
}


static bool OnReadyMessage(Component* component, Array(InputTouch*)* touchArr)
{
    InputTouch* touch;

    if (touchArr->length == 1)
    {
        touch = AArray_Get(touchArr, 0, InputTouch*);
    }
    else
    {
        for (int i = 0; i < touchArr->length; ++i)
        {
            touch = AArray_Get(touchArr, i, InputTouch*);

            if (touch->fingerID == touchDownID && touch->type == InputTouchType_Move)
            {
                ReadyMove(component, touch->x, touch->y);
                break;
            }
        }

        return false;
    }

    switch (touch->type)
    {
        case InputTouchType_Cancel:
            break;
        
        case InputTouchType_Down:
            if (touch->fingerID != touchDownID)
            {
                // after fire arrow will use this touch
                touchDownPreID = touch->fingerID;
                touchDownPreX  = touch->x;
                touchDownPreY  = touch->y;
            }
            break;

        case InputTouchType_Move:
        {
            if (touch->fingerID == touchDownID)
            {
                ReadyMove(component, touch->x, touch->y);
            }
            break;
        }

        case InputTouchType_Up:
        {
            if (touch->fingerID == touchDownID)
            {
                FireArrow(component, false, true);

                touchDownID    = touchDownPreID;
                touchDownX     = touchDownPreX;
                touchDownY     = touchDownPreY;
                touchDownPreID = -1;
            }
            else if (touch->fingerID == touchDownPreID)
            {
                FireArrow(component, true, true);

                touchDownPreID = -1;
                ReleaseArrowPath();

                if (touchDownID != -1)
                {
                    // record touch down for call the ReadyMove()
                    touch          = AInput->GetTouch(touchDownID);
                    touchDownPreX  = touch->x;
                    touchDownPreY  = touch->y;
                }
            }
            break;
        }
    }

    return false;
}


static inline bool OnKeepMessage(Component* component, Array(InputTouch*)* touchArr)
{
    return OnReadyMessage(component, touchArr);
}


static inline bool OnDizzyMessage(Array(InputTouch*)* touchArr)
{
    InputTouch* touch = AArray_Get(touchArr, 0, InputTouch*);

    switch (touch->type)
    {
        case InputTouchType_Move:
            break;
        
        case InputTouchType_Cancel:
            break;
        
        case InputTouchType_Down:
            touchDownPreID = touchDownID;
            touchDownX     = touchDownPreX;
            touchDownY     = touchDownPreY;

            touchDownX     = touch->x;
            touchDownY     = touch->y;
            touchDownID    = touch->fingerID;
            break;

        case InputTouchType_Up:
            if (touch->fingerID == touchDownID)
            {
                touchDownID    = touchDownPreID;
                touchDownX     = touchDownPreX;
                touchDownY     = touchDownPreY;
                touchDownPreID = -1;
            }
            else if (touch->fingerID == touchDownPreID)
            {
                touchDownPreID = -1;
            }
            break;
    }
    
    return false;
}


static void WalkOrReady(float touchX)
{
    Drawable* heroDrawable = AHero_GetDrawable();

    if
    (
        (touchX < -AGLTool->screenRatio * 0.7f) ||
        (touchX >  AGLTool->screenRatio * 0.7f)
    )
    {
        SetDirection(touchX);
        AComponent->SetState(AHero->component, HeroState_Walk);
    }
    else
    {
        if
        (
            (heroDrawable->scaleX > 0 && touchX < heroDrawable->positionX) ||
            (heroDrawable->scaleX < 0 && touchX > heroDrawable->positionX)
        )
        {
            AComponent->SetState(AHero->component, HeroState_Ready);
        }
    }
}


static bool isDizzyLocked = false;


/**
 * Handle stand and walk.
 */
static bool OnStandMessage(Array(InputTouch*)* touchArr)
{
    InputTouch* touch;

    if (touchArr->length == 1)
    {
        touch = AArray_Get(touchArr, 0, InputTouch*);
    }
    else
    {
        for (int i = 0; i < touchArr->length; ++i)
        {
            touch = AArray_Get(touchArr, i, InputTouch*);

            if (touch->fingerID == touchDownID && touch->type == InputTouchType_Move)
            {
                goto DoMove;
            }
        }

        return false;
    }

    switch (touch->type)
    {
        case InputTouchType_Cancel:
            break;  
        
        case InputTouchType_Down:
        {
            WalkOrReady(touch->x);

            touchDownPreID = touchDownID;
            touchDownPreX  = touchDownX;
            touchDownPreY  = touchDownY;

            touchDownX     = touch->x;
            touchDownY     = touch->y;
            touchDownID    = touch->fingerID;
            break;
        }

        case InputTouchType_Move:
        {
            if (touch->fingerID == touchDownID)
            {
                DoMove:
                
                if (isDizzyLocked == false)
                {
                    Drawable* heroDrawable = AHero_GetDrawable();
                    float     deltaX       = fabsf(touchDownX - touch->x);
                    float     deltaY       = touchDownY - touch->y;

                    if (deltaY > 0.08f && deltaX < 0.1f)
                    {
                        if
                        (
                            (heroDrawable->scaleX > 0 && touch->x > heroDrawable->positionX) ||
                            (heroDrawable->scaleX < 0 && touch->x < heroDrawable->positionX)
                        )
                        {
                            isDizzyLocked = true;
                            AComponent->SetState(AHero->component, HeroState_Dizzy);
                        }
                    }
                    else if (deltaY < 0.1f && deltaX > 0.1f)
                    {
                        ADrawable_SetScaleX (heroDrawable,    -heroDrawable->scaleX);
                        AComponent->SetState(AHero->component, HeroState_Ready);
                    }
                }
            }
            break;
        }

        case InputTouchType_Up:
        {
            if (touch->fingerID == touchDownID)
            {
                // block dizzy continues trigger
                isDizzyLocked  = false;
                touchDownID    = touchDownPreID;
                touchDownX     = touchDownPreX;
                touchDownY     = touchDownPreY;
                touchDownPreID = -1;
            }
            else if (touch->fingerID == touchDownPreID)
            {
                touchDownPreID = -1;
            }
            break;
        }
    }

    return false;
}


static inline bool OnWalkMessage(Array(InputTouch*)* touchArr)
{
    InputTouch* touch = AArray_Get(touchArr, 0, InputTouch*);

    switch (touch->type)
    {
        case InputTouchType_Up:
            if (touch->fingerID == touchDownID)
            {
                isDizzyLocked  = false;
                touchDownID    = touchDownPreID;
                touchDownX     = touchDownPreX;
                touchDownY     = touchDownPreY;
                touchDownPreID = -1;

                if (touchDownID == -1)
                {
                    AComponent->SetState(AHero->component, HeroState_Stand);
                }
                else
                {
                    SetDirection(touchDownX);
                }
            }
            else if (touch->fingerID == touchDownPreID)
            {
                touchDownPreID = -1;
            }
            break;

        default:
            OnStandMessage(touchArr);
            break;
    }

    return false;
}


static void WalkUpdate(Component* component, float deltaSeconds)
{
    Update(component, deltaSeconds);

    float     speed        = AGLTool_ToGLWidth(AGameData->speed);
    float     middleSpeed  = speed       * 0.69f;
    float     backSpeed    = middleSpeed * 0.52f;
    Drawable* heroDrawable = AHero_GetDrawable();

    if (touchDownX > 0)
    {
        if (heroDrawable->positionX < 0)
        {
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX + speed);
        }
        else if (AGameMap->maxX > AGameMap->groundPosData[4])
        {
            AGameMap->maxX -= speed;
            AGameMap->minX -= speed;

            ADrawable_SetPositionX(AGameMap->beforeDrawable, AGameMap->beforeDrawable->positionX - speed);
            ADrawable_SetPositionX(AGameMap->middleDrawable, AGameMap->middleDrawable->positionX - middleSpeed);
            ADrawable_SetPositionX(AGameMap->backDrawable,   AGameMap->backDrawable->positionX   - backSpeed);
        }
        else if (heroDrawable->positionX < AGameMap->groundPosData[4])
        {
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX + speed);
        }
    }
    else if (touchDownX < 0)
    {
        if (heroDrawable->positionX > 0)
        {
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX - speed);
        }
        else if (AGameMap->minX < AGameMap->groundPosData[2])
        {
            AGameMap->minX += speed;
            AGameMap->maxX += speed;

            ADrawable_SetPositionX(AGameMap->beforeDrawable, AGameMap->beforeDrawable->positionX + speed);
            ADrawable_SetPositionX(AGameMap->middleDrawable, AGameMap->middleDrawable->positionX + middleSpeed);
            ADrawable_SetPositionX(AGameMap->backDrawable,   AGameMap->backDrawable->positionX   + backSpeed);
        }
        else if (heroDrawable->positionX > AGameMap->groundPosData[2])
        {
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX - speed);
        }
    }
}


static inline bool OnShootFastMessage(Array(InputTouch*)* touchArr)
{
    InputTouch* touch = AArray_Get(touchArr, 0, InputTouch*);

    // just check if need to clear the touch
    switch (touch->type)
    {
        case InputTouchType_Up:
            if (touch->fingerID == touchDownID)
            {
                touchDownID = -1;
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            else if (touch->fingerID == touchDownPreID)
            {
                touchDownPreID = -1;
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            break;

        default:
            break;
    }

    return false;
}


static inline bool OnHurtMessage(Array(InputTouch*)* touchArr)
{
    InputTouch* touch = AArray_Get(touchArr, 0, InputTouch*);

    // just check if need to clear the touch
    switch (touch->type)
    {
        case InputTouchType_Up:
            if (touch->fingerID == touchDownID)
            {
                touchDownID    = touchDownPreID;
                touchDownX     = touchDownPreX;
                touchDownY     = touchDownPreY;
                touchDownPreID = -1;
            }
            else if (touch->fingerID == touchDownPreID)
            {
                touchDownPreID = -1;
            }
            break;

        default:
            break;
    }

    return false;
}


static void OnActionOver(SkeletonAnimationPlayer* player)
{
    switch (AHero->component->curState->id)
    {
        case HeroState_Shoot:
            if (touchDownID != -1)
            {
                SetDirection(touchDownX);
                AComponent->SetState(AHero->component, HeroState_Walk);
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            break;

        case HeroState_Shoot_Fast:
            AComponent->SetState(AHero->component, HeroState_Ready);
            break;

        case HeroState_Hurt:
            if (touchDownID != -1)
            {
                // keep direction unchanged
                // so not call the SetDirection
                AComponent->SetState(AHero->component, HeroState_Walk);
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            break;

        case HeroState_Dizzy:
        {
            if (touchDownID != -1)
            {
                // if touch down when dizzy animation
                // continue the touch down effect
                WalkOrReady(touchDownX);
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }

            if (AHUD->CostPower(AGameData->hitFloorCostEnergy))
            {
                ATool->ShakeWorld();

                ADrawable_SetPositionX
                (
                    ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor),
                    ADrawable->ConvertBetweenLocalPositionX
                    (
                        AGameMap->groundPosDrawable,
                        AHero_GetDrawable()->positionX,
                        AGameMap->beforeDrawable
                    )
                );

                AHero->hitFloor->loop = 1;
                AComponent->Notify(AHero->component, HeroNotify_Dizzy, NULL);

                AAudioTool->Play(AudioID_FallDown);
                AVibrator->Vibrate(100);
            }
            else
            {
                // no power hit floor, release dizzy lock
                isDizzyLocked = false;
            }
            break;
        }

        case HeroState_Ready:
            AComponent->SetState(AHero->component, HeroState_Keep);
            break;

        case HeroState_Die:
            AComponent->SetState(AHero->component, HeroState_DieOver);
            AComponent->Notify  (AHero->component, HeroNotify_Die, NULL);
            break;

        default:
            break;
    }
}


static void CurtainPlayerCallBack(SkeletonAnimationPlayer* player)
{
    for (int i = 0; i < arrowSet->elementList->size; ++i)
    {
        Arrow* arrow       = AArrayList_Get(arrowSet->elementList, i, Arrow*);
        bool   isHasAction = ATween->TryCompleteAllActions(arrow->sprite->drawable, false);

        // arrow not collision yet
        if (isHasAction == false)
        {
            arrow->body->state = PhysicsBodyState_Freeze;
            ++AHero->roundMissCount;
            ++AHero->roundArrowCount;
        }

        ResetArrow(arrow);
    }

    AArrayList->Clear(arrowSet->elementList);

    AHUD->ClearDrop();
    AUI->ShowFail  ();
}


static void OpenCurtain(Scheduler* scheduler, float deltaSeconds)
{
    AAudioTool->Play(AudioID_Over);
    AHUD->OpenCurtain(CurtainPlayerCallBack);
}


static void TombstoneActionComplete(TweenAction* action)
{
    ATweenTool->AddFadeTo  (0.0f, 1.0f)
              ->SetRelative(false)
              ->RunActions (AHero_GetDrawable());

    AHero->hitFloor->loop = 1;

    ADrawable_SetPositionX
    (
        ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor),
        ADrawable->ConvertBetweenLocalPositionX
        (
            AGameMap->groundPosDrawable,
            AHero_GetDrawable()->positionX,
            AGameMap->beforeDrawable
        )
    );

    AEnemyAI  ->SetAllEnemy (EnemyState_Stand, 0.01f);
    AScheduler->ScheduleOnce(OpenCurtain, 1.0f);

    AAudioTool->Play(AudioID_FallDown);
}


static void DieActionComplete(TweenAction* action)
{
    ADrawable_SetVisible  (AHUD->tombstoneDrawable);
    ADrawable_SetPositionX(AHUD->tombstoneDrawable, AHero_GetDrawable()->positionX);

    ATweenTool->AddMoveY
                (
                    AGameMap->groundY + AHUD->tombstoneHeight / 2,
                    0.35f
                )
              ->SetEaseType  (TweenEaseType_ExponentialIn)
              ->SetRelative  (false)
              ->SetOnComplete(TombstoneActionComplete)
              ->RunActions   (AHUD->tombstoneDrawable);
}


static void OnHurt(PhysicsBody* self, PhysicsBody* other, float deltaSeconds)
{
    if
    (
        other->userID                  >= CollisionBodyID_Heart &&
        other->userID                  <= CollisionBodyID_Wood  &&
        AHero->component->curState->id <  HeroState_Die
    )
    {
        AHUD->FlyItem(AUserData_GetSlotPtrWithType(other->userData, 0, DropCollisionItem*));
        return;
    }

    if (AHero->component->curState->id < HeroState_Hurt && isDizzyLocked == false)
    {
        // fire arrow if needed
        FireArrow(AHero->component, false, false);

        Enemy* enemy = AUserData_GetSlotPtrWithType(other->userData, 0, Enemy*);

        if (enemy->component->curState->id >= EnemyState_Dizzy)
        {
            return;
        }

        float move;
        if (AEnemy_GetDrawable(enemy)->scaleX > 0.0f)
        {
            move = -0.08f;
        }
        else
        {
            move =  0.08f;
        }

        --AHero->roundHP;
        AFont->SetInt(AHUD->heartText, AHero->roundHP);

        if (AHero->roundHP > 0)
        {
            AComponent->SetState(AHero->component, HeroState_Hurt);

            ATweenTool->AddMoveX   (move, 0.3f)
                      ->SetEaseType(TweenEaseType_SineOut)
                      ->SetQueue   (false)

                      ->AddMoveY   (0.015f,  0.1f)
                      ->SetEaseType(TweenEaseType_SineOut)

                      ->AddMoveY   (-0.015f, 0.1f)
                      ->SetEaseType(TweenEaseType_SineOut)

                      ->RunActions (AHero_GetDrawable());
        }
        else
        {
            AComponent->SetState(AHero->component, HeroState_Die);

            ATweenTool->AddMoveX     (move * 2, 1.25f)
                      ->SetEaseType  (TweenEaseType_SineOut)
                      ->SetQueue     (false)
                      ->SetOnComplete(DieActionComplete)

                      ->AddMoveY     (0.02f,  0.15f)
                      ->SetEaseType  (TweenEaseType_SineOut)

                      ->AddMoveY     (-0.02f,  0.15f)
                      ->SetEaseType  (TweenEaseType_SineOut)

                      ->RunActions   (AHero_GetDrawable());
        }
    }
}


static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (sender == AApplication) // touch event
    {
        Array(InputTouch*)* touchArr = extraData;

        switch (component->curState->id)
        {
            case HeroState_Stand:
                OnStandMessage(touchArr);
                break;

            case HeroState_Walk:
                OnWalkMessage(touchArr);
                break;

            case HeroState_Ready:
                OnReadyMessage(component, touchArr);
                break;

            case HeroState_Keep:
                OnKeepMessage(component, touchArr);
                break;

            case HeroState_Shoot:
                OnStandMessage(touchArr);
                break;

            case HeroState_Shoot_Fast:
                OnShootFastMessage(touchArr);
                break;

            case HeroState_Dizzy:
                OnDizzyMessage(touchArr);
                break;

            case HeroState_Hurt:
                OnHurtMessage(touchArr);
                break;

            default:
                break;
        }
    }
    else if (sender == AComponent)  // Component msg
    {
        switch (subject)
        {
            case ComponentMsg_OnEnter:
            {
                switch (component->curState->id)
                {
                    case HeroState_Stand:
                        if (component->preState->id == HeroState_Walk || component->preState->id == HeroState_Shoot_Fast)
                        {
                            ASkeletonAnimationPlayer->SetAnimationMix(AHero->player, "stand", 0.2f);
                        }
                        else
                        {
                            ASkeletonAnimationPlayer->SetAnimation(AHero->player, "stand");
                        }
                        break;

                    case HeroState_Walk:
                        ASkeletonAnimationPlayer->SetAnimationMix(AHero->player, "walk", 0.1f);
                        break;

                    case HeroState_Shoot:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "shoot");
                        break;

                    case HeroState_Shoot_Fast:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "shoot_fast");
                        break;

                    case HeroState_Ready:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "ready");
                        
                        if (component->preState->id != HeroState_Shoot_Fast)
                        {
                            Ready(component, false);
                        }
                        else
                        {
                            Ready(component, true);
                        }
                        break;

                    case HeroState_Keep:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "keep");
                        break;

                    case HeroState_Hurt:
                        AAudioTool->Play(AudioID_HurtHero);
                        AVibrator->Vibrate(50);
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "hurt");
                        break;

                    case HeroState_Dizzy:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "dizzy");
                        break;

                    case HeroState_Die:
                        AAudioTool->Play(AudioID_HeroDie);
                        AVibrator->Vibrate(50);
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "die");
                        break;

                    case HeroState_DieOver:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "die_over");
                        break;

                    default:
                        break;
                }
            }
            break;

            default:
                break;
        }
    }

    return false;
}


static inline void InitProperty()
{
    AHero->roundHP         = (int) AGameData->maxHP; // for compatible with archived data, maxHP cannot use int
    AHero->roundEnergy     = AGameData->maxEnergy;

    AHero->roundKillCount  = 0;
    AHero->roundArrowCount = 0;
    AHero->roundMissCount  = 0;
    AHero->roundScore      = 0;
    AHero->roundTime       = 0.0f;

    AGameData->dressIndex  = AMath_RandomInt(0, StoreDress_Length  - 1);
    AGameData->weaponIndex = AMath_RandomInt(0, StoreWeapon_Length - 1);

    char buff[20];
    sprintf(buff, "%s%s", AUIStore->dressNames[AGameData->dressIndex], AUIStore->weaponNames[AGameData->weaponIndex]);
    ASkeleton->SetSkin(AHero->player->skeleton, buff);
}


static void Init()
{
    AComponent->Init(AHero->component);
    ASkeletonAnimationPlayer->Init("Animation/redgirl", "stand", AHero->player);

    Drawable* heroDrawable = AHero_GetDrawable();
    ADrawable_SetParent   (heroDrawable, AGameMap->beforeDrawable);
    ADrawable_SetPositionY(heroDrawable, AGameMap->groundY);

    SkeletonSlot* collisionSlot = ASkeletonAnimationPlayer_GetSlot(AHero->player, "CollisionBox");
    if (collisionSlot != NULL)
    {
        AHero->collisionBoxDrawable          = collisionSlot->bone->drawable;
        AHero->collisionBoxBody              = APhysicsWorld->AddBody
                                               (
                                                   PhysicsShape_Polygon,
                                                   ASkeletonSlot_GetBoundingBox(collisionSlot)->vertexArr
                                               );
        AHero->collisionBoxBody->OnCollision = OnHurt;
        AHero->collisionBoxBody->state       = PhysicsBodyState_Fixed;
        APhysicsBody_SetCollisionGroup
        (
            AHero->collisionBoxBody,
            CollisionGroup_HeroBody | CollisionGroup_HeroAttack | CollisionGroup_EnemyBody
        );

        #ifdef APP_DEBUG
        ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable
        (
            AHero->player,
            "CollisionBox",
            debugCollisionBoundingBox
        );
        #endif
    }

    AHero->bowHandDrawable      = ASkeletonAnimationPlayer_GetBone(AHero->player, "righthand-up")->drawable;
    AHero->player->OnActionOver = OnActionOver;

    ASkeletonAnimationPlayer->Init("Animation/hit-floor", "animation", AHero->hitFloor);
    AHero->hitFloor->loop       = 0;

    Drawable* hitFloorDrawable = ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor);
    ADrawable_SetParent   (hitFloorDrawable, AGameMap->beforeDrawable);
    ADrawable_SetPositionY(hitFloorDrawable, AGameMap->groundY);

    AHero->component->stateMap->elementList->increase = 10;

    AComponent->AddState(AHero->component, HeroState_Stand,      OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Walk,       OnMessage, WalkUpdate);
    AComponent->AddState(AHero->component, HeroState_Shoot,      OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Shoot_Fast, OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Ready,      OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Keep,       OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Hurt,       OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Dizzy,      OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Die,        OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_DieOver,    OnMessage, Update);

    AComponent->SetState(AHero->component, HeroState_Stand);

    arrowPathDrawAtlas = ADrawAtlas->Get("Texture/Arrow.atlas");
    // all bodies under beforeDrawable matrix
    ADrawable_SetParent(arrowPathDrawAtlas->mesh->drawable, AGameMap->beforeDrawable);

    AHero->roundCount = 0;
}


static void Revive()
{
    InitProperty();

    ADrawable_SetParent   (ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor), AGameMap->beforeDrawable);
    ADrawable_SetPositionY(ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor), AGameMap->groundY);

    ADrawable_SetParent   (AHero_GetDrawable(),      AGameMap->beforeDrawable);
    ADrawable_SetPositionY(AHero_GetDrawable(),      AGameMap->groundY);
    ADrawable_SetPositionX(AHero_GetDrawable(),      0.0f);   
    ADrawable_SetOpacity  (AHero_GetDrawable(),      1.0f);

    ADrawable_SetPositionX(AGameMap->beforeDrawable, 0.0f);
    ADrawable_SetPositionX(AGameMap->middleDrawable, 0.0f);
    ADrawable_SetPositionX(AGameMap->backDrawable,   0.0f);

    ADrawable_SetInvisible(AHUD->tombstoneDrawable);
    ADrawable_SetPositionY(AHUD->tombstoneDrawable,  AHUD->tombstoneBornY);

    ADrawable_SetParent(arrowPathDrawAtlas->mesh->drawable, AGameMap->beforeDrawable);

    AGameMap->minX = AGameMap->groundPosData[0];
    AGameMap->maxX = AGameMap->groundPosData[6];

    touchDownID    = -1;
    touchDownPreID = -1;

    AComponent->SetState(AHero->component, HeroState_Stand);
}


static void Run()
{
    AApplication_AddChild(AHero->component, 60);
    AArrayIntSet->Clear(AHero->component->observerSet);
    InitProperty();
}


static void TalkTextTweenActionOnComplete(TweenAction* tweenAction)
{
    AFont->ReleaseText(AUserData_GetSlotPtrWithType(tweenAction->userData, 0, FontText*));
}


static void RoundScore()
{
    ++AHero->roundKillCount;
    AFont->SetInt(AHUD->killText, AHero->roundKillCount);

    int score = ((int) AHero->roundTime + AHero->roundKillCount) / (Enemy_KillIncrease + AEnemyAI->currentEnemyNum) + 1;

    AHero->roundScore += score;

    FontText* text = AFont->GetText(AGameActor->talkFont);
    char buff[12];
    sprintf(buff, "+%d", score);
    AFont->SetString(text, buff);
    AFont_SetTextColor(text, AColor_Make(0.72f, 0.99f, 0.38f, 1.0f));
    ATool->FlyTextOnHero(text, 0.045f, 3.0f);

    AHero->roundEnergy += score;
}


struct AHero AHero[1] =
{{
    .Init       = Init,
    .Run        = Run,
    .Revive     = Revive,
    .ResetArrow = ResetArrow,
    .RoundScore = RoundScore,
}};
