/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2015-9-29
 * Author: scott.cgi
 */

#ifndef ENEMY_H
#define ENEMY_H


#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Application/Component.h"
#include "Engine/Toolkit/Utils/Tween.h"
#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Config.h"


enum
{
    EnemyState_Null,
    EnemyState_Stand,
    EnemyState_Walk,
    EnemyState_Hurt,
    EnemyState_Attack,
    EnemyState_Dizzy,
    EnemyState_Die,
    EnemyState_DieOver,
    EnemyState_Length,
};


typedef struct
{
    Component                     component  [1];
    SkeletonAnimationPlayer       player     [1];
    SkeletonAnimationPlayer       hurtEffect [1];
    SkeletonAnimationPlayer       dizzyEffect[1];
    ArrayIntSet(Arrow*)           arrowSet   [1];
    ArrayIntSet(Enemy*)*          belongAI;
    TweenAction*                  belongAIAction;

    int                           hp;
    float                         speed;
    int                           dizzyTime;

    float                         dieXTime;
    float                         dieYUpTime;
    float                         dieYDownTime;

    float                         hurtXTime;
    float                         hurtYUpTime;
    float                         hurtYDownTime;

    float                         hurtMoveX;
    float                         hurtMoveY;

    float                         dieMoveX;
    float                         dieMoveY;
    TweenEaseType                 dieMoveYEaseType;
    TweenEaseType                 dieMoveXEaseType;

    float                         dizzyY;
    float                         dizzyX;

    float                         hitDis;
    bool                          isWoodStop;

    PhysicsBody*                  attackBoxBody;
    Drawable*                     attackBoxDrawable;
    PhysicsBody*                  collisionBoxBody;
    Drawable*                     collisionBoxDrawable;

    #ifdef APP_DEBUG
    Drawable                      debugAttackBoundingBox   [1];
    Drawable                      debugCollisionBoundingBox[1];
    #endif
}
Enemy;


struct AEnemy
{
    Enemy* (*Create)(char*  filePath, float bornX, float bornY);
    void   (*Reset) (Enemy* enemy,    float bornX, float bornY);
    void   (*Delete)(Enemy* enemy);
};


extern struct AEnemy AEnemy[1];


static inline Drawable* AEnemy_GetDrawable(Enemy* enemy)
{
    return ASkeletonAnimationPlayer_GetDrawable(enemy->player);
}


static inline char* AEnemy_GetPath(Enemy* enemy)
{
    return ASkeletonAnimationPlayer_GetPath(enemy->player);
}


#endif
