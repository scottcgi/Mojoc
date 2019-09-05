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


typedef enum
{
    EnemyType_Wood,
    EnemyType_Pig,
    EnemyType_Stone,
    EnemyType_Wolf,
    EnemyType_Bear,
    EnemyType_Snake,
    EnemyType_Grass,
    EnemyType_Penguin,
    EnemyType_SnowBall,
    EnemyType_BigSnowMan,
    EnemyType_Fly,
    EnemyType_NULL,
}
EnemyType;


typedef struct
{
    EnemyType               type;

    Component               component  [1];
    SkeletonAnimationPlayer player     [1];
    SkeletonAnimationPlayer hurtEffect [1];
    SkeletonAnimationPlayer dizzyEffect[1];

    ArrayIntSet(Arrow*)     arrowSet   [1];
    ArrayIntSet(Enemy*)*    belongAI;
    TweenAction*            belongAIAction;

    int                     hp;
    float                   speed;
    int                     dizzyTime;

    float                   dieXTime;
    float                   dieYUpTime;
    float                   dieYDownTime;

    float                   hurtXTime;
    float                   hurtYUpTime;
    float                   hurtYDownTime;

    float                   hurtMoveX;
    float                   hurtMoveY;

    float                   dieMoveX;
    float                   dieMoveY;
    TweenEaseType           dieMoveYEaseType;
    TweenEaseType           dieMoveXEaseType;

    float                   dizzyY;
    float                   dizzyX;

    float                   hitDis;
    bool                    isWoodStop;
    bool                    isBoss;

    PhysicsBody*            attackBoxBody;
    Drawable*               attackBoxDrawable;
    PhysicsBody*            collisionBoxBody;
    Drawable*               collisionBoxDrawable;

    #ifdef APP_DEBUG
    Drawable                debugAttackBoundingBox   [1];
    Drawable                debugCollisionBoundingBox[1];
    #endif
}
Enemy;



struct AEnemy
{
    /**
     * filePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    Enemy* (*Create) (const char* filePath, float bornX, float bornY);
    void   (*Reset)  (Enemy*      enemy,    float bornX, float bornY);
    void   (*Destroy)(Enemy*      enemy);
};


extern struct AEnemy AEnemy[1];


static inline Drawable* AEnemy_GetDrawable(Enemy* enemy)
{
    return ASkeletonAnimationPlayer_GetDrawable(enemy->player);
}


static inline const char* AEnemy_GetPath(Enemy* enemy)
{
    return ASkeletonAnimationPlayer_GetPath(enemy->player);
}


#endif
