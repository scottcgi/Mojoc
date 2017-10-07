/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-9-17
 * Author : scott.cgi
 * Version: 0.0.0
 */

#include <stdio.h>
#include <string.h>

#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Application/Input.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Graphics/OpenGL/Sprite.h"
#include "Engine/Extension/Font.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Application/Application.h"

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


static float touchDownX;
static float touchDownY;
static int   touchDownId = -1;


static ArrayIntSet(Arrow*) arrowSet [1] = AArrayIntSet_Init(Arrow*, 25);
static ArrayList  (Arrow*) arrowList[1] = AArrayList_Init  (Arrow*, 25);


#ifdef APP_DEBUG
static Drawable debugCollisionBoundingBox[1];
#endif


static void Update(Component* component, float deltaSeconds)
{
    ASkeletonAnimationPlayer_Update(AHero->player,   deltaSeconds);
    ASkeletonAnimationPlayer_Update(AHero->hitFloor, deltaSeconds);

    if (AHero->component->curState->id < HeroState_Die)
    {
        AHero->roundTime += deltaSeconds;
    }

    if (AHero->collisionBoxBody != NULL)
    {
        if (APhysicsBody_CheckState(AHero->collisionBoxBody, PhysicsBodyState_IsFreeze) == false)
        {
            ATool->UpdateBox(AHero->collisionBoxBody, AHero->collisionBoxDrawable);
        }


        #ifdef APP_DEBUG
        ADrawable->Draw(debugCollisionBoundingBox);
        #endif
    }

    for (int i = 0; i < arrowSet->elementList->size; i++)
    {
        Arrow*       arrow    = AArrayList_Get(arrowSet->elementList, i, Arrow*);
        PhysicsBody* body     = arrow->body;
        Drawable*    drawable = arrow->sprite->drawable;

        if (APhysicsBody_CheckState(body, PhysicsBodyState_IsFreeze) == false)
        {
            ADrawable_SetRotationZ(drawable, ADrawable->GetFlipRotationZ(drawable, body->rotationZ));
            ADrawable_SetPosition2(drawable, body->positionX, body->positionY);
        }

        ADrawable->Draw(drawable);


        #ifdef APP_DEBUG
        if (body != NULL)
        {
            ADrawable->Draw(arrow->debugDrawable);
        }
        #endif
    }
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

    ADrawable_SetInVisible(arrow->sprite->drawable);

    // reset body born position
    memcpy
    (
        arrow->body->positionArr->data,
        arrow->body->vertexArr->data,
        sizeof(float) * arrow->body->positionArr->length
    );

    AArrayList_Add(arrowList, arrow);
}


static void ArrowActionOnComplete(TweenAction* tweenAction)
{
    Arrow* arrow = (Arrow*) tweenAction->userData->slot0->ptrValue;
    ArrowOnComplete(arrow);
}


static void FreezeOnEnemy(Scheduler* scheduler, float deltaSeconds)
{
    Arrow* arrow = (Arrow*) scheduler->userData->slot0->ptrValue;

    AArrayIntSet->TryRemove(arrowSet,               (intptr_t) arrow);
    AArrayIntSet->TryAdd   (arrow->enemy->arrowSet, (intptr_t) arrow);

    float fadeTime;

    if (arrow->enemy->hp < 0)
    {
        fadeTime = 2.0f;
    }
    else
    {
        fadeTime = 6.0f;
    }

    ATweenTool->AddFadeTo      (0.0f, fadeTime)
              ->SetRelative    (false)
              ->SetUserData0Ptr(arrow)
              ->SetOnComplete  (ArrowActionOnComplete)
              ->RunActions     (arrow->sprite->drawable);

    ADrawable->ConvertToParent
    (
        arrow->sprite->drawable,
        arrow->enemy->collisionBoxDrawable
    );

    APhysicsBody_SetState(arrow->body, PhysicsBodyState_IsFreeze);
}


