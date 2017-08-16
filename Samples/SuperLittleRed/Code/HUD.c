/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-12-2
 * Author : scott.cgi
 * Version: 0.0.0
 */


#include <stdio.h>
#include <string.h>

#include <Engine/Toolkit/Utils/ArrayIntSet.h>
#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Extension/Font.h"
#include "Engine/Graphics/OpenGL/GLPrimitive.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Application/Application.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Toolkit/Utils/Coroutine.h"

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


//----------------------------------------------------------------------------------------------------------------------


static bool       isCostEnergy   = false;
static float      recoverTime    = 0.0f;
static FontText*  roundText      = NULL;
static FontText*  roundNumText   = NULL;
static FontText*  timeText       = NULL;
static DrawAtlas* drawAtlas      = NULL;
static Scheduler* timeScheduler  = NULL;


//----------------------------------------------------------------------------------------------------------------------


static void Update(Component* component, float deltaSeconds)
{
    ADrawAtlas_Draw(drawAtlas);
    ASkeletonAnimationPlayer_Update(AHUD->hudPlayer,     deltaSeconds);
    ASkeletonAnimationPlayer_Update(AHUD->curtainPlayer, deltaSeconds);


#ifdef APP_DEBUG

    for (int i = 0; i < collisionItemDebugDrawSet->elementList->size; i++)
    {
        DropCollisionItem* item = AArrayList_Get(collisionItemDebugDrawSet->elementList, i, DropCollisionItem*);

        if (APhysicsBody_CheckState(item->body, PhysicsBodyState_IsFreeze) == false)
        {
            ADrawable->Draw
            (
                item->debugDrawable
            );
        }
    }

#endif
    

    if (isCostEnergy == false)
    {
        if (AHero->roundEnergy < AGameData->maxEnergy)
        {
            if (recoverTime > 1.0f)
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
    ATool->SetTimeToBuff(buff,     AHero->roundTime);
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


static void StartRunGame(Coroutine* coroutine)
{
    ACoroutine_Begin();

    AGameMap->Run();

    ACoroutine_YieldSeconds(0.2f);
    AHUD->CloseCurtain(NULL);
    AHero     ->Run();
    AGameActor->Run();
    AEnemyAI  ->Run();

    ACoroutine_End();
}


static void OnInitActionOver(SkeletonAnimationPlayer* player)
{
    OnCurtainActionOver(player);
    AUI->Run();

    ACoroutine->StartCoroutine(StartRunGame);
}


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

//----------------------------------------------------------------------------------------------------------------------

    ASkeletonAnimationPlayer->Init("UI/Curtain", "open", AHUD->curtainPlayer);
    AHUD->curtainPlayer->loop = 0;

    ADrawable_SetScale2
    (
        ASkeletonAnimationPlayer_GetDrawable(AHUD->curtainPlayer),
        ATool->globalScaleX,
        ATool->globalScaleY
    );

//----------------------------------------------------------------------------------------------------------------------

    AHUD->energyLengthDrawable  = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "EnergyLength")          ->drawable;
    AHUD->energyDrawable        = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Energy")                ->drawable;
    AHUD->tombstoneDrawable     = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Tombstone")             ->drawable;
    AHUD->tombstoneHeight       = ASkeleton_GetSubMesh            (AHUD->hudPlayer->skeleton, "Tombstone", "Tombstone")->drawable->height;
    AHUD->tombstoneBornY        = AHUD->tombstoneDrawable->positionY;
    AHUD->upDrawable            = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Up")                    ->drawable;
    AHUD->downDrawable          = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Down")                  ->drawable;

    ADrawable_SetInVisible(AHUD->tombstoneDrawable);

    AHUD->heartDrawable         = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Heart")                 ->drawable;
    AHUD->coinDrawable          = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Coin")                  ->drawable;
    AHUD->stoneDrawable         = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Stone")                 ->drawable;
    AHUD->woodDrawable          = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Wood")                  ->drawable;
    AHUD->killDrawable          = ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer,           "Kill")                  ->drawable;
    AHUD->energyPointerDrawable = ASkeletonAnimationPlayer_GetSubMesh(AHUD->hudPlayer,       "Pointer", "Pointer")     ->drawable;

//----------------------------------------------------------------------------------------------------------------------

    AHUD->tryAgainCount = 0;

    roundText = AFont->GetText(AGameActor->talkFont);
    AFont->SetString          (roundText, "Round");
    ADrawable_SetScaleSame2   (roundText->drawable, 1.5f);
    ADrawable_SetPositionY    (roundText->drawable, 0.2f);
    ADrawable_SetPositionX    (roundText->drawable, -roundText->drawable->width * 1.5f / 2);
    ADrawable_SetOpacity      (roundText->drawable, 0.0f);

    roundNumText = AFont->GetText(AGameActor->talkFont);
    ADrawable_SetScaleSame2      (roundNumText->drawable, 1.2f);
    ADrawable_SetPositionY       (roundNumText->drawable, 0.2018f);
    ADrawable_SetPositionX       (roundNumText->drawable, -roundText->drawable->positionX);
    ADrawable_SetOpacity         (roundNumText->drawable, 0.0f);

    timeText = AFont->GetText(AGameActor->talkFont);
    ADrawable_SetOpacity     (timeText->drawable, 0.0f);
    ADrawable_SetParent
    (
        timeText->drawable,
        ASkeletonAnimationPlayer_GetBone(AHUD->hudPlayer, "ScorePos")->drawable
    );

//----------------------------------------------------------------------------------------------------------------------

    Drawable* items[] =
    {
        AHUD->heartDrawable,
        AHUD->coinDrawable,
        AHUD->killDrawable,
        AHUD->stoneDrawable,
        AHUD->woodDrawable,
    };

    FontText** textPtrs[] =
    {
        &AHUD->heartText,
        &AHUD->coinText,
        &AHUD->killText,
        &AHUD->stoneText,
        &AHUD->woodText,
    };

    for (int i = 0; i < 5; i++)
    {
        FontText* text  = AFont->GetText(AGameActor->talkFont);
        text->alignment = FontTextAlignment_HorizontalRight;

        ADrawable_SetParent
        (
            text->drawable,
            items[i]
        );

        if (i > 2)
        {
            ADrawable_SetPositionX(text->drawable, -AGLTool_ToGLWidth(35));
        }
        else
        {
            ADrawable_SetPositionX(text->drawable, -AGLTool_ToGLWidth(25));
        }

        *textPtrs[i] = text;
    }

//----------------------------------------------------------------------------------------------------------------------

    drawAtlas = ADrawAtlas->Get("Texture/Drop.atlas");
}


