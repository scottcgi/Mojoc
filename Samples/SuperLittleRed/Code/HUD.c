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
 * Since    : 2016-12-2
 * Update   : 2019-2-18
 * Author   : scott.cgi
 */


#include <stdio.h>
#include <string.h>

#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Extension/Font.h"
#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Application/Application.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Application/Platform/Vibrator.h"

#include "HUD.h"
#include "GameMap.h"
#include "Hero.h"
#include "GameActor.h"
#include "Tool.h"
#include "UI.h"
#include "GameData.h"
#include "EnemyAI.h"
#include "AudioTool.h"


static ArrayList  (DropCollisionItem*) collisionItemList[1] = AArrayList_Init  (DropCollisionItem*, 10);
static ArrayIntSet(drawable*)          dropSet          [1] = AArrayIntSet_Init(Drawable*,          10);

#ifdef APP_DEBUG
static ArrayIntSet(DropCollisionItem*) collisionItemDebugDrawSet[1] = AArrayIntSet_Init(DropCollisionItem*, 10);
#endif

static bool       isCostEnergy  = false;
static float      recoverTime   = 0.0f;
static FontText*  roundText     = NULL;
static FontText*  roundNumText  = NULL;
static FontText*  timeText      = NULL;
static DrawAtlas* dropDrawAtlas = NULL;
static Scheduler* timeScheduler = NULL;


static void Update(Component* component, float deltaSeconds)
{
    ADrawAtlas_Draw(dropDrawAtlas);
    ASkeletonAnimationPlayer_Update(AHUD->hudPlayer,     deltaSeconds);
    ASkeletonAnimationPlayer_Update(AHUD->curtainPlayer, deltaSeconds);

    #ifdef APP_DEBUG
    for (int i = 0; i < collisionItemDebugDrawSet->elementList->size; ++i)
    {
        DropCollisionItem* item = AArrayList_Get(collisionItemDebugDrawSet->elementList, i, DropCollisionItem*);

        if (item->body->state != PhysicsBodyState_Freeze)
        {
            ADrawable->Draw(item->debugDrawable);
        }
    }
    #endif

    if (isCostEnergy == false)
    {
        if (AHero->roundEnergy < AGameData->maxEnergy)
        {
            if (recoverTime > heroEnergyRecoverTime)
            {
                AHero->roundEnergy += AGameData->recoverEnergy;

                if (AHero->roundEnergy > AGameData->maxEnergy)
                {
                    AHero->roundEnergy = AGameData->maxEnergy;
                }

                float scaleY = (float) AHero->roundEnergy / (float) AGameData->maxEnergy;

                ATweenTool->AddScaleY  (scaleY, 0.35f)
                          ->SetRelative(false)
                          ->SetEaseType(TweenEaseType_SineInOut)
                          ->RunActions (AHUD->energyLengthDrawable);

                if (scaleY != 0.0f)
                {
                    ATweenTool->AddScaleY  (1.0f / scaleY, 0.35f)
                              ->SetRelative(false)
                              ->SetEaseType(TweenEaseType_SineInOut)
                              ->RunActions (AHUD->energyPointerDrawable);
                }

                recoverTime = 0.0f;
            }
            else
            {
                recoverTime += deltaSeconds;
            }
        }
    }
}


static void ScoreSchedulerUpdate(Scheduler* scheduler, float deltaSeconds)
{
    char buff[10];
    ATool->SetTimeToBuff(buff,     (int) AHero->roundTime);
    AFont->SetString    (timeText, buff);
}


static void OnHUDActionOver(SkeletonAnimationPlayer* player)
{
    ASkeletonAnimationPlayer->SetAnimation(player, "animation");
    player->OnActionOver = NULL;
    timeScheduler        = AScheduler->Schedule(ScoreSchedulerUpdate, 0.9f);

    ATweenTool->AddFadeTo  (1.0f, 1.0f)
              ->SetRelative(false)
              ->SetEaseType(TweenEaseType_ExponentialIn)
              ->RunActions (timeText->drawable);
}


static void HUDSchedulerUpdate(Scheduler* scheduler, float deltaSeconds)
{
    AHUD->Reset();
}


static SkeletonAnimationPlayerOnActionOver OnCurtainCallback;


