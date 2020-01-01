/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2015-4-29
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include <stddef.h>
#include <string.h>

#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Toolkit/Platform/Log.h"

#include "EnemyAI.h"
#include "Hero.h"
#include "GameMap.h"
#include "Enemy.h"
#include "GameData.h"


#define AskGod() \
    (AMath_Random() > AMath_Random())


static ArrayIntSet woodSet       [1];


static ArrayIntSet* enemySets[Enemy_KindsNum] =
{
    woodSet,
};


typedef enum
{
    AIResult_Skip,
    AIResult_Move,
}
AIResult;


typedef AIResult (*EnemyAI)(Enemy* enemy, Drawable* enemyDrawable, Drawable* heroDrawable, float deltaX);


static void DoEnemySetAI(ArrayIntSet* enemySet, EnemyAI enemyAI)
{
    for (int i = 0; i < enemySet->elementList->size; ++i)
    {
        Enemy*    enemy         = AArrayList_Get(enemySet->elementList, i, Enemy*);
        Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);
        Drawable* heroDrawable  = AHero_GetDrawable();
        float     deltaX        = enemyDrawable->positionX - ADrawable->ConvertBetweenLocalPositionX
                                                             (
                                                                 heroDrawable->parent,
                                                                 heroDrawable->positionX,
                                                                 enemyDrawable->parent
                                                             );

        if (enemy->component->curState->id >= EnemyState_Dizzy)
        {
            continue;
        }
        else if (AHero->component->curState->id >= HeroState_Die)
        {
            continue;
        }


        if (deltaX * enemyDrawable->scaleX < 0)
        {
            ADrawable_SetScaleX(enemyDrawable, -enemyDrawable->scaleX);
        }

        deltaX          = -deltaX;
        AIResult result = AIResult_Skip;

        if (enemy->isWoodStop == false)
        {
            result = enemyAI(enemy, enemyDrawable, heroDrawable, deltaX);
        }

        if (deltaX > -enemy->hitDis && deltaX < enemy->hitDis)
        {
            ATween->TryRemoveAction(enemyDrawable, enemy->belongAIAction);
            AComponent->SetState(enemy->component, EnemyState_Attack);
            return;
        }

        if (result == AIResult_Move)
        {
            return;
        }

        if (enemy->component->curState->id != EnemyState_Walk)
        {
            AComponent->SetState(enemy->component, EnemyState_Walk);

            if (AskGod())
            {
                return;
            }
        }

        if (enemy->isWoodStop)
        {
            return;
        }

        ATween->TryRemoveAction(enemyDrawable, enemy->belongAIAction);

        ATweenTool->AddMoveX   (deltaX, fabsf(deltaX) / enemy->speed)
                  ->GetAction  (&enemy->belongAIAction)
                  ->SetEaseType(TweenEaseType_Linear)
                  ->RunActions (enemyDrawable);
    }
}


static void AttackOnComplete(TweenAction* action)
{
    Enemy* enemy = AUserData_GetSlotPtrWithType(action->userData, 0, Enemy*);

    if (enemy != NULL)
    {
        AComponent->SetState(enemy->component, EnemyState_Attack);
    }
}


static AIResult WoodAI(Enemy* enemy, Drawable* enemyDrawable, Drawable* heroDrawable, float deltaX)
{
    if (deltaX > -0.35f && deltaX < 0.35f && AskGod())
    {
        ATween->TryRemoveAllActions(enemyDrawable);

        if (AskGod())
        {
            ATweenTool->AddMoveX      (deltaX * 0.98f, 0.35f)
                      ->SetEaseType   (AskGod() ? TweenEaseType_QuadraticIn : TweenEaseType_CubicIn)
                      ->SetQueue      (false)
                      ->SetOnComplete (AttackOnComplete)
                      ->SetUserDataPtr(0, enemy)

                      ->AddMoveY      (0.04f, 0.25f)
                      ->SetEaseType   (AskGod() ? TweenEaseType_QuadraticIn : TweenEaseType_CubicIn)

                      ->AddMoveY      (AGameMap->groundY, 0.15f)
                      ->SetEaseType   (TweenEaseType_QuadraticOut)
                      ->SetRelative   (false)

                      ->RunActions    (enemyDrawable);
        }
        else
        {
            ATweenTool->AddMoveX      (deltaX * 0.98f, 0.35f)
                      ->SetEaseType   (AskGod() ? TweenEaseType_QuadraticOut : TweenEaseType_CubicOut)
                      ->SetQueue      (false)
                      ->SetOnComplete (AttackOnComplete)
                      ->SetUserDataPtr(0, enemy)

                      ->AddMoveY      (AGameMap->groundY, 0.15f)
                      ->SetEaseType   (AskGod() ? TweenEaseType_QuadraticOut : TweenEaseType_CubicOut)
                      ->SetRelative   (false)
                      ->RunActions    (enemyDrawable);
        }

        return AIResult_Move;
    }

    return AIResult_Skip;
}