static void CostEnergyActionComplete(TweenAction* action)
{
    isCostEnergy = false;
}


static void TalkTextTweenActionOnComplete(TweenAction* tweenAction)
{
    AFont->ReuseText((FontText*) tweenAction->userData->slot0->ptrValue);
}


static bool CostPower(float cost)
{
    if (AHero->roundEnergy >= cost)
    {
        AHero->roundEnergy -= cost;
        isCostEnergy        = true;
        float scaleY        = (float) AHero->roundEnergy / (float) AGameData->maxEnergy;

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

//----------------------------------------------------------------------------------------------------------------------

        FontText* text = AFont->GetText(AGameActor->talkFont);
        ADrawable_SetParent(text->drawable, AGameMap->beforeDrawable);

        AFont->SetString(text, "Power");

        ADrawable_SetPosition2
        (
            text->drawable,
            ADrawable->ConvertBetweenLocalX
            (
                AGameMap          ->groundPosDrawable,
                AHero_GetDrawable()->positionX - text->drawable->width / 2,
                AGameMap          ->beforeDrawable
            ),
            AHero_GetDrawable()->positionY + ASkeletonAnimationPlayer_GetHeight(AHero->player)
        );

        ATweenTool->AddAction      ()

                  ->SetFadeTo      (0.0f)
                  ->SetRelative    (false)
                  ->SetEaseType    (TweenEaseType_SineOut)

                  ->SetMoveY       (0.1f)
                  ->SetEaseType    (TweenEaseType_ExponentialOut)

                  ->SetDuration    (3.0f)
                  ->SetOnComplete  (TalkTextTweenActionOnComplete)
                  ->SetUserData0Ptr(text)

                  ->RunActions     (text->drawable);

        return false;
    }
}