static void OnCurtainActionOver(SkeletonAnimationPlayer* player)
{
    if (strcmp(player->curAnimationData->name, "close") == 0)
    {
        player->loop = 0;

        // play show animation
        ASkeletonAnimationPlayer->SetAnimation(AHUD->hudPlayer, "ShowIn");
        AHUD->hudPlayer->OnActionOver = OnHUDActionOver;
        AHUD->hudPlayer->loop         = -1;

        AScheduler->ScheduleOnce(HUDSchedulerUpdate, 0.1f);
    }
    else if (strcmp(player->curAnimationData->name, "open") == 0)
    {
        ASkeletonAnimationPlayer->SetAnimation(player, "stop");

        if (timeScheduler != NULL)
        {
            timeScheduler->isCancel = true;
            ADrawable_SetOpacity(timeText->drawable, 0.0f);
        }
    }

    player->OnActionOver = NULL;

    if (OnCurtainCallback != NULL)
    {
        OnCurtainCallback(player);
    }
}


static void OnInitActionOver(SkeletonAnimationPlayer* player)
{
    OnCurtainActionOver(player);
    AUI->Run();
    AUI->ShowMenu();
}


enum
{
    HUD_TextCount = 5,
};


static void Init()
{
    AComponent->Init(AHUD->component);
    AHUD->component->curState->Update = Update;

    ASkeletonAnimationPlayer->Init("UI/HUD", "ShowIn", AHUD->hudPlayer);
    AHUD->hudPlayer->OnActionOver = OnHUDActionOver;
    AHUD->hudPlayer->loop         = 0;

    ADrawable_SetScale2
    (
        ASkeletonAnimationPlayer_GetDrawable(AHUD->hudPlayer),
        ATool->globalScaleX,
        ATool->globalScaleY
    );

    ASkeletonAnimationPlayer->Init("UI/Curtain", "open", AHUD->curtainPlayer);
    AHUD->curtainPlayer->loop = 0;

    ADrawable_SetScale2
    (
        ASkeletonAnimationPlayer_GetDrawable(AHUD->curtainPlayer),
        ATool->globalScaleX,
        ATool->globalScaleY
    );

    AHUD->energyLengthDrawable  = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "EnergyLength")->drawable;
    AHUD->energyDrawable        = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "Energy")      ->drawable;
    AHUD->tombstoneDrawable     = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "Tombstone")   ->drawable;
    AHUD->tombstoneHeight       = ASkeleton_GetSubMesh
                                  (
                                      AHUD->hudPlayer->skeleton,
                                      "Tombstone",
                                      "Tombstone"
                                  )
                                  ->drawable->height;

    AHUD->tombstoneBornY        = AHUD->tombstoneDrawable->positionY;
    AHUD->upDrawable            = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "Up")  ->drawable;
    AHUD->downDrawable          = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "Down")->drawable;

    ADrawable_SetInvisible(AHUD->tombstoneDrawable);

    AHUD->heartDrawable         = ASkeletonAnimationPlayer_GetBone   (AHUD->hudPlayer, "Heart")->drawable;
    AHUD->coinDrawable          = ASkeletonAnimationPlayer_GetBone   (AHUD->hudPlayer, "Coin") ->drawable;
    AHUD->stoneDrawable         = ASkeletonAnimationPlayer_GetBone   (AHUD->hudPlayer, "Stone")->drawable;
    AHUD->woodDrawable          = ASkeletonAnimationPlayer_GetBone   (AHUD->hudPlayer, "Wood") ->drawable;
    AHUD->killDrawable          = ASkeletonAnimationPlayer_GetBone   (AHUD->hudPlayer, "Kill") ->drawable;
    AHUD->energyPointerDrawable = ASkeletonAnimationPlayer_GetSubMesh(AHUD->hudPlayer, "Pointer", "Pointer")->drawable;

    roundText = AFont->GetText(AGameActor->talkFont);

    if (ATool->languageCode == LanguageCode_zh)
    {
        AFont->SetString(roundText, "@");
        ADrawable_SetPositionY(roundText->drawable, 0.204f);
    }
    else
    {
        AFont->SetString(roundText, "Round");
        ADrawable_SetPositionY(roundText->drawable, 0.2f);
    }

    const float scaleX = ATool->globalScaleX / 1.5f;
    const float scaleY = ATool->globalScaleX / 1.5f;

    ADrawable_SetScale2   (roundText->drawable, scaleX, scaleY);
    ADrawable_SetOpacity  (roundText->drawable, 0.0f);
    ADrawable_SetPositionX(roundText->drawable, -roundText->drawable->width * scaleX / 2);

    roundNumText = AFont->GetText(AGameActor->talkFont);
    ADrawable_SetScale2    (roundNumText->drawable, scaleX / 1.2f, scaleY / 1.2f);
    ADrawable_SetPositionY (roundNumText->drawable, 0.2018f);
    ADrawable_SetPositionX (roundNumText->drawable, -roundText->drawable->positionX);
    ADrawable_SetOpacity   (roundNumText->drawable, 0.0f);

    timeText = AFont->GetText(AGameActor->talkFont);
    ADrawable_SetOpacity(timeText->drawable, 0.0f);
    ADrawable_SetParent
    (
        timeText->drawable,
        ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "ScorePos")->drawable
    );

    Drawable* items[HUD_TextCount] =
    {
        AHUD->heartDrawable,
        AHUD->coinDrawable,
        AHUD->killDrawable,
        AHUD->stoneDrawable,
        AHUD->woodDrawable,
    };

    FontText** textPtrs[HUD_TextCount] =
    {
        &AHUD->heartText,
        &AHUD->coinText,
        &AHUD->killText,
        &AHUD->stoneText,
        &AHUD->woodText,
    };

    for (int i = 0; i < HUD_TextCount; ++i)
    {
        FontText* text  = AFont->GetText(AGameActor->talkFont);
        text->alignment = FontTextAlignment_HorizontalRight;

        ADrawable_SetParent
        (
            text->drawable,
            items[i]
        );
        
        ADrawable_SetPositionX(text->drawable, -AGLTool_ToGLWidth(20.0f));
        *textPtrs[i] = text;
    }

    dropDrawAtlas = ADrawAtlas->Get("Texture/Drop.atlas");
}