static void FreezeOnOther(Scheduler* scheduler, float deltaSeconds)
{
    Arrow* arrow = (Arrow*) scheduler->userData->slot0->ptrValue;

    ATweenTool->AddFadeTo      (0.0f, 5.0f)
              ->SetRelative    (false)
              ->SetUserData0Ptr(arrow)
              ->SetOnComplete  (ArrowActionOnComplete)
              ->RunActions     (arrow->sprite->drawable);

    APhysicsBody_SetState(arrow->body, PhysicsBodyState_IsFreeze);
}


static void OnHit(PhysicsBody* self, PhysicsBody* other, float deltaSeconds)
{
    Arrow* arrow = (Arrow*) self->userData->slot0->ptrValue;
    Enemy* enemy = (Enemy*) other->userData->slot0->ptrValue;

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
        AScheduler->ScheduleOnce(FreezeOnEnemy, 0)->userData->slot0->ptrValue = arrow;
    }
    else
    {
        arrow->enemy   = NULL;
        arrow->hitType = ArrowHitType_Ground;
        AScheduler->ScheduleOnce(FreezeOnOther, 0)->userData->slot0->ptrValue = arrow;
        AHero->roundMissCount++;

        AAudioTool->Play(AudioId_HurtGround);
    }

    AHero->roundArrowCount++;
}


#ifdef APP_DEBUG
static void DebugRender(Drawable* drawable)
{
    Arrow* arrow = (Arrow*) drawable->userData->slot0->ptrValue;

    AGLPrimitive->color->r    = 0.0f;
    AGLPrimitive->color->b    = 0.0f;
    AGLPrimitive->color->g    = 1.0f;
    AGLPrimitive->modelMatrix = AGameMap->beforeDrawable->modelMatrix;
    AGLPrimitive->DrawPolygon(arrow->body->positionArr);
}
#endif


static float readyX;
static float readyY;


static inline bool OnReadyMessage(Component* component, void* sender, int subject, void* extraData)
{
    Arrow*           arrow    = (Arrow*) component->userData->slot0->ptrValue;
    Array*           pointArr = (Array*) extraData;
    InputTouch*      touch;

    if (touchDownId < 2 && pointArr->length > 1)
    {
        touch = AArray_Get((Array*) extraData, touchDownId, InputTouch*);
    }
    else
    {
        touch = AArray_Get((Array*) extraData, 0, InputTouch*);
    }

    switch (touch->type)
    {
        case InputTouchType_Cancel:
            break;
            
        case InputTouchType_Down:
            break;

        case InputTouchType_Move:
        {
            if (touch->fingerId == touchDownId)
            {
                if (arrow != NULL && AHero->roundEnergy >= AGameData->arrowCostEnergy)
                {
                    float deltaX = touchDownX - touch->x;
                    float deltaY = touchDownY - touch->y;

                    ADrawable_SetRotationZ
                    (
                        arrow->sprite->drawable,
                        AMath_Atan2
                        (
                            deltaX * (AHero_GetDrawable()->scaleX > 0 ? 1 : -1),
                            deltaY
                        )
                    );

                    readyX = touch->x;
                    readyY = touch->y;
                }
            }

            break;
        }

        case InputTouchType_Up:
        {
            if (touch->fingerId == touchDownId)
            {
                if (AHero->component->curState->id < HeroState_Die)
                {
                    AComponent->SetState(AHero->component, HeroState_Shoot);
                }

                if (AHUD->CostPower(AGameData->arrowCostEnergy) && arrow != NULL)
                {
                    Drawable* arrowDrawable = arrow->sprite->drawable;

                    component->userData->slot0->ptrValue = NULL;
                    ADrawable->ConvertToParent(arrowDrawable, AGameMap->beforeDrawable);

                    float deltaX           = (touch->x - touchDownX) * 1.1f  * AGameData->arrowSpeed;
                    float deltaY           = (touch->y - touchDownY) * 0.75f * AGameData->arrowSpeed;
                    float strength         = AVector2_Length2(deltaX, deltaY);

                    arrow->body->rotationZ = ADrawable->GetWorldRotationZ(arrowDrawable);
                    arrow->body->velocityX = AMath_Cos(arrow->body->rotationZ) * strength;
                    arrow->body->velocityY = AMath_Sin(arrow->body->rotationZ) * strength;
                    arrow->body->positionX = arrowDrawable->positionX;
                    arrow->body->positionY = arrowDrawable->positionY;

                    APhysicsBody_ClearState       (arrow->body, PhysicsBodyState_IsFreeze);
                    APhysicsBody_SetCollisionGroup(arrow->body, CollisionGroup_HeroAttack | CollisionGroup_Attack);

                    if (strength > 1.6f)
                    {
                        AAudioTool->Play(AudioId_ShootFast);
                    }
                    else
                    {
                        AAudioTool->Play(AudioId_Shoot);
                    }
                }
                else
                {
                    AAudioTool->Play(AudioId_NoArrow);
                }

                if (touchDownId == 0)
                {
                    touchDownId = -1;
                }
                else
                {
                    // means first touch still down, then get first touch
                    touch       = AInput->GetTouch(0);
                    touchDownId = 0;
                    touchDownX  = touch->x;
                    touchDownY  = touch->y;
                }
            }

            break;
        }
    }

    return false;
}


