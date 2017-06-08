/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-4-29
 * Author : scott.cgi
 * Version: 0.1
 */


#include <stddef.h>
#include <string.h>

#include "Engine/Toolkit/Head/UserData.h"
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


static ArrayIntSet woodSet[1];


static ArrayIntSet* sets[] =
{
	woodSet,
};


typedef enum
{
	AIResult_Skip,
	AIResult_Move,
}
AIResult;


typedef AIResult (*AIFunc)(Enemy* enemy, Drawable* enemyDrawable, Drawable* heroDrawable, float deltaX);


static void DoAI(ArrayIntSet* enemySet, AIFunc AI)
{
	for (int i = 0; i < enemySet->elementList->size; i++)
	{
		Enemy*    enemy         = AArrayList_Get(enemySet->elementList, i, Enemy*);
		Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);
		Drawable* heroDrawable  = AHero_GetDrawable();
		float     deltaX        = enemyDrawable->positionX - ADrawable->ConvertBetweenLocalX
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

		if (deltaX > 0.0f)
		{
			ADrawable_SetScaleX(enemyDrawable,  1.0f);
		}
		else
		{
			ADrawable_SetScaleX(enemyDrawable, -1.0f);
		}

        deltaX          = -deltaX;
        AIResult result = AIResult_Skip;

		if (enemy->isWoodStop == false)
		{
			result = AI(enemy, enemyDrawable, heroDrawable, deltaX);
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

			if (AMath_Random() > AMath_Random())
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
	Enemy* enemy = (Enemy*) action->userData->slot0->ptrValue;
	if (enemy != NULL)
	{
		AComponent->SetState(enemy->component, EnemyState_Attack);
	}
}


static AIResult WoodAI(Enemy* enemy, Drawable* enemyDrawable, Drawable* heroDrawable, float deltaX)
{
	if (deltaX > -0.35f && deltaX < 0.35f && AMath_Random() > AMath_Random())
	{
		ATween->TryRemoveAllActions(enemyDrawable);

		if (AMath_Random() > AMath_Random())
		{
			ATweenTool->AddMoveX       (deltaX * 0.98f, 0.35f)
					  ->SetEaseType    (TweenEaseType_QuadraticIn)
					  ->SetQueue       (false)
					  ->SetOnComplete  (AttackOnComplete)
					  ->SetUserData0Ptr(enemy)

					  ->AddMoveY       (0.035f, 0.25f)
					  ->SetEaseType    (TweenEaseType_QuadraticIn)

					  ->AddMoveY       (-0.035f, 0.15f)
					  ->SetEaseType    (TweenEaseType_QuadraticOut)

					  ->RunActions     (enemyDrawable);
		}
		else
		{
			ATweenTool->AddMoveX       (deltaX * 0.98f, 0.35f)
					  ->SetEaseType    (TweenEaseType_QuadraticOut)
					  ->SetQueue       (false)
					  ->SetOnComplete  (AttackOnComplete)
					  ->SetUserData0Ptr(enemy)

                      ->AddMoveY       (AGameMap->groundY, 0.15f)
                      ->SetEaseType    (TweenEaseType_QuadraticOut)
                      ->SetRelative    (false)
					  ->RunActions     (enemyDrawable);
		}

		return AIResult_Move;
	}

	return AIResult_Skip;
}


//--------------------------------------------------------------------------------------------------


static AIFunc AIFuncs[] =
{
	WoodAI,
};


static void EnemyAI(Scheduler* scheduler, float deltaSeconds)
{
    if (AHero->component->curState->id >= HeroState_Die)
    {
        scheduler->isCancel = true;
    }
    else
    {
        for (int i = 0; i < ENEMY_KINDS_COUNT; i++)
        {
            DoAI(sets[i], AIFuncs[i]);
        }
    }
}


//--------------------------------------------------------------------------------------------------


static inline Enemy* CreateAndCache(char* filePath, float bornX, float bornY)
{
    Enemy* enemy = NULL;

    if (AEnemyAI->isInit == false)
    {
        for (int i = 0; i < AEnemyAI->enemyDeadList->size; i++)
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
	int    increase      = AHero->roundKillCount / KILL_INCREASE_SCALE + 1;

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

	return enemy;
}


//--------------------------------------------------------------------------------------------------


typedef Enemy* (*Create)(float x);


static Create creates[] =
{
	CreateWood,
};


//--------------------------------------------------------------------------------------------------


static void CreateEnemy(Scheduler* scheduler, float deltaSeconds)
{
    if (AHero->component->curState->id >= HeroState_Die)
    {
        scheduler->isCancel = true;
    }
	else if
	(
        AEnemyAI->currentEnemyNum <
        AHero->roundKillCount / KILL_INCREASE_SCALE + (int) AHero->roundTime / TIME_INCREASE_SCALE + 2
	)
	{
		float pos;

		if (AMath_Random() > 0.5f)
		{
			pos =  AGLTool->screenRatio + 0.2f;
		}
		else
		{
			pos = -AGLTool->screenRatio - 0.2f;
		}

		float  x        = ADrawable->ConvertToLocalX(AGameMap->beforeDrawable, pos);
		int    n        = AMath_RandomInt(0, ENEMY_KINDS_COUNT - 1);
		Enemy* enemy    = creates[n](x);
		enemy->belongAI = sets[n];

		AArrayIntSet->TryAdd(sets[n], (intptr_t) enemy);
        AEnemyAI->currentEnemyNum++;
	}
}


static void CreateCache(int kind)
{
    Enemy* enemy = creates[kind](-2.0f);

    AArrayList_Add
    (
        AEnemyAI->enemyDeadList,
        enemy
    );

    AComponent->SetActive(enemy->component, false);
}


static void Init()
{
    AEnemyAI->currentEnemyNum = 0;
    AEnemyAI->isInit          = true;

	AArrayList->Init(sizeof(Enemy*), AEnemyAI->enemyDeadList);

	for (int i = 0; i < ENEMY_KINDS_COUNT; i++)
	{
		AArrayIntSet->Init(sets[i]);
	}
}


static void Run()
{
    AEnemyAI->isInit = false;
    AScheduler->Schedule(EnemyAI,     1.0f);
    AScheduler->Schedule(CreateEnemy, 0.5f);
}


static float moveDis;


static void SetAllEnemy(int enemyState, float enemyMoveDis)
{
    moveDis = enemyMoveDis;

    for (int i = 0; i < ENEMY_KINDS_COUNT; i++)
    {
        ArrayIntSet* enemySet = sets[i];

        for (int j = 0; j < enemySet->elementList->size; j++)
        {
            Enemy* enemy = AArrayList_Get(enemySet->elementList, j, Enemy*);

            if (enemy->component->curState->id >= EnemyState_Die)
            {
                continue;
            }


            Drawable* enemyDrawable = AEnemy_GetDrawable(enemy);
            Drawable* heroDrawable  = AHero_GetDrawable();
            float     deltaX        = enemyDrawable->positionX - ADrawable->ConvertBetweenLocalX
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
                ADrawable_SetScaleX(enemyDrawable,  1.0f);
            }
            else
            {
                dis = -moveDis;
                ADrawable_SetScaleX(enemyDrawable, -1.0f);
            }

            ATween->TryRemoveAllActions(enemyDrawable);

            if (enemyState != EnemyState_Null)
            {
                AComponent->SetState(enemy->component, enemyState);
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
    for (int i = 0; i < ENEMY_KINDS_COUNT; i++)
    {
        ArrayIntSet* enemySet = sets[i];

        while (enemySet->elementList->size > 0)
        {
            AEnemy->Delete(AArrayList_Get(enemySet->elementList, 0, Enemy*));
        }
    }

    Run();
}


struct AEnemyAI AEnemyAI[1] =
{
	{
		.Init        = Init,
        .Run         = Run,
		.CreateCache = CreateCache,
        .SetAllEnemy = SetAllEnemy,
        .Restart     = Restart,
 	}
};
