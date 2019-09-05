/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2015-9-29
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Toolkit/Math/Vector.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Toolkit/HeaderUtils/Struct.h"

#include "GameActor.h"
#include "GameMap.h"
#include "Hero.h"
#include "Tool.h"
#include "EnemyAI.h"
#include "GameData.h"
#include "HUD.h"
#include "Enemy.h"
#include "AudioTool.h"


static void Update(Component* component, float deltaSeconds)
{
    Enemy* enemy = AUserData_GetSlotPtrWithType(component->userData, 0, Enemy*);

    if (enemy->collisionBoxBody != NULL)
    {
        if (enemy->collisionBoxBody->state != PhysicsBodyState_Freeze)
        {
            ATool->UpdateBox(enemy->collisionBoxBody, enemy->collisionBoxDrawable);
        }
    }

    if (component->curState->id == EnemyState_Attack && enemy->attackBoxBody != NULL)
    {
        ATool->UpdateBox(enemy->attackBoxBody, enemy->attackBoxDrawable);
    }

    ASkeletonAnimationPlayer_Update(enemy->player, deltaSeconds);

    for (int i = 0; i < enemy->arrowSet->elementList->size; ++i)
    {
        Arrow* arrow = AArrayList_Get(enemy->arrowSet->elementList, i, Arrow*);
        ATool->UpdateArrowTail(arrow, deltaSeconds);
        ADrawable->Draw(arrow->sprite->drawable);
        ADrawable->Draw(arrow->tailDrawable);
    }

    ASkeletonAnimationPlayer_Update(enemy->hurtEffect,  deltaSeconds);
    ASkeletonAnimationPlayer_Update(enemy->dizzyEffect, deltaSeconds);

    #ifdef APP_DEBUG
    if (enemy->collisionBoxBody != NULL)
    {
        ADrawable->Draw(enemy->debugCollisionBoundingBox);
    }

    if (component->curState->id == EnemyState_Attack && enemy->attackBoxBody)
    {
        ADrawable->Draw(enemy->debugAttackBoundingBox);
    }
    #endif
}


static void HurtTextTweenActionOnComplete(TweenAction* tweenAction)
{
    AFont->ReleaseText(AUserData_GetSlotPtrWithType(tweenAction->userData, 0, FontText*));
}


static void WoodStopSchedulerUpdate(Scheduler* scheduler, float deltaSeconds)
{
    AUserData_GetSlotPtrWithType(scheduler->userData, 0, Enemy*)->isWoodStop = false;
}


static void WoodStopActionOnComplete(TweenAction* action)
{
    AUserData_GetSlotPtrWithType(action->userData, 0, Enemy*)->isWoodStop = false;
}


static void OnHurt(PhysicsBody* self, PhysicsBody* other, float deltaSeconds)
{
    Enemy* enemy = AUserData_GetSlotPtrWithType(self->userData, 0, Enemy*);

    if (enemy->component->curState->id >= EnemyState_Die)
    {
        return;
    }

    Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);

    if (other->userID == CollisionBodyID_Wood)
    {
        if (enemy->isWoodStop == false)
        {
            enemy->isWoodStop = true;

            ATween->TryRemoveAllActions(enemyDrawable);

            // once stop action make sure stand on ground
            ATweenTool->AddMoveY   (AGameMap->groundY, enemy->hurtYDownTime)
                      ->SetEaseType(TweenEaseType_CubicIn)
                      ->SetRelative(false)
                      ->RunActions (enemyDrawable);

            if (AMath_Random() < 0.05f)
            {
                AComponent->SetState(enemy->component, EnemyState_Dizzy);
            }
            else if (AMath_Random() < 0.6f)
            {
                AComponent->SetState(enemy->component, EnemyState_Walk);

                if (AMath_Random() < 0.2f)
                {
                    float deltaX;

                    if (enemyDrawable->scaleX > 0.0f)
                    {
                        deltaX =  AMath_Random() * 0.1f;
                    }
                    else
                    {
                        deltaX = -AMath_Random() * 0.1f;
                    }

                    ATweenTool->AddMoveX      (deltaX, fabsf(deltaX) / enemy->speed)
                              ->SetEaseType   (TweenEaseType_Linear)
                              ->SetUserDataPtr(0, enemy)
                              ->SetOnComplete (WoodStopActionOnComplete)
                              ->SetQueue      (false)
                              ->RunActions    (enemyDrawable);

                    goto ScheduleOnce;
                }
            }
            else if (AMath_Random() < 0.9f)
            {
                AComponent->SetState(enemy->component, EnemyState_Attack);
            }
            else
            {
                AComponent->SetState(enemy->component, EnemyState_Stand);
            }

            ScheduleOnce:
            
            AUserData_SetSlotPtr
            (
                AScheduler->ScheduleOnce(WoodStopSchedulerUpdate, 0.5f)->userData,
                0,
                enemy
            );
        }

        return;
    }