static inline bool OnKeepMessage(Component* component, void* sender, int subject, void* extraData)
{
    return OnReadyMessage(component, sender, subject, extraData);
}


static inline bool OnDizzyMessage(Component* component, void* sender, int subject, void* extraData)
{
    InputTouch* touch = AArray_Get((Array*) extraData, 0, InputTouch*);

    switch (touch->type)
    {
        case InputTouchType_Down:
            touchDownX  = touch->x;
            touchDownY  = touch->y;
            touchDownId = touch->fingerId;
            break;

        case InputTouchType_Up:
            touchDownId = -1;
            break;
            
        case InputTouchType_Move:
            break;
            
        case InputTouchType_Cancel:
            break;
    }
    
    return false;
}


static bool isDizzyLocked = false;


static inline bool OnStandMessage(Component* component, void* sender, int subject, void* extraData)
{
    InputTouch* touch = AArray_Get((Array*) extraData, 0, InputTouch*);

    switch (touch->type)
    {
        case InputTouchType_Down:
        {
            Drawable* heroDrawable = AHero_GetDrawable();

            if
            (
                touch->y < AGameMap->moveAreaY          ||
                touch->x < -AGLTool->screenRatio + 0.5f ||
                touch->x >  AGLTool->screenRatio - 0.5f
            )
            {
                ADrawable_SetScaleX(heroDrawable, touch->x > 0 ? 1.0f : -1.0f);
                AComponent->SetState(AHero->component, HeroState_Walk);
            }
            else
            {
                if
                (
                    (heroDrawable->scaleX > 0 && touch->x < heroDrawable->positionX) ||
                        (heroDrawable->scaleX < 0 && touch->x > heroDrawable->positionX)
                )
                {
                    AComponent->SetState(AHero->component, HeroState_Ready);
                }
            }

            touchDownX  = touch->x;
            touchDownY  = touch->y;
            touchDownId = touch->fingerId;

            break;
        }


        case InputTouchType_Move:
        {
            if (touch->fingerId == touchDownId)
            {
                if (isDizzyLocked == false)
                {
                    Drawable* heroDrawable = AHero_GetDrawable();
                    float     deltaX       = fabsf(touchDownX - touch->x);
                    float     deltaY       = touchDownY - touch->y;

                    if (deltaY > 0.3f && deltaX < 0.15f)
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
                    else if (deltaY < 0.15f && deltaX > 0.15f)
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
            if (touch->fingerId == touchDownId)
            {
                // block dizzy continues trigger
                isDizzyLocked = false;
                touchDownId   = -1;
            }

            break;
        }
            
        case InputTouchType_Cancel:
            break;
    }

    return false;
}


static float  walkSpeed;
static bool   isWalkStart;


static inline bool OnWalkMessage(Component* component, void* sender, int subject, void* extraData)
{
    InputTouch* touch = AArray_Get((Array*) extraData, 0, InputTouch*);

    switch (touch->type)
    {
        case InputTouchType_Up:
            if (touch->fingerId == touchDownId)
            {
                isDizzyLocked = false;
                isWalkStart   = false;
                walkSpeed     = AGameData->speed - 0.01f;
                touchDownId   = -1;
            }
            break;

        default:
            OnStandMessage(component, sender, subject, extraData);
            break;
    }

    return false;
}


static void WalkUpdate(Component* component, float deltaSeconds)
{ 
    Update(component, deltaSeconds);
    
    if (walkSpeed != AGameData->speed) 
    {
        if (isWalkStart) 
        { 
            if (walkSpeed < AGameData->speed) 
            { 
                walkSpeed += 0.2f; 
            } 
            else 
            { 
                walkSpeed = AGameData->speed; 
            }
        } 
        else 
        { 
            if (walkSpeed > 0) 
            { 
                walkSpeed -= 0.2f; 
            } 
            else 
            { 
                AComponent->SetState(AHero->component, HeroState_Stand); 
            }
        }
    }
    
    float     speed        = AGLTool_ToGLWidth(walkSpeed);
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
            
            ADrawable_SetPositionX(AGameMap->beforeDrawable,  AGameMap->beforeDrawable->positionX  - speed);
            ADrawable_SetPositionX(AGameMap->middleDrawable,  AGameMap->middleDrawable->positionX  - middleSpeed);
            ADrawable_SetPositionX(AGameMap->backDrawable,    AGameMap->backDrawable->positionX    - backSpeed); 
        } 
        else if (heroDrawable->positionX < AGameMap->groundPosData[4]) 
        { 
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX + speed); 
        }
    } 
    else if (touchDownX < 0.0f) 
    { 
        if (heroDrawable->positionX > 0) 
        { 
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX - speed); 
        } 
        else if (AGameMap->minX < AGameMap->groundPosData[2]) 
        {
            AGameMap->minX += speed;
            AGameMap->maxX += speed;
            
            ADrawable_SetPositionX(AGameMap->beforeDrawable,  AGameMap->beforeDrawable->positionX  + speed);
            ADrawable_SetPositionX(AGameMap->middleDrawable,  AGameMap->middleDrawable->positionX  + middleSpeed);
            ADrawable_SetPositionX(AGameMap->backDrawable,    AGameMap->backDrawable->positionX    + backSpeed); 
        } 
        else if (heroDrawable->positionX > AGameMap->groundPosData[2]) 
        { 
            ADrawable_SetPositionX(heroDrawable, heroDrawable->positionX - speed); 
        }
    }
}