static void OpenCurtain(SkeletonAnimationPlayerOnActionOver callback)
{
    ASkeletonAnimationPlayer->SetAnimation(AHUD->curtainPlayer, "open");
    AHUD->curtainPlayer->loop         = -1;
    AHUD->curtainPlayer->OnActionOver = OnCurtainActionOver;

    AAudioTool->Play(AudioId_CurtainSlideShow);
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

    AAudioTool->Play(AudioId_CurtainSlideHide);
    AAudioTool->Play(AudioId_CurtainRing);
    AAudioTool->StartGameBG();

    if (callback == NULL)
    {
        OnCurtainCallback = NULL;
    }
    else
    {
        OnCurtainCallback = callback;
    }

    ADrawable_SetParent     (roundText->drawable, AGameMap->beforeDrawable);

    ATweenTool->AddFadeTo   (1.0, 2.0f)
              ->SetEaseType (TweenEaseType_SineInOut)
              ->SetRelative (false)

              ->AddFadeTo   (0.0, 3.0f)
              ->SetEaseType (TweenEaseType_QuinticIn)
              ->SetRelative (false)

              ->RunActions  (roundText->drawable);

//----------------------------------------------------------------------------------------------------------------------

    AFont->SetInt           (roundNumText, ++AHUD->tryAgainCount);
    ADrawable_SetParent     (roundNumText->drawable, AGameMap->beforeDrawable);

    ATweenTool->AddFadeTo   (1.0, 2.0f)
              ->SetEaseType (TweenEaseType_SineInOut)
              ->SetRelative (false)

              ->AddFadeTo   (0.0, 3.0f)
              ->SetEaseType (TweenEaseType_QuinticIn)
              ->SetRelative (false)

              ->RunActions  (roundNumText->drawable);
}


static void Run()
{
    AApplication_AddChild(AHUD->component, 100);
    AHUD->curtainPlayer->OnActionOver = OnInitActionOver;
    AHUD->curtainPlayer->loop         = -1;

    AAudioTool->Play(AudioId_CurtainSlideShow);
}