static void CostEnergyActionComplete(TweenAction* action)
{
    isCostEnergy = false;
}


static bool CostPower(int cost)
{
    if (AHero->roundEnergy >= cost)
    {
        AHero->roundEnergy -= cost;
        isCostEnergy        = true;

        if (AHero->roundEnergy > AGameData->maxEnergy)
        {
            AHero->roundEnergy = AGameData->maxEnergy;
        }
        
        float scaleY = (float) AHero->roundEnergy / (float) AGameData->maxEnergy;

        ATweenTool->AddScaleY    (scaleY, 0.2f)
                  ->SetRelative  (false)
                  ->SetEaseType  (TweenEaseType_SineInOut)
                  ->SetOnComplete(CostEnergyActionComplete)
                  ->RunActions   (AHUD->energyLengthDrawable);

        if (scaleY != 0.0f)
        {
            ATweenTool->AddScaleY(1.0f / scaleY, 0.2f)
                      ->SetRelative(false)
                      ->SetEaseType(TweenEaseType_SineInOut)
                      ->RunActions(AHUD->energyPointerDrawable);
        }

        return true;
    }
    else
    {
        ATool->ShakeX(AHUD->energyDrawable);

        FontText* text = AFont->GetText(AGameActor->talkFont);

        if (ATool->languageCode == LanguageCode_zh)
        {
            AFont->SetString(text, "!");
            ADrawable_SetScaleSame2(text->drawable, 1.2f);
        }
        else
        {
            AFont->SetString(text, "Weak");
        }

        AFont_SetTextColor(text, AColor_Make(0.81f, 0.78f, 0.75f,1.0f));
        ATool->FlyTextOnHero(text, 0.1f, 4.0f);
        
        return false;
    }
}


static void OpenCurtain(SkeletonAnimationPlayerOnActionOver callback)
{
    ASkeletonAnimationPlayer->SetAnimation(AHUD->curtainPlayer, "open");
    AHUD->curtainPlayer->loop         = -1;
    AHUD->curtainPlayer->OnActionOver = OnCurtainActionOver;

    AAudioTool->Play(AudioID_CurtainSlideShow);
    AAudioTool->StartUIBG();

    if (callback == NULL)
    {
        OnCurtainCallback = NULL;
    }
    else
    {
        OnCurtainCallback = callback;
    }
}