static void OnActionOver(SkeletonAnimationPlayer* player)
{ 
    switch (AHero->component->curState->id) 
    { 
        case HeroState_Shoot:
            if (touchDownId != -1)
            {
                AComponent->SetState(AHero->component, HeroState_Walk);
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            break;
        
        case HeroState_Hurt:
            if (touchDownId != -1)
            {
                AComponent->SetState(AHero->component, HeroState_Walk);
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }
            break;
        
        case HeroState_Dizzy: 
        {
            if (touchDownId != -1)
            {
                // if touch down when dizzy animation
                // continue the touch down effect

                Drawable* heroDrawable = AHero_GetDrawable();

                if
                (
                    touchDownY <  AGameMap->moveAreaY         ||
                    touchDownX < -AGLTool->screenRatio + 0.5f ||
                    touchDownX >  AGLTool->screenRatio - 0.5f
                )
                {
                    ADrawable_SetScaleX(heroDrawable, touchDownX > 0 ? 1.0f : -1.0f);
                    AComponent->SetState(AHero->component, HeroState_Walk);
                }
                else
                {
                    if
                    (
                        (heroDrawable->scaleX > 0 && touchDownX < heroDrawable->positionX) ||
                        (heroDrawable->scaleX < 0 && touchDownX > heroDrawable->positionX)
                    )
                    {
                        AComponent->SetState(AHero->component, HeroState_Ready);
                    }
                }
            }
            else
            {
                AComponent->SetState(AHero->component, HeroState_Stand);
            }

            if (AHUD->CostPower(AGameData->hitFloorCostEnergy))
            {
                ATweenTool->AddMoveY      (0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->AddMoveY      (-0.01f, 0.2f)
                          ->SetEaseType   (TweenEaseType_BounceOut)

                          ->RunActions    (AHero_GetDrawable());


                ATweenTool->AddMoveY      (-0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->AddMoveY      (0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_SineInOut)
                          ->RunActions    (AGameMap->beforeDrawable);


                ATweenTool->AddInterval   (0.04f)

                          ->AddMoveY      (-0.01f, 0.15f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->AddMoveY      (0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->RunActions    (AGameMap->middleDrawable);


                ATweenTool->AddInterval   (0.08f)

                          ->AddMoveY      (-0.01f, 0.2f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->AddMoveY      (0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_BounceOut)

                          ->RunActions    (AGameMap->backDrawable);


                ATweenTool->AddInterval   (0.1f)

                          ->AddMoveY      (-0.01f, 0.25f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->AddMoveY      (0.01f, 0.1f)
                          ->SetEaseType   (TweenEaseType_SineInOut)

                          ->RunActions    (AGameMap->skyDrawable);


                ATweenTool->AddMoveY      (0.0015f, 0.2f)
                          ->SetEaseType   (TweenEaseType_SineOut)

                          ->AddMoveY      (-0.0015f, 0.2f)
                          ->SetEaseType   (TweenEaseType_SineIn)

                          ->RunActions    (AHUD->upDrawable);


                ATweenTool->AddMoveY      (-0.002f, 0.2f)
                          ->SetEaseType   (TweenEaseType_SineOut)

                          ->AddMoveY      (0.002f, 0.2f)
                          ->SetEaseType   (TweenEaseType_SineIn)

                          ->RunActions    (AHUD->downDrawable);

                ADrawable_SetPositionX
                (
                    ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor),
                    ADrawable->ConvertBetweenLocalX
                    (
                        AGameMap->groundPosDrawable,
                        AHero_GetDrawable()->positionX,
                        AGameMap->beforeDrawable
                    )
                );

                AHero->hitFloor->loop = 1;
                AComponent->Notify(AHero->component, HeroNotify_Dizzy, NULL);
            }
            else
            {
            }

            AAudioTool->Play(AudioId_FallDown);

            break; 
        }
        
        case HeroState_Ready:
            AComponent->SetState(AHero->component, HeroState_Keep);
            break;
        
        case HeroState_Die:
            AComponent->SetState(AHero->component, HeroState_DieOver);
            AComponent->Notify  (AHero->component, HeroNotify_Die, NULL);
            break; 
    }
}


static void CurtainPlayerCallBack(SkeletonAnimationPlayer* player)
{
    while (arrowSet->elementList->size > 0)
    {
        Arrow* arrow       = AArrayList_Get(arrowSet->elementList, arrowSet->elementList->size - 1, Arrow*);
        bool   isHasAction = ATween->TryCompleteAllActions
                             (
                                arrow->sprite->drawable,
                                true
                             );

        // arrow not collision yet
        if (isHasAction == false)
        {
            APhysicsBody_SetState(arrow->body, PhysicsBodyState_IsFreeze);

            // assuming hit ground
            arrow->hitType = ArrowHitType_Ground;
            AHero->roundMissCount++;
            AHero->roundArrowCount++;

            ArrowOnComplete(arrow);
        }
    }

    AHUD->ClearDrop();
    AUI->ShowFail  ();
}


static void OpenCurtain(Scheduler* scheduler, float deltaSeconds)
{
    AAudioTool->Play(AudioId_Die);
    AHUD->OpenCurtain(CurtainPlayerCallBack);
}


static void TombstoneActionComplete(TweenAction* action)
{
    ATweenTool->AddFadeTo   (0.0f, 1.0f)
              ->SetRelative (false)
              ->RunActions  (AHero_GetDrawable());

    AHero->hitFloor->loop = 1;

    ADrawable_SetPositionX
    (
        ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor),
        ADrawable->ConvertBetweenLocalX
        (
            AGameMap          ->groundPosDrawable,
            AHero_GetDrawable()->positionX,
            AGameMap          ->beforeDrawable
        )
    );
    
    AEnemyAI  ->SetAllEnemy (EnemyState_Stand, 0.01f);
    AScheduler->ScheduleOnce(OpenCurtain, 1.0f);

    AAudioTool->Play(AudioId_FallDown);
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
    if (other->userId >= CollisionBodyId_Heart && other->userId <= CollisionBodyId_Wood)
    {
        AHUD->FlyItem((DropCollisionItem*) other->userData->slot0->ptrValue);
        return;
    }
    
    int id = AHero->component->curState->id;
    
    if (id < HeroState_Hurt && id != HeroState_Dizzy) 
    { 
        Enemy* enemy = (Enemy*) other->userData->slot0->ptrValue;
        
        if (enemy->component->curState->id == EnemyState_Dizzy)
        {
            return;
        }
        
        float  move;
        if (AEnemy_GetDrawable(enemy)->scaleX > 0.0f) 
        { 
            move = -0.08f; 
        } 
        else 
        { 
            move =  0.08f; 
        }

        AHero->roundHP--;
        AFont->SetInt(AHUD->heartText, AHero->roundHP);
        
        if (AHero->roundHP > 0) 
        { 
            AComponent->SetState(AHero->component, HeroState_Hurt);
            
            ATweenTool->AddMoveX      (move, 0.3f)
                      ->SetEaseType   (TweenEaseType_SineOut)
                      ->SetQueue      (false)

                      ->AddMoveY      (0.015f,  0.1f)
                      ->SetEaseType   (TweenEaseType_SineOut)

                      ->AddMoveY      (-0.015f, 0.1f)
                      ->SetEaseType   (TweenEaseType_SineOut)

                      ->RunActions    (AHero_GetDrawable());
        }
        else
        {
            AComponent->SetState(AHero->component, HeroState_Die);

            ATweenTool->AddMoveX      (move * 2, 1.25f)
                      ->SetEaseType   (TweenEaseType_SineOut)
                      ->SetQueue      (false)
                      ->SetOnComplete (DieActionComplete)

                      ->AddMoveY      (0.02f,  0.15f)
                      ->SetEaseType   (TweenEaseType_SineOut)

                      ->AddMoveY      (-0.02f,  0.15f)
                      ->SetEaseType   (TweenEaseType_SineOut)

                      ->RunActions    (AHero_GetDrawable());
        }

        Arrow* arrow = (Arrow*) AHero->component->userData->slot0->ptrValue;
        if (arrow != NULL)
        {
            OnReadyMessage
            (
                AHero->component, NULL, 0,
                AArray_Make
                (
                    InputTouch*, 1,
                    (InputTouch[])
                    {
                        readyX,
                        readyY,
                        0,
                        InputTouchType_Up
                    }
                )
            );
        }
    }
}


static char* weaponNames[] =
{
    "Copper",
    "Dragon",
    "Gold",
    "Heart",
    "Ice",
    "Iron",
    "Lightning",
    "Silver",
    "Steel",
    "Wood",
};


static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (sender == AApplication)
    {
        switch (component->curState->id)
        {
            case HeroState_Stand:
                OnStandMessage(component, sender, subject, extraData);
                break;

            case HeroState_Walk:
                OnWalkMessage(component, sender, subject, extraData);
                break;

            case HeroState_Ready:
                OnReadyMessage(component, sender, subject, extraData);
                break;

            case HeroState_Keep:
                OnKeepMessage(component, sender, subject, extraData);
                break;

            case HeroState_Shoot:
                OnStandMessage(component, sender, subject, extraData);
                break;

            case HeroState_Dizzy:
                OnDizzyMessage(component, sender, subject, extraData);
                break;
        }
    }
    else if (sender == AComponent)
    {
        switch (subject)
        {
            case ComponentMsg_OnEnter:
            {
                switch (component->curState->id)
                {
                    case HeroState_Stand:
                        if (component->preState->id == HeroState_Walk)
                        {
                            ASkeletonAnimationPlayer->SetAnimationMix(AHero->player, "stand", 0.2f);
                        }
                        else
                        {
                            ASkeletonAnimationPlayer->SetAnimation(AHero->player, "stand");
                        }
                        break;

                    case HeroState_Walk:
                        walkSpeed   = 0.0f;
                        isWalkStart = true;
                        ASkeletonAnimationPlayer->SetAnimationMix(AHero->player, "walk", 0.1f);
                        break;

                    case HeroState_Shoot:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "shoot");
                        break;

                    case HeroState_Ready:
                    {
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "ready");

                        if (AHero->roundEnergy >= AGameData->arrowCostEnergy)
                        {
                            Arrow* arrow     = AArrayList_Pop(arrowList, Arrow*);
                            char*  arrowName = weaponNames[AMath_RandomInt(0, 9)];
                            char   buff[25];
                            sprintf(buff, "Texture/%s.png", arrowName);

                            if (arrow == NULL)
                            {
                                arrow          = (Arrow*) malloc(sizeof(Arrow));
                                arrow->body    = NULL;
                                arrow->enemy   = NULL;

                                ASprite->InitWithFile(buff, arrow->sprite);

                                arrow->body = APhysicsWorld->AddBody
                                (
                                    PhysicsShape_Line,
                                    AArray_Make
                                    (
                                        float,
                                        4,
                                        0.0f,
                                        0.0f,
                                        AGLTool_ToGLWidth(25),
                                        0.0f
                                    )
                                );

                                arrow->body->userData->slot0->ptrValue = arrow;
                                arrow->body->OnCollision               = OnHit;
                            }
                            else
                            {
                                arrow->sprite->texture = ATexture->Get(buff);
                                ADrawable_SetVisible(arrow->sprite->drawable);
                                ADrawable_SetScale2 (arrow->sprite->drawable, 1.0f, 1.0f);
                            }

                            ATool->InitBox(arrow->body, 2.0f, -2.0f);
                            APhysicsBody_SetState (arrow->body,             PhysicsBodyState_IsFreeze);
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

                            arrow->hitType = ArrowHitType_Null;
                            arrow->enemy   = NULL;


                            #ifdef APP_DEBUG
                            ADrawable->Init(arrow->debugDrawable);
                            arrow->debugDrawable->userData->slot0->ptrValue = arrow;
                            arrow->debugDrawable->Render                    = DebugRender;
                            #endif


                            component->userData->slot0->ptrValue = arrow;
                            AArrayIntSet->TryAdd(arrowSet, (intptr_t) arrow);

                            // change hero skin
                            sprintf(buff, "RedGirl%s", arrowName);
                            ASkeleton->SetSkin(AHero->player->skeleton, buff);
                        }

                        break;
                    }

                    case HeroState_Keep:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "keep");
                        break;

                    case HeroState_Hurt:
                        AAudioTool->Play(AudioId_PickHero);
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "hurt");
                        break;

                    case HeroState_Dizzy:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "dizzy");
                        break;

                    case HeroState_Die:
                        AAudioTool->Play(AudioId_HeroDie);
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "die");
                        break;

                    case HeroState_DieOver:
                        ASkeletonAnimationPlayer->SetAnimation(AHero->player, "die_over");
                        break;
                }

                break;
            }

            case ComponentMsg_OnExit:
            {
                switch (component->curState->id)
                {
                    case EnemyState_Walk:
                        break;
                }

                break;
            }
        }
    }

    return false;
}