//----------------------------------------------------------------------------------------------------------------------

    Arrow* arrow = AUserData_GetSlotPtrWithType(other->userData, 0, Arrow*);

    if (arrow->enemy != NULL && arrow->enemy != enemy)
    {
        return;
    }

//----------------------------------------------------------------------------------------------------------------------

    float hurtEffectPos;

    enemy->hp -= AMath_RandomInt(0, AHero->roundKillCount / Enemy_KillIncrease) + 1;

    if (enemy->hp > 0)
    {
        hurtEffectPos = 30;
        AComponent->SetState(enemy->component, EnemyState_Hurt);

        FontText* text = AFont->GetText(AGameActor->hpFont);
        ADrawable_SetParent(text->drawable, AGameMap->beforeDrawable);

        ADrawable_SetPosition2
        (
            text->drawable,
            ADrawable->ConvertBetweenLocalPositionX(enemyDrawable, 0.0f, AGameMap->beforeDrawable),
            other->positionY
        );

        AFont->SetInt(text, enemy->hp);

        ATweenTool->AddAction     ()

                  ->SetFadeTo     (0.0f)
                  ->SetRelative   (false)
                  ->SetEaseType   (TweenEaseType_ExponentialOut)

                  ->SetMoveY      (0.08f)
                  ->SetEaseType   (TweenEaseType_CubicIn)

                  ->SetMoveX      (0.1f * enemyDrawable->scaleX)
                  ->SetEaseType   (TweenEaseType_ExponentialOut)

                  ->SetScaleX     (0.5f)
                  ->SetRelative   (false)
                  ->SetEaseType   (TweenEaseType_CubicOut)

                  ->SetScaleY     (0.5f)
                  ->SetRelative   (false)
                  ->SetEaseType   (TweenEaseType_CubicOut)

                  ->SetDuration   (1.8f)
                  ->SetOnComplete (HurtTextTweenActionOnComplete)
                  ->SetUserDataPtr(0, text)

                  ->RunActions    (text->drawable);
    }
    else
    {
        hurtEffectPos = 40;
        ASkeleton ->SetSkin (enemy->hurtEffect->skeleton, "die");
        AComponent->SetState(enemy->component, EnemyState_Die);

        // if (AMath_Random() < 0.8f)
        {
            float random = AMath_Random();

            if (random < 0.20f)
            {
                AHUD->DropPickCoin(other, enemyDrawable->scaleX);
            }
            else if (random < 0.30f)
            {
                AHUD->DropPickHeart(other, enemyDrawable->scaleX);
            }
            else if (random < 0.65f)
            {
                AHUD->DropPickStone(other, enemyDrawable->scaleX);
            }
            else
            {
                AHUD->DropPickWood(other, enemyDrawable->scaleX);
            }

            AAudioTool->Play(AudioID_Drop);
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    enemy->hurtEffect->loop      = 1;
    Drawable* hurtEffectDrawable = ASkeletonAnimationPlayer_GetDrawable(enemy->hurtEffect);

    ADrawable_SetPosition2
    (
        hurtEffectDrawable,
        other->positionX + AGLTool_ToGLWidth (hurtEffectPos) * enemyDrawable->scaleX,
        other->positionY + AGLTool_ToGLHeight(10.0f)
    );

    ADrawable_SetRotationZ
    (
        hurtEffectDrawable,
        ADrawable->GetFlipRotationZ(hurtEffectDrawable, other->rotationZ)
    );

//----------------------------------------------------------------------------------------------------------------------

    if (enemy->collisionBoxDrawable != NULL)
    {
        ATween->TryRemoveAllActions(enemyDrawable);

        float dir;
        if (AHero_GetDrawable()->scaleX > 0.0f)
        {
            dir =  1.0f;
        }
        else
        {
            dir = -1.0f;
        }

        if (enemy->component->curState->id == EnemyState_Die)
        {
            float sx = fabsf(other->velocityX) * enemy->dieMoveX;
            float sy = fabsf(other->velocityY) * enemy->dieMoveY;

            ATweenTool->AddMoveX   (dir * sx, enemy->dieXTime)
                      ->SetEaseType(enemy->dieMoveXEaseType)
                      ->SetQueue   (false)

                      ->AddMoveY   (sy, enemy->dieYUpTime)
                      ->SetEaseType(TweenEaseType_QuadraticIn)

                      ->AddMoveY   (AGameMap->groundY, enemy->dieYDownTime)
                      ->SetEaseType(enemy->dieMoveYEaseType)
                      ->SetRelative(false)

                      ->RunActions (enemyDrawable);
        }
        else
        {
            float sx = fabsf(other->velocityX) * enemy->hurtMoveX;
            float sy = fabsf(other->velocityY) * enemy->hurtMoveY;

            ATweenTool->AddMoveX   (dir * sx, enemy->hurtXTime)
                      ->SetEaseType(TweenEaseType_QuadraticOut)
                      ->SetQueue   (false)

                      ->AddMoveY   (sy, enemy->hurtYUpTime)
                      ->SetEaseType(TweenEaseType_QuadraticIn);

            if (enemy->hurtYDownTime != 0)
            {
                ATweenTool ->AddMoveY   (AGameMap->groundY, enemy->hurtYDownTime)
                           ->SetEaseType(TweenEaseType_QuadraticOut)
                           ->SetRelative(false);
            }

            ATweenTool->RunActions(enemyDrawable);
        }

        other->velocityX *= 0.5f;
        other->velocityY *= 0.5f;
    }
}


static void Destroy(Enemy* enemy)
{
    for (int i = 0; i < enemy->arrowSet->elementList->size; ++i)
    {
        Arrow* arrow = AArrayList_Get(enemy->arrowSet->elementList, i, Arrow*);
        ATween->TryCompleteAllActions(arrow->sprite->drawable, false);
        AHero->ResetArrow(arrow);
    }

    AArrayList->Clear(enemy->arrowSet->elementList);

    AArrayIntSet->TryRemove    (enemy->belongAI, (intptr_t) enemy);
    ATween->TryRemoveAllActions(AEnemy_GetDrawable(enemy));
    AComponent->RemoveObserver (AHero->component, enemy->component);

    if (enemy->collisionBoxBody != NULL)
    {
        enemy->collisionBoxBody->state = PhysicsBodyState_Freeze;
        ATool->InitBox(enemy->collisionBoxBody, 2.0f, 2.0f);
    }

    if (enemy->attackBoxBody != NULL)
    {
        enemy->attackBoxBody->state = PhysicsBodyState_Freeze;
        ATool->InitBox(enemy->attackBoxBody, 2.0f, 2.0f);
    }

    if (enemy->isBoss)
    {
        enemy->isBoss       = false;
        AEnemyAI->isHasBoss = false;
    }

    ADrawable_SetScaleSame2(AEnemy_GetDrawable(enemy), 1.0f);

    AArrayList_Add(AEnemyAI->enemyDeadList, enemy);

    --AEnemyAI->currentEnemyNum;
    
    AComponent->SetActive(enemy->component, false);
}


static void Reset(Enemy* enemy, float bornX, float bornY)
{
    AComponent->SetActive(enemy->component, true);

    ADrawable_SetParent    (AEnemy_GetDrawable(enemy), AGameMap->beforeDrawable);
    ADrawable_SetPosition2 (AEnemy_GetDrawable(enemy), bornX, bornY);
    AComponent->AddObserver(AHero->component, enemy->component);

    if (enemy->collisionBoxBody != NULL)
    {
        enemy->collisionBoxBody->state = PhysicsBodyState_Fixed;
        ATool->InitBox(enemy->collisionBoxBody, 2.0f, 2.0f);
    }

    if (enemy->attackBoxBody != NULL)
    {
        enemy->attackBoxBody->state = PhysicsBodyState_Fixed;
        ATool->InitBox(enemy->attackBoxBody, 2.0f, 2.0f);
    }

    AComponent->SetState(enemy->component, EnemyState_Stand);
    enemy->player->loop = -1;

    ADrawable_SetParent(ASkeletonAnimationPlayer_GetDrawable(enemy->hurtEffect), AGameMap->beforeDrawable);
}


static void ScheduleDestroy(Scheduler* scheduler, float deltaSeconds)
{
    Destroy(AUserData_GetSlotPtrWithType(scheduler->userData, 0, Enemy*));
}


static void OnActionOver(SkeletonAnimationPlayer* player)
{
    Enemy* enemy = AStruct_GetParent(player, Enemy);

    switch (enemy->component->curState->id)
    {
        case EnemyState_Dizzy:
            if (enemy->dizzyTime > 0)
            {
                --enemy->dizzyTime;
            }
            else
            {
                enemy->dizzyEffect->loop = 0;
                AComponent->SetState(enemy->component, EnemyState_Walk);
            }
            break;

        case EnemyState_Hurt:
            if (enemy->dizzyTime == 0 || enemy->component->curState->id != EnemyState_Dizzy)
            {
                AComponent->SetState(enemy->component, EnemyState_Walk);
            }
            else
            {
                AComponent->SetState(enemy->component, EnemyState_Dizzy);
            }
            break;

        case EnemyState_Attack:
            AComponent->SetState(enemy->component, EnemyState_Stand);
            break;

        case EnemyState_Die:
            AComponent->SetState(enemy->component, EnemyState_DieOver);
            break;

        case EnemyState_DieOver:
            AUserData_SetSlotPtr(AScheduler->ScheduleOnce(ScheduleDestroy, 0.0f)->userData, 0, enemy);
            enemy->player->loop = 0;
            break;

        default:
            break;
    }
}


static void Dizzy(Scheduler* scheduler, float deltaSeconds)
{
    Enemy* enemy = AUserData_GetSlotPtrWithType(scheduler->userData, 0, Enemy*);
    
    AComponent->SetState(enemy->component, EnemyState_Dizzy);

    ATween->TryRemoveAllActions(AEnemy_GetDrawable(enemy));

    if (enemy->type != EnemyType_Fly)
    {
        ATweenTool->AddMoveY   (0.05f, 0.2f)
                  ->SetEaseType(TweenEaseType_SineIn)
                  ->AddMoveY   (AGameMap->groundY, 0.3f)
                  ->SetRelative(false)
                  ->SetEaseType(TweenEaseType_BounceOut)
                  ->RunActions (AEnemy_GetDrawable(enemy));
    }
    else
    {
        ATweenTool->AddMoveY   (0.02f,  0.2f)
                  ->SetEaseType(TweenEaseType_SineIn)

                  ->AddMoveY   (-0.02f, 0.3f)
                  ->SetEaseType(TweenEaseType_BounceOut)
                  ->RunActions (AEnemy_GetDrawable(enemy));
    }

}



static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (sender == AHero->component)
    {
        Enemy*    enemy         = AUserData_GetSlotPtrWithType(component->userData, 0, Enemy*);
        Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);

        if (enemy->component->curState->id >= EnemyState_Die)
        {
            return false;
        }

        switch (subject)
        {
            case HeroNotify_Die:
                break;

            case HeroNotify_Dizzy:
            {
                Drawable* heroDrawable = AHero_GetDrawable();
                float     posX         = ADrawable->ConvertBetweenLocalPositionX
                                         (
                                             heroDrawable->parent,
                                             heroDrawable->positionX,
                                             enemyDrawable->parent
                                         );

                if (fabsf(enemyDrawable->positionX - posX) < AGameData->dizzyDistance)
                {
                    AUserData_SetSlotPtr
                    (
                        AScheduler->ScheduleOnce
                        (
                            Dizzy,
                            fabsf(enemyDrawable->positionX - posX) / 3
                        )
                        ->userData,
                        0,
                        enemy
                    );
                }
                else
                {
                    ATween->TryRemoveAction(enemyDrawable, enemy->belongAIAction);
                    AComponent->SetState(enemy->component, EnemyState_Stand);
                }
                break;
            }

            default:
                break;
        }
    }
    else if(sender == AComponent)
    {
        Enemy* enemy = AUserData_GetSlotPtrWithType(component->userData, 0, Enemy*);

        switch (subject)
        {
            case ComponentMsg_OnEnter:
            {
                switch (component->curState->id)
                {
                    case EnemyState_Stand:
                        if (component->preState->id != EnemyState_DieOver)
                        {
                            ASkeletonAnimationPlayer->SetAnimationMix(enemy->player, "stand", 0.2f);
                        }
                        else
                        {
                            ASkeletonAnimationPlayer->SetAnimation(enemy->player, "stand");
                        }

                        enemy->dizzyEffect->loop = 0;
                        break;

                    case EnemyState_Walk:
                        ASkeletonAnimationPlayer->SetAnimationMix(enemy->player, "walk", 0.2f);
                        enemy->dizzyEffect->loop = 0;
                        break;

                    case EnemyState_Hurt:
                        ASkeletonAnimationPlayer->SetAnimation(enemy->player, "hurt");
                        AAudioTool->Play(AudioID_HurtEnemy);
                        break;

                    case EnemyState_Attack:
                        ASkeletonAnimationPlayer->SetAnimationMix(enemy->player, "attack", 0.1f);
                        if (enemy->attackBoxBody != NULL)
                        {
                            enemy->attackBoxBody->state = PhysicsBodyState_Fixed;
                        }
                        break;

                    case EnemyState_Die:
                        ASkeletonAnimationPlayer->SetAnimationMix(enemy->player, "die", 0.2f);
                        AAudioTool->Play(AudioID_HurtEnemy);
                        AHero->RoundScore();
                        break;

                    case EnemyState_DieOver:
                    {
                        ASkeletonAnimationPlayer->SetAnimation(enemy->player, "die_over");
                        enemy->dizzyEffect->loop = 0;

                        FontText* text         = AFont->GetText(AGameActor->hpFont);
                        Drawable* heroDrawable = AEnemy_GetDrawable(enemy);

                        AFont->SetString   (text, "_");
                        ADrawable_SetScale2(text->drawable, 0.0f, 0.0f);
                        ADrawable_SetParent(text->drawable, AGameMap->beforeDrawable);

                        float    offsetX   = AGLTool_ToGLWidth(20.0f);
                        Vector2 localA[1] = {{heroDrawable->scaleX > 0 ? -offsetX : offsetX, 0.0f}};
                        Vector2 localB[1];

                        ADrawable->ConvertBetweenLocalPositionV2
                        (
                           heroDrawable,
                           localA,
                           AGameMap->beforeDrawable,
                           localB
                        );

                        ADrawable_SetPosition2(text->drawable, localB->x, localB->y);

                        ATweenTool->AddAction     ()

                                  ->SetFadeTo     (0.0f)
                                  ->SetRelative   (false)
                                  ->SetEaseType   (TweenEaseType_Linear)

                                  ->SetMoveY      (0.1f)
                                  ->SetEaseType   (TweenEaseType_SineOut)

                                  ->SetScaleX     (1.0f)
                                  ->SetRelative   (false)
                                  ->SetEaseType   (TweenEaseType_QuinticOut)

                                  ->SetScaleY     (1.0f)
                                  ->SetRelative   (false)
                                  ->SetEaseType   (TweenEaseType_ExponentialOut)

                                  ->SetDuration   (1.4f)
                                  ->SetOnComplete (HurtTextTweenActionOnComplete)
                                  ->SetUserDataPtr(0, text)

                                  ->RunActions    (text->drawable);

                        break;
                    }

                    case EnemyState_Dizzy:
                    {
                        ADrawable_SetVisible(enemy->dizzyEffect->skeleton->drawable);
                        enemy->dizzyEffect->loop = -1;
                        enemy->dizzyTime         = AGameData->dizzyTime;

                        if (AMath_Random() > 0.5f)
                        {
                            ASkeleton->SetSkin(enemy->dizzyEffect->skeleton, "star");
                        }
                        else
                        {
                            ASkeleton->SetSkin(enemy->dizzyEffect->skeleton, "duck");
                        }

                        ADrawable_SetPosition2
                        (
                            enemy->dizzyEffect->skeleton->drawable,
                            AGLTool_ToGLWidth(enemy->dizzyX),
                            AGLTool_ToGLHeight(enemy->dizzyY)
                        );

                        ASkeletonAnimationPlayer->SetAnimation(enemy->player, "dizzy");

                        if (enemy->attackBoxBody != NULL)
                        {
                            enemy->attackBoxBody->state = PhysicsBodyState_Freeze;
                        }
                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            case ComponentMsg_OnExit:
            {
                switch (component->curState->id)
                {
                    case EnemyState_Attack:
                        if (enemy->attackBoxBody != NULL)
                        {
                            enemy->attackBoxBody->state = PhysicsBodyState_Freeze;
                        }
                        break;

                    default:
                        break;
                }

                break;
            }

            default:
                break;
        }
    }

    return false;
}


static Enemy* Create(const char* filePath, float bornX, float bornY)
{
    Enemy* enemy = malloc(sizeof(Enemy));
    AComponent->Init(enemy->component);

    ASkeletonAnimationPlayer->Init(filePath,         "walk",      enemy->player);
    ASkeletonAnimationPlayer->Init("Animation/hit", "animation", enemy->hurtEffect);
    enemy->hurtEffect->loop = 0;
    ASkeleton->SetSkin(enemy->hurtEffect->skeleton, "normal");
    ADrawable_SetParent(ASkeletonAnimationPlayer_GetDrawable(enemy->hurtEffect), AGameMap->beforeDrawable);

    ASkeletonAnimationPlayer->Init("Animation/dizzy-star", "animation", enemy->dizzyEffect);
    enemy->dizzyEffect->loop = 0;
    Drawable* dizzyEffectDrawable  = ASkeletonAnimationPlayer_GetDrawable(enemy->dizzyEffect);
    ADrawable_SetParent   (dizzyEffectDrawable, AEnemy_GetDrawable(enemy));
    ADrawable_SetInvisible(dizzyEffectDrawable);

    AArrayIntSet->Init(enemy->arrowSet);

    enemy->belongAI             = NULL;
    enemy->belongAIAction       = NULL;
    enemy->attackBoxBody        = NULL;
    enemy->attackBoxDrawable    = NULL;
    enemy->collisionBoxBody     = NULL;
    enemy->collisionBoxDrawable = NULL;

    AUserData_SetSlotPtr(enemy->component->userData, 0, enemy);

    enemy->hp                   = 0;
    enemy->speed                = 0.0f;
    enemy->dizzyTime            = 0;
    enemy->dizzyY               = 0.0f;
    enemy->dizzyX               = 0.0f;
    enemy->hitDis               = 0;

    enemy->dieXTime             = 0.0f;
    enemy->dieYUpTime           = 0.0f;
    enemy->dieYDownTime         = 0.0f;
    enemy->dieMoveYEaseType     = TweenEaseType_CubicOut;
    enemy->dieMoveXEaseType     = TweenEaseType_CubicOut;

    enemy->hurtXTime            = 0.0f;
    enemy->hurtYUpTime          = 0.0f;
    enemy->hurtYDownTime        = 0.0f;

    enemy->hurtMoveX            = 0.01f;
    enemy->hurtMoveY            = 0.01f;

    enemy->dieMoveX             = 0.01f;
    enemy->dieMoveY             = 0.01f;

    enemy->isWoodStop           = false;
    enemy->type                 = EnemyType_NULL;
    enemy->isBoss               = false;

    Drawable* drawable          = AEnemy_GetDrawable(enemy);
    ADrawable_SetParent   (drawable, AGameMap->beforeDrawable);
    ADrawable_SetPosition2(drawable, bornX, bornY);


    SkeletonSlot* collisionSlot = ASkeletonAnimationPlayer_GetSlot(enemy->player, "CollisionBox");
    if (collisionSlot != NULL)
    {
        enemy->collisionBoxDrawable          = collisionSlot->bone->drawable;
        enemy->collisionBoxBody              = APhysicsWorld->AddBody
                                               (
                                                   PhysicsShape_Polygon,
                                                   ASkeletonSlot_GetBoundingBox(collisionSlot)->vertexArr
                                               );
        enemy->collisionBoxBody->OnCollision = OnHurt;
        enemy->collisionBoxBody->state       = PhysicsBodyState_Fixed;

        AUserData_SetSlotPtr(enemy->collisionBoxBody->userData, 0, enemy);
        APhysicsBody_SetCollisionGroup
        (
            enemy->collisionBoxBody,
            CollisionGroup_EnemyBody | CollisionGroup_EnemyAttack
        );
        ATool->InitBox(enemy->collisionBoxBody, 2.0f, 2.0f);

        #ifdef APP_DEBUG
        ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable
        (
            enemy->player,
            "CollisionBox",
            enemy->debugCollisionBoundingBox
        );
        #endif
    }

    SkeletonSlot* attackSlot = ASkeletonAnimationPlayer_GetSlot(enemy->player, "AttackBox");
    if (attackSlot != NULL)
    {
        enemy->attackBoxDrawable    = attackSlot->bone->drawable;
        enemy->attackBoxBody        = APhysicsWorld->AddBody
                                      (
                                          PhysicsShape_Polygon,
                                          ASkeletonSlot_GetBoundingBox(attackSlot)->vertexArr
                                      );
        enemy->attackBoxBody->state = PhysicsBodyState_Fixed;

        AUserData_SetSlotPtr(enemy->attackBoxBody->userData, 0, enemy);
        APhysicsBody_SetCollisionGroup
        (
            enemy->attackBoxBody,
            CollisionGroup_EnemyAttack | CollisionGroup_ArrowAttack
        );
        ATool->InitBox(enemy->attackBoxBody, 2.0f, 2.0f);

        #ifdef APP_DEBUG
        ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable
        (
             enemy->player,
             "AttackBox",
             enemy->debugAttackBoundingBox
        );
        #endif
    }

    if (AEnemyAI->isInit == false)
    {
        AComponent->AddObserver(AHero->component, enemy->component);
    }

    enemy->component->stateMap->elementList->increase = 10;
    enemy->player->OnActionOver                       = OnActionOver;

    AComponent->AddState(enemy->component, EnemyState_Stand,   OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_Walk,    OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_Hurt,    OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_Attack,  OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_Die,     OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_DieOver, OnMessage, Update);
    AComponent->AddState(enemy->component, EnemyState_Dizzy,   OnMessage, Update);

    AComponent->SetState   (enemy->component,      EnemyState_Stand);
    AComponent->AppendChild(AGameActor->component, enemy->component);

    return enemy;
}


struct AEnemy AEnemy[1] =
{{
    Create,
    Reset,
    Destroy,
}};