static void CloseCurtain(SkeletonAnimationPlayerOnActionOver callback)
{
    ASkeletonAnimationPlayer->SetAnimation(AHUD->curtainPlayer, "close");
    AHUD->curtainPlayer->loop         = -1;
    AHUD->curtainPlayer->OnActionOver = OnCurtainActionOver;

    AAudioTool->Play(AudioID_CurtainSlideHide);
    AAudioTool->Play(AudioID_CurtainRing);
    AAudioTool->StartGameBG();

    if (callback == NULL)
    {
        OnCurtainCallback = NULL;
    }
    else
    {
        OnCurtainCallback = callback;
    }

    ADrawable_SetParent    (roundText->drawable, AGameMap->beforeDrawable);

    ATweenTool->AddFadeTo  (1.0, 2.0f)
              ->SetEaseType(TweenEaseType_SineInOut)
              ->SetRelative(false)

              ->AddFadeTo  (0.0, 3.0f)
              ->SetEaseType(TweenEaseType_QuinticIn)
              ->SetRelative(false)

              ->RunActions (roundText->drawable);


    AFont->SetInt          (roundNumText, ++AHero->roundCount);
    ADrawable_SetParent    (roundNumText->drawable, AGameMap->beforeDrawable);

    ATweenTool->AddFadeTo  (1.0, 2.0f)
              ->SetEaseType(TweenEaseType_SineInOut)
              ->SetRelative(false)

              ->AddFadeTo  (0.0, 3.0f)
              ->SetEaseType(TweenEaseType_QuinticIn)
              ->SetRelative(false)

              ->RunActions (roundNumText->drawable);
}


static void Run()
{
    AApplication_AddChild(AHUD->component, 100);
    AHUD->curtainPlayer->OnActionOver = OnInitActionOver;
    AHUD->curtainPlayer->loop         = -1;

    AAudioTool->Play(AudioID_CurtainSlideShow);
}


static void Reset()
{
    AGameData->wood  = 0;
    AGameData->stone = 0;

    AFont->SetInt(AHUD->heartText, AHero->roundHP);
    AFont->SetInt(AHUD->coinText,  AGameData->coin);
    AFont->SetInt(AHUD->woodText,  AGameData->wood);
    AFont->SetInt(AHUD->stoneText, AGameData->stone);
    AFont->SetInt(AHUD->killText,  AHero->roundKillCount);

    ADrawable_SetScaleY(AHUD->energyLengthDrawable,  1.0f);
    ADrawable_SetScaleY(AHUD->energyPointerDrawable, 1.0f);

    AMath_RandomSeedByTime();
}


static void ClearDrop()
{
    while (dropSet->elementList->size > 0)
    {
        ATween->TryCompleteAllActions
        (
            AArrayList_Get(dropSet->elementList, dropSet->elementList->size - 1, Drawable*),
            true
        );
    }
}


static void OnStoneFade(TweenAction* action)
{
    SubMesh* subMesh = AUserData_GetSlotPtrWithType(action->userData, 0, SubMesh*);
    ADrawAtlas->ReleaseQuad(dropDrawAtlas, subMesh);
    AArrayIntSet->TryRemove(dropSet,       (intptr_t) subMesh->drawable);
}


static void OnStoneDown(TweenAction* action)
{
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

    AEnemyAI->SetAllEnemy(EnemyState_Stand, AGameData->stonePower);
    AAudioTool->Play(AudioID_FallDown);
}