static inline void InitProperty()
{
    AHero->roundHP         = AGameData->maxHP;
    AHero->roundEnergy     = AGameData->maxEnergy;

    AHero->roundKillCount  = 0;
    AHero->roundArrowCount = 0;
    AHero->roundMissCount  = 0;
    AHero->roundTime       = 0.0;

    ASkeleton->SetSkin(AHero->player->skeleton, "RedGirlWood");
}


static void Init()
{
    AComponent->Init(AHero->component);
    ASkeletonAnimationPlayer->Init("Animation/redgirl", "stand", AHero->player);

    Drawable* heroDrawable = AHero_GetDrawable();
    ADrawable_SetParent   (heroDrawable, AGameMap->beforeDrawable);
    ADrawable_SetPositionY(heroDrawable, AGameMap->groundY);

//----------------------------------------------------------------------------------------------------------------------

    SkeletonSlot* collisionSlot = ASkeletonAnimationPlayer_GetSlot(AHero->player, "CollisionBox");
    if (collisionSlot != NULL)
    {
        AHero->collisionBoxDrawable          = collisionSlot->bone->drawable;
        AHero->collisionBoxBody              = APhysicsWorld->AddBody(PhysicsShape_Polygon, ASkeletonSlot_GetBoundingBox(collisionSlot)->vertexArr);
        AHero->collisionBoxBody->OnCollision = OnHurt;
        APhysicsBody_SetCollisionGroup(AHero->collisionBoxBody, CollisionGroup_HeroBody | CollisionGroup_HeroAttack | CollisionGroup_EnemyBody);
        APhysicsBody_SetState         (AHero->collisionBoxBody, PhysicsBodyState_IsFixed);


        #ifdef APP_DEBUG
        ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable(AHero->player, "CollisionBox", debugCollisionBoundingBox);
        #endif
    }

    AHero->bowHandDrawable      = ASkeletonAnimationPlayer_GetBone(AHero->player, "righthand-up")->drawable;
    AHero->player->OnActionOver = OnActionOver;

    ASkeletonAnimationPlayer->Init("Animation/hit-floor", "animation", AHero->hitFloor);
    AHero->hitFloor->loop       = 0;

    Drawable* hitFloorDrawable = ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor);
    ADrawable_SetParent   (hitFloorDrawable, AGameMap->beforeDrawable);
    ADrawable_SetPositionY(hitFloorDrawable, AGameMap->groundY);