static void Reset()
{
    AFont->SetInt(AHUD->heartText, AHero->roundHP);
    AFont->SetInt(AHUD->coinText,  AGameData->coin);
    AFont->SetInt(AHUD->woodText,  AGameData->wood);
    AFont->SetInt(AHUD->stoneText, AGameData->stone);
    AFont->SetInt(AHUD->killText,  AHero->roundKillCount);

    ADrawable_SetScaleY(AHUD->energyLengthDrawable,  1.0f);
    ADrawable_SetScaleY(AHUD->energyPointerDrawable, 1.0f);
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


//----------------------------------------------------------------------------------------------------------------------


static void OnStoneFade(TweenAction* action)
{
    ADrawAtlas  ->ReuseQuad(drawAtlas, (Drawable*) action->userData->slot0->ptrValue);
    AArrayIntSet->TryRemove(dropSet,   (intptr_t)  action->userData->slot0->ptrValue);
}


static void OnStoneDown(TweenAction* action)
{
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

    AEnemyAI->SetAllEnemy(EnemyState_Stand, AGameData->stonePower);

    AAudioTool->Play(AudioId_FallDown);
}


static inline Drawable* DropStoneWood
(
    char*                 name,
    float                 fadeTime,
    TweenActionOnComplete OnDown,
    TweenActionOnComplete OnFade,
    void*                 userData
)
{
    Drawable* drawable = ADrawAtlas->GetQuad(drawAtlas, name);
    float     posX     = ADrawable->ConvertBetweenLocalX
                         (
                            AGameMap          ->groundPosDrawable,
                            AHero_GetDrawable()->positionX,
                            AGameMap          ->beforeDrawable
                         );

    float     posY     = ADrawable->ConvertToLocalY(AGameMap->beforeDrawable, 1.0f + drawable->height / 2);

    ADrawable_SetParent   (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2(drawable, posX, posY);

    ATweenTool->AddMoveY
                (
                    AGameMap->groundY + drawable->height / 2 - AGLTool_ToGLHeight(0.5f),
                    0.35f
                )
              ->SetEaseType    (TweenEaseType_ExponentialIn)
              ->SetRelative    (false)
              ->SetUserData0Ptr(userData == NULL ? drawable : userData)
              ->SetOnComplete  (OnDown)

              ->AddFadeTo      (0.0f, fadeTime)
              ->SetRelative    (false)
              ->SetUserData0Ptr(userData == NULL ? drawable : userData)
              ->SetEaseType    (TweenEaseType_ExponentialIn)
              ->SetOnComplete  (OnFade)

              ->RunActions     (drawable);

    AArrayIntSet->TryAdd(dropSet, (intptr_t) drawable);

    return drawable;
}


static void DropStone()
{
    DropStoneWood("DropStone", 2.0f, OnStoneDown, OnStoneFade, NULL);
}


//----------------------------------------------------------------------------------------------------------------------


#ifdef APP_DEBUG

static void DebugRender(Drawable* drawable)
{
    DropCollisionItem* item   = (DropCollisionItem*) drawable->userData->slot0->ptrValue;

    AGLPrimitive->color->r    = 0.0f;
    AGLPrimitive->color->b    = 0.0f;
    AGLPrimitive->color->g    = 1.0f;
    AGLPrimitive->modelMatrix = AGameMap->beforeDrawable->modelMatrix;
    AGLPrimitive->DrawPolygon(item->body->positionArr);
}

#endif


static void OnItemFade(TweenAction* action)
{
    DropCollisionItem* item = (DropCollisionItem*) action->userData->slot0->ptrValue;
    ADrawAtlas->ReuseQuad(drawAtlas, item->drawable);
    APhysicsBody_SetState(item->body, PhysicsBodyState_IsFreeze);

    AArrayList_Add(collisionItemList, item);
    AArrayIntSet->TryRemove(dropSet, (intptr_t) item->drawable);


#ifdef APP_DEBUG
    AArrayIntSet->TryRemove(collisionItemDebugDrawSet, (intptr_t) item);
#endif
}


static void OnItemDown(TweenAction* action)
{
    DropCollisionItem* item = (DropCollisionItem*) action->userData->slot0->ptrValue;

    item->body->positionX   = item->drawable->positionX;
    item->body->positionY   = item->drawable->positionY;
    APhysicsBody->UpdateMotion(item->body, 0.0f);

    APhysicsBody_ClearState(item->body, PhysicsBodyState_IsFreeze);
}


static inline DropCollisionItem* DropItem(int bodySize, int bodyId, int collisionGroup)
{
    DropCollisionItem* item = NULL;

    for (int i = 0; i < collisionItemList->size; i++)
    {
        DropCollisionItem* tmp = AArrayList_Get(collisionItemList, i, DropCollisionItem*);

        if (tmp->bodySize == bodySize && tmp->bodyId == bodyId && tmp->collisionGroup == collisionGroup)
        {
            item = tmp;
            AArrayList->Remove(collisionItemList, i);
            break;
        }
    }

    if (item == NULL)
    {
        item                 = (DropCollisionItem*) malloc(sizeof(DropCollisionItem));
        item->bodyId         = bodyId;
        item->bodySize       = bodySize;
        item->collisionGroup = collisionGroup;

        float len  = AGLTool_ToGLWidth(bodySize);
        item->body = APhysicsWorld->AddBody
                     (
                        PhysicsShape_Polygon,
                        AArray_Make
                        (
                            float, 8,
                            -len,  len,
                             len,  len,
                             len, -len,
                            -len, -len,
                        )
                     );

        item->body->userId                    = bodyId;
        item->body->userData->slot0->ptrValue = item;
        APhysicsBody_SetCollisionGroup(item->body, collisionGroup);
        APhysicsBody_SetState(item->body, PhysicsBodyState_IsFixed);


#ifdef APP_DEBUG
        ADrawable->Init(item->debugDrawable);
        item->debugDrawable->userData->slot0->ptrValue = item;
        item->debugDrawable->Render                    = DebugRender;
#endif

    }

    APhysicsBody_SetState(item->body, PhysicsBodyState_IsFreeze);


#ifdef APP_DEBUG
    AArrayIntSet->TryAdd(collisionItemDebugDrawSet, (intptr_t) item);
#endif

    return item;
}


static void OnWoodDown(TweenAction* action)
{
    AAudioTool->Play(AudioId_FallDown);
    OnItemDown(action);

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
}


static void DropWood()
{
    DropCollisionItem* item = DropItem(18, CollisionBodyId_Wood, CollisionGroup_HeroAttack | CollisionGroup_Attack);
    item->drawable          = DropStoneWood("DropWood", AGameData->woodTime, OnWoodDown, OnItemFade, item);
}


//----------------------------------------------------------------------------------------------------------------------


static inline Drawable* DropHeartCoinStoneWood(char* name, void* userData, PhysicsBody* arrowBody, float enemyScaleX)
{
    Drawable* drawable = ADrawAtlas->GetQuad(drawAtlas, name);

    ADrawable_SetParent    (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2 (drawable, arrowBody->positionX, arrowBody->positionY);
    ADrawable_SetScaleSame2(drawable, 0.3f);

    float scaleY = 0.75f;

    ATweenTool->AddMoveY
                (
                    AGameMap->groundY + drawable->height * scaleY / 2 - AGLTool_ToGLHeight(0.5f),
                    0.8f
                )
              ->SetEaseType    (TweenEaseType_BounceOut)
              ->SetRelative    (false)

              ->SetScaleSame2  (scaleY, false, TweenEaseType_QuarticOut)

              ->AddMoveX       (-0.05f * enemyScaleX * fabsf(arrowBody->velocityX), 1.4f)
              ->SetQueue       (false)
              ->SetEaseType    (TweenEaseType_CubicOut)
              ->SetUserData0Ptr(userData)
              ->SetOnComplete  (OnItemDown)

              ->AddFadeTo      (0.0, 14.0f)
              ->SetRelative    (false)
              ->SetEaseType    (TweenEaseType_ExponentialIn)
              ->SetUserData0Ptr(userData)
              ->SetOnComplete  (OnItemFade)

              ->RunActions     (drawable);

    AArrayIntSet->TryAdd(dropSet, (intptr_t) drawable);

    return drawable;
}


static inline void DropPickItem(char* name, int size,int bodyId, PhysicsBody* arrowBody, float enemyScaleX)
{
    DropCollisionItem* item = DropItem(size, bodyId, CollisionGroup_EnemyAttack | CollisionGroup_Attack);
    item->drawable          = DropHeartCoinStoneWood(name, item, arrowBody, enemyScaleX);
}


static void DropPickHeart(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropPickItem("Heart", 9, CollisionBodyId_Heart, arrowBody, enemyScaleX);
}


static void DropPickCoin(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropPickItem("Coin", 9, CollisionBodyId_Coin, arrowBody, enemyScaleX);
}


static void DropPickStone(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropPickItem("Stone", 9, CollisionBodyId_Stone, arrowBody, enemyScaleX);
}


static void DropPickWood(PhysicsBody* arrowBody, float enemyScaleX)
{
    DropPickItem("Wood", 9, CollisionBodyId_Wood, arrowBody, enemyScaleX);
}


static void OnItemGet(TweenAction* action)
{
    Drawable* drawable = (Drawable*) action->userData->slot0->ptrValue;
    int       bodyId   = drawable->userData->slot0->intValue;

    switch (bodyId)
    {
        case CollisionBodyId_Heart:
            AFont->SetInt(AHUD->heartText, ++AHero->roundHP);
            break;

        case CollisionBodyId_Coin:
            AFont->SetInt(AHUD->coinText,  ++AGameData->coin);
            break;

        case CollisionBodyId_Stone:
            AFont->SetInt(AHUD->stoneText, ++AGameData->stone);
            break;

        case CollisionBodyId_Wood:
            AFont->SetInt(AHUD->woodText,  ++AGameData->wood);
            break;
    }

    ADrawAtlas->ReuseQuad(drawAtlas, drawable);
}


static void FlyItem(DropCollisionItem* item)
{
    char*     name   = NULL;
    Drawable* target = NULL;

    switch (item->bodyId)
    {
        case CollisionBodyId_Heart:
            name   = "Heart";
            target = AHUD->heartDrawable;
            AAudioTool->Play(AudioId_PickHeart);
            break;

        case CollisionBodyId_Coin:
            name   = "Coin";
            target = AHUD->coinDrawable;
            AAudioTool->Play(AudioId_Coin);
            break;

        case CollisionBodyId_Stone:
            name   = "Stone";
            target = AHUD->stoneDrawable;
            AAudioTool->Play(AudioId_PickStone);
            break;

        case CollisionBodyId_Wood:
            name   = "Wood";
            target = AHUD->woodDrawable;
            AAudioTool->Play(AudioId_PickWood);
            break;
    }

    Drawable* drawable = ADrawAtlas->GetQuad(drawAtlas, name);

    ADrawable_SetParent   (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2(drawable, item->drawable->positionX, item->drawable->positionY);

    ADrawable->ConvertToParent(drawable, target->parent);
    ADrawable_SetScaleSame2   (drawable, 0.55f);

    drawable->userData->slot0->intValue = item->bodyId;

    ATweenTool->AddMove2       (target->positionX, target->positionY, 1.0f, false, TweenEaseType_SineOut)
              ->SetUserData0Ptr(drawable)
              ->SetOnComplete  (OnItemGet)
              ->RunActions     (drawable);

    ATween->TryCompleteAllActions(item->drawable, true);
}


//----------------------------------------------------------------------------------------------------------------------


struct AHUD AHUD[1] =
{
    {
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
    }
};