static inline void DropStoneWood
(
    const char*           name,
    float                 fadeTime,
    TweenActionOnComplete OnDown,
    TweenActionOnComplete OnFade,
    DropCollisionItem*    item
)
{
    SubMesh*  subMesh  = ADrawAtlas->GetQuad(dropDrawAtlas, name);
    Drawable* drawable = subMesh->drawable;
    float     posX     = ADrawable->ConvertBetweenLocalPositionX
                         (
                            AGameMap->groundPosDrawable,
                            AHero_GetDrawable()->positionX,
                            AGameMap->beforeDrawable
                         );

    float     posY     = ADrawable->ConvertToLocalPositionY(AGameMap->beforeDrawable, 1.0f + drawable->height / 2);

    ADrawable_SetParent   (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2(drawable, posX, posY);

    ATweenTool->AddMoveY      (AGameMap->groundY + drawable->height / 2 - AGLTool_ToGLHeight(0.5f), 0.35f)
              ->SetEaseType   (TweenEaseType_ExponentialIn)
              ->SetRelative   (false)
              ->SetUserDataPtr(0, item == NULL ? (void*) drawable : item)
              ->SetOnComplete (OnDown)

              ->AddFadeTo     (0.0f, fadeTime)
              ->SetRelative   (false)
              ->SetUserDataPtr(0, item == NULL ? (void*) subMesh  : item)
              ->SetEaseType   (TweenEaseType_ExponentialIn)
              ->SetOnComplete (OnFade)

              ->RunActions    (drawable);

    AArrayIntSet->TryAdd(dropSet, (intptr_t) drawable);

    if (item != NULL)
    {
        item->subMesh = subMesh;
    }
}


static void DropStone()
{
    DropStoneWood("DropStone", 2.0f, OnStoneDown, OnStoneFade, NULL);
}


#ifdef APP_DEBUG
static void ItemDebugRender(Drawable *drawable)
{
    AGLPrimitive->RenderPolygon
    (
        AUserData_GetSlotPtrWithType(drawable->userData, 0, DropCollisionItem*)->body->transformedVertexArr,
        AGameMap->beforeDrawable->mvpMatrix,
        COLOR_GREED_ARRAY,
        1.0f
    );
}
#endif


static inline void ClearItem(DropCollisionItem* item)
{
    item->body->state = PhysicsBodyState_Freeze;
    ADrawAtlas->ReleaseQuad(dropDrawAtlas, item->subMesh);

    AArrayList_Add(collisionItemList, item);
    AArrayIntSet->TryRemove(dropSet, (intptr_t) item->subMesh->drawable);

    #ifdef APP_DEBUG
    AArrayIntSet->TryRemove(collisionItemDebugDrawSet, (intptr_t) item);
    #endif
}


static void OnItemFade(TweenAction* action)
{
    ClearItem(AUserData_GetSlotPtrWithType(action->userData, 0, DropCollisionItem*));
}


static inline void UpdateItemBody(DropCollisionItem* item)
{
    item->body->positionX = item->subMesh->drawable->positionX;
    item->body->positionY = item->subMesh->drawable->positionY;
    item->body->state     = PhysicsBodyState_Fixed;
    APhysicsBody->Update(item->body, 0.0f);
}


static void OnItemDown(TweenAction* action)
{
    UpdateItemBody(AUserData_GetSlotPtrWithType(action->userData, 0, DropCollisionItem*));
}


static inline DropCollisionItem* DropItem(int bodySize, int bodyID, int collisionGroup)
{
    DropCollisionItem* item = NULL;

    for (int i = 0; i < collisionItemList->size; ++i)
    {
        DropCollisionItem* tmp = AArrayList_Get(collisionItemList, i, DropCollisionItem*);

        if (tmp->bodySize == bodySize && tmp->bodyID == bodyID && tmp->collisionGroup == collisionGroup)
        {
            item = tmp;
            AArrayList->Remove(collisionItemList, i);
            break;
        }
    }

    if (item == NULL)
    {
        item                 = malloc(sizeof(DropCollisionItem));
        item->bodyID         = bodyID;
        item->bodySize       = bodySize;
        item->collisionGroup = collisionGroup;

        float len  = AGLTool_ToGLWidth(bodySize);
        item->body = APhysicsWorld->AddBody
                     (
                        PhysicsShape_Polygon,
                        AArray_Make
                        (
                             float,  8,
                            -len,  len,
                             len,  len,
                             len, -len,
                            -len, -len,
                        )
                     );

        item->body->userID = bodyID;
        AUserData_SetSlotPtr(item->body->userData, 0, item);
        APhysicsBody_SetCollisionGroup(item->body, collisionGroup);

        #ifdef APP_DEBUG
        ADrawable->Init(item->debugDrawable);
        AUserData_SetSlotPtr(item->debugDrawable->userData, 0, item);
        item->debugDrawable->Render = ItemDebugRender;
        #endif
    }

    item->body->state = PhysicsBodyState_Freeze;

    #ifdef APP_DEBUG
    AArrayIntSet->TryAdd(collisionItemDebugDrawSet, (intptr_t) item);
    #endif

    return item;
}


static void OnWoodDown(TweenAction* action)
{
    AAudioTool->Play(AudioID_FallDown);
    OnItemDown(action);

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
}


static void DropWood()
{
    DropStoneWood
    (
        "DropWood",
        AGameData->woodTime,
        OnWoodDown,
        OnItemFade,
        DropItem
        (
            18,
            CollisionBodyID_Wood,
            CollisionGroup_HeroAttack | CollisionGroup_ArrowAttack
        )
    );
}


static void DropCollisionItemSchedulerUpdate(Scheduler* scheduler, float deltaSeconds)
{
    float              time = AUserData_GetSlotFloat(scheduler->userData, 1);
    DropCollisionItem* item = AUserData_GetSlotPtrWithType(scheduler->userData, 0, DropCollisionItem*);

    if (time > 1.2f || item->body->state == PhysicsBodyState_Freeze)
    {
        scheduler->isCancel = true;
    }
    else
    {
        time += deltaSeconds;
        AUserData_SetSlotFloat(scheduler->userData, 1, time);
        UpdateItemBody(item);
    }
}


static void OnDropItemAfter(TweenAction* action)
{
    DropCollisionItem* item = AUserData_GetSlotPtrWithType(action->userData, 0, DropCollisionItem*);

    // update item body when item easing.
    Scheduler* scheduler = AScheduler->Schedule(DropCollisionItemSchedulerUpdate, 0.0f);

    AUserData_SetSlotPtr  (scheduler->userData, 0, item);
    AUserData_SetSlotFloat(scheduler->userData, 1, 0.0f);// time
    UpdateItemBody(item); // start collision test
}


static inline void DropHeartCoinStoneWood
(
    const char*        name,
    PhysicsBody*       arrowBody,
    float              enemyScaleX,
    DropCollisionItem* item
)
{
    SubMesh*  subMesh  = ADrawAtlas->GetQuad(dropDrawAtlas, name);
    Drawable* drawable = subMesh->drawable;

    ADrawable_SetParent    (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2 (drawable, arrowBody->positionX, arrowBody->positionY);
    ADrawable_SetScaleSame2(drawable, 0.2f);

    float scaleY = 0.7f;

    ATweenTool->AddMoveY      (AGameMap->groundY + drawable->height * scaleY / 2 - AGLTool_ToGLHeight(0.5f), 0.8f)
              ->SetEaseType   (TweenEaseType_BounceOut)
              ->SetRelative   (false)

              ->SetScaleSame2 (scaleY, false, TweenEaseType_QuarticOut)

              ->AddMoveX      (-0.11f * enemyScaleX * fabsf(arrowBody->velocityX), 1.2f)
              ->SetQueue      (false)
              ->SetEaseType   (TweenEaseType_CubicOut)

              ->AddFadeTo     (0.0, 15.0f)
              ->SetRelative   (false)
              ->SetEaseType   (TweenEaseType_ExponentialIn)
              ->SetUserDataPtr(0, item)
              ->SetOnComplete (OnItemFade)

              ->AddInterval   (0.5f)
              ->SetQueue      (false)
              ->SetUserDataPtr(0, item)
              ->SetOnComplete (OnDropItemAfter)

              ->RunActions    (drawable);

    AArrayIntSet->TryAdd(dropSet, (intptr_t) drawable);
    item->subMesh = subMesh;
}


static void DropPickHeart(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropHeartCoinStoneWood
    (
        "Heart",
        arrowBody,
        enemyScaleX,
        DropItem
        (
            9,
            CollisionBodyID_Heart,
            CollisionGroup_EnemyAttack | CollisionGroup_ArrowAttack
        )
    );
}


static void DropPickCoin(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropHeartCoinStoneWood
    (
        "Coin",
        arrowBody,
        enemyScaleX,
        DropItem
        (
            9,
            CollisionBodyID_Coin,
            CollisionGroup_EnemyAttack | CollisionGroup_ArrowAttack
        )
    );
}


static void DropPickStone(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropHeartCoinStoneWood
    (
        "Stone",
        arrowBody,
        enemyScaleX,
        DropItem
        (
            9,
            CollisionBodyID_Stone,
            CollisionGroup_EnemyAttack | CollisionGroup_ArrowAttack
        )
    );
}


static void DropPickWood(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropHeartCoinStoneWood
    (
        "Wood",
        arrowBody,
        enemyScaleX,
        DropItem
        (
            9,
            CollisionBodyID_Wood,
            CollisionGroup_EnemyAttack | CollisionGroup_ArrowAttack
        )
    );
}


static void OnItemGet(TweenAction* action)
{
    DropCollisionItem* item = AUserData_GetSlotPtrWithType(action->userData, 0, DropCollisionItem*);
    Drawable*          drawable;

    switch (item->bodyID)
    {
        case CollisionBodyID_Heart:
            AFont->SetInt(AHUD->heartText, ++AHero->roundHP);
            drawable = AHUD->heartDrawable;
            break;

        case CollisionBodyID_Coin:
            AFont->SetInt(AHUD->coinText,  ++AGameData->coin);
            drawable = AHUD->coinDrawable;
            break;

        case CollisionBodyID_Stone:
            AFont->SetInt(AHUD->stoneText, ++AGameData->stone);
            drawable = AHUD->stoneDrawable;
            break;

        case CollisionBodyID_Wood:
            AFont->SetInt(AHUD->woodText,  ++AGameData->wood);
            drawable = AHUD->woodDrawable;
            break;

        default:
            return;
    }

    ADrawAtlas->ReleaseQuad(dropDrawAtlas, AUserData_GetSlotPtrWithType(action->userData, 1, SubMesh*));

    ATweenTool->AddScaleSame2(1.25f, 0.1f, false, TweenEaseType_SineOut)
              ->AddScaleSame2(1.0f,  0.2f, false, TweenEaseType_ElasticOut)
              ->RunActions(drawable);
}


static void FlyItem(DropCollisionItem* item)
{
    const char* name;
    Drawable*   target;

    switch (item->bodyID)
    {
        case CollisionBodyID_Heart:
            name   = "Heart";
            target = AHUD->heartDrawable;
            AAudioTool->Play(AudioID_PickHeart);
            break;

        case CollisionBodyID_Coin:
            name   = "Coin";
            target = AHUD->coinDrawable;
            AAudioTool->Play(AudioID_PickCoin);
            break;

        case CollisionBodyID_Stone:
            name   = "Stone";
            target = AHUD->stoneDrawable;
            AAudioTool->Play(AudioID_PickStone);
            break;

        case CollisionBodyID_Wood:
            name   = "Wood";
            target = AHUD->woodDrawable;
            AAudioTool->Play(AudioID_PickWood);
            break;

        default:
            return;
    }

    Drawable* drawable = ADrawAtlas->GetQuad(dropDrawAtlas, name)->drawable;

    ADrawable_SetParent   (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2(drawable, item->subMesh->drawable->positionX, item->subMesh->drawable->positionY);

    ADrawable->ConvertToParent(drawable, target->parent);
    ADrawable_SetScaleSame2   (drawable, 0.5f);

    ATweenTool->AddMove2      (target->positionX, target->positionY, 1.2f, false, TweenEaseType_SineOut)
              ->SetUserDataPtr(0, item)
              ->SetUserDataPtr(1, drawable)
              ->SetOnComplete (OnItemGet)
              ->RunActions    (drawable);

    // item tween may not complete
    ATween->TryRemoveAllActions(item->subMesh->drawable);
    ClearItem(item);
}


struct AHUD AHUD[1] =
{{
    .Init          = Init,
    .Run           = Run,
    .CostPower     = CostPower,
    .OpenCurtain   = OpenCurtain,
    .CloseCurtain  = CloseCurtain,
    .Reset         = Reset,
    .DropStone     = DropStone,
    .DropWood      = DropWood,
    .ClearDrop     = ClearDrop,
    .DropPickHeart = DropPickHeart,
    .DropPickCoin  = DropPickCoin,
    .DropPickStone = DropPickStone,
    .DropPickWood  = DropPickWood,
    .FlyItem       = FlyItem,
}};