//----------------------------------------------------------------------------------------------------------------------


static EnemyAI enemyAIs[Enemy_KindsNum] =
{
    WoodAI,
};


static void DoEnemyAI(Scheduler* scheduler, float deltaSeconds)
{
    if (AHero->component->curState->id >= HeroState_Die)
    {
        scheduler->isCancel = true;
    }
    else
    {
        for (int i = 0; i < Enemy_KindsNum; ++i)
        {
            DoEnemySetAI(enemySets[i], enemyAIs[i]);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------


static inline void RandomSmallEnemey(Enemy* enemy)
{
    if (AMath_Random() < 0.2f)
    {
        Drawable* drawable = AEnemy_GetDrawable(enemy);
        ADrawable_SetScaleSame2(drawable, 0.7f);
    }
}



static inline Enemy* CreateAndCache(const char* filePath, float bornX, float bornY)
{
    Enemy* enemy = NULL;

    if (AEnemyAI->isInit == false)
    {
        for (int i = 0; i < AEnemyAI->enemyDeadList->size; ++i)
        {
            Enemy* e = AArrayList_Get(AEnemyAI->enemyDeadList, i, Enemy*);

            if (strcmp(filePath, AEnemy_GetPath(e)) == 0)
            {
                enemy = e;
                AArrayList->Remove(AEnemyAI->enemyDeadList, i);
                break;
            }
        }
    }

    if (enemy == NULL)
    {
        enemy = AEnemy->Create(filePath, bornX, bornY);
    }
    else
    {
        AEnemy->Reset(enemy, bornX, bornY);
    }

    return enemy;
}


static Enemy* CreateWood(float x)
{
    Enemy* enemy         = CreateAndCache("Animation/wood", x, AGameMap->groundY);
    int    increase      = AHero->roundKillCount / Enemy_KillIncrease + 1;

    enemy->hp            = increase;
    enemy->speed         = AGLTool_ToGLWidth(35 + AMath_RandomInt(increase, increase * 3));
    enemy->dizzyY        = 50.0f;

    enemy->hitDis        = AGLTool_ToGLWidth(30);

    enemy->dieXTime      = 1.25f;
    enemy->dieYUpTime    = 0.35f;
    enemy->dieYDownTime  = 0.25f;

    enemy->hurtXTime     = 0.35f;
    enemy->hurtYUpTime   = 0.25f;
    enemy->hurtYDownTime = 0.15f;

    enemy->hurtMoveX     = 0.05f * AGameData->arrowPower;
    enemy->hurtMoveY     = 0.04f;

    enemy->dieMoveX      = 0.15f;
    enemy->dieMoveY      = 0.05f;

    enemy->type          = EnemyType_Wood;

    RandomSmallEnemey(enemy);

    return enemy;
}


//----------------------------------------------------------------------------------------------------------------------


typedef Enemy* (*Create)(float x);


static Create Creates[Enemy_KindsNum] =
{
    CreateWood,
};


//----------------------------------------------------------------------------------------------------------------------


static ArrayList(int) kinds[1]   = AArayList_InitFixed(int, Enemy_KindsNum, 0);
static int            eachKindNum;
static int            kindIndex;
static int            createNum;


static void CreateEnemy(Scheduler* scheduler, float deltaSeconds)
{
    if (AHero->component->curState->id >= HeroState_Die)
    {
        scheduler->isCancel = true;
    }
    else
    {
        int num1 =       AHero->roundKillCount /  Enemy_KillIncrease;
        int num2 = (int) AHero->roundTime      / (Enemy_TimeIncrease + num1 * 3) + 2;
        int num3 = AEnemyAI->isHasBoss ? num2  / 2 : num2;

        if (AEnemyAI->currentEnemyNum < num3)
        {
            float pos = AMath_RandomFloat(0.2f, 0.3f);

            if (AMath_Random() > 0.5f)
            {
                pos =  AGLTool->screenRatio + pos;
            }
            else
            {
                pos = -AGLTool->screenRatio - pos;
            }

            if (kinds->size > 0)
            {
                if (eachKindNum == Enemy_EachKindInitNum)
                {
                    // random one kind of enemy
                    int index = AMath_RandomInt(0, kinds->size - 1);

                    kindIndex = AArrayList_Get(kinds, index, int);

                    AArrayList->Remove(kinds, index);
                }

                --eachKindNum;

                if (eachKindNum == 0)
                {
                    // one kind of enemy show over
                    eachKindNum = Enemy_EachKindInitNum;
                }
            }
            else
            {
                // random enemy after all kinds of enemy showed
                kindIndex = AMath_RandomInt(0, Enemy_KindsNum - 1);
            }

            float  x        = ADrawable->ConvertToLocalPositionX(AGameMap->beforeDrawable, pos);
            Enemy* enemy    = Creates  [kindIndex](x);
            enemy->belongAI = enemySets[kindIndex];

            AArrayIntSet->TryAdd(enemySets[kindIndex], (intptr_t) enemy);
            ++AEnemyAI->currentEnemyNum;
            ++createNum;

            if (createNum % Enemy_BossInterval == 0)
            {
                enemy->isBoss       = true;
                enemy->hp          *= Enemy_BossHPX;
                enemy->speed       *= Enemy_BossSpeedX;
                AEnemyAI->isHasBoss = true;
                ADrawable_SetScaleSame2(AEnemy_GetDrawable(enemy), 1.3f);
            }
        }
    }
}


static void CreateCache(int kind)
{
    Enemy* enemy = Creates[kind](-2.0f);

    AArrayList_Add(AEnemyAI->enemyDeadList, enemy);
    AComponent->SetActive(enemy->component, false);
}


static void Init()
{
    AEnemyAI->currentEnemyNum = 0;
    AEnemyAI->isInit          = true;
    createNum                 = 0;

    AArrayList->Init(sizeof(Enemy*), AEnemyAI->enemyDeadList);

    for (int i = 0; i < Enemy_KindsNum; ++i)
    {
        AArrayIntSet->Init(enemySets[i]);
    }
}


static void Run()
{
    AEnemyAI->isInit    = false;
    AEnemyAI->isHasBoss = false;
    eachKindNum         = Enemy_EachKindInitNum;
    createNum           = 0;

    AArrayList->Clear(kinds);
    for (int i = 0; i < Enemy_KindsNum; ++i)
    {
        AArrayList_Add(kinds, i);
    }

    #ifndef APP_NO_ENEMY
    AScheduler->Schedule(DoEnemyAI,   enemyDoAITime);
    AScheduler->Schedule(CreateEnemy, enemyCreateTime);
    #endif
}


static void SetAllEnemy(int enemyStateID, float enemyMoveDis)
{
    float moveDis = enemyMoveDis;

    for (int i = 0; i < Enemy_KindsNum; ++i)
    {
        ArrayIntSet* enemySet = enemySets[i];

        for (int j = 0; j < enemySet->elementList->size; ++j)
        {
            Enemy* enemy = AArrayList_Get(enemySet->elementList, j, Enemy*);

            if (enemy->component->curState->id >= EnemyState_Die)
            {
                continue;
            }

            Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);
            Drawable* heroDrawable  = AHero_GetDrawable();
            float     deltaX        = enemyDrawable->positionX - ADrawable->ConvertBetweenLocalPositionX
                                                                 (
                                                                    heroDrawable->parent,
                                                                    heroDrawable->positionX,
                                                                    enemyDrawable->parent
                                                                 );
            moveDis += enemyMoveDis;

            float dis;

            if (deltaX > 0.0f)
            {
                dis = moveDis;
            }
            else
            {
                dis = -moveDis;
            }

            if (deltaX * enemyDrawable->scaleX < 0)
            {
                ADrawable_SetScaleX(enemyDrawable, -enemyDrawable->scaleX);
            }

            ATween->TryRemoveAllActions(enemyDrawable);

            if (enemyStateID != EnemyState_Null)
            {
                AComponent->SetState(enemy->component, enemyStateID);
            }

            ATweenTool->AddMoveX   (dis, moveDis / enemy->speed)
                      ->SetQueue   (false)
                      ->SetEaseType(TweenEaseType_ExponentialOut)
                      ->RunActions (enemyDrawable);
        }
    }
}


static void Restart()
{
    for (int i = 0; i < Enemy_KindsNum; ++i)
    {
        ArrayIntSet* enemySet = enemySets[i];

        while (enemySet->elementList->size > 0)
        {
            AEnemy->Destroy(AArrayList_Get(enemySet->elementList, 0, Enemy*));
        }
    }

    Run();
}


struct AEnemyAI AEnemyAI[1] =
{{
    .Init        = Init,
    .Run         = Run,
    .CreateCache = CreateCache,
    .SetAllEnemy = SetAllEnemy,
    .Restart     = Restart,
}};


#undef AskGod