//----------------------------------------------------------------------------------------------------------------------

    AHero->component->stateMap->elementList->increase = 10;

    AComponent->AddState(AHero->component, HeroState_Stand,    OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Walk,     OnMessage, WalkUpdate);
    AComponent->AddState(AHero->component, HeroState_Shoot,    OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Ready,    OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Keep,     OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Hurt,     OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Dizzy,    OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_Die,      OnMessage, Update);
    AComponent->AddState(AHero->component, HeroState_DieOver, OnMessage, Update);

    AComponent->SetState(AHero->component, HeroState_Stand);
}


static void Revive()
{
    InitProperty();

    ADrawable_SetParent   (ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor), AGameMap->beforeDrawable);
    ADrawable_SetPositionY(ASkeletonAnimationPlayer_GetDrawable(AHero->hitFloor), AGameMap->groundY);

    ADrawable_SetParent   (AHero_GetDrawable(),       AGameMap->beforeDrawable);
    ADrawable_SetPositionY(AHero_GetDrawable(),       AGameMap->groundY);
    ADrawable_SetPositionX(AHero_GetDrawable(),       0.0f);
    ADrawable_SetOpacity  (AHero_GetDrawable(),       1.0f);

    ADrawable_SetPositionX(AGameMap->beforeDrawable, 0.0f);
    ADrawable_SetPositionX(AGameMap->middleDrawable, 0.0f);
    ADrawable_SetPositionX(AGameMap->backDrawable,   0.0f);

    ADrawable_SetInVisible(AHUD->tombstoneDrawable);
    ADrawable_SetPositionY(AHUD->tombstoneDrawable, AHUD->tombstoneBornY);

    AGameMap->minX = AGameMap->groundPosData[0];
    AGameMap->maxX = AGameMap->groundPosData[6];

    AComponent->SetState(AHero->component, HeroState_Stand);
}


static void Run()
{
    AApplication_AddChild(AHero->component, 60);
    AArrayIntSet->Clear(AHero->component->observerSet);

    InitProperty();
}


struct AHero AHero[1] =
{
    {
        .Init   = Init,
        .Run    = Run,
        .Revive = Revive,
    }
};
