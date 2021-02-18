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


#ifndef HERO_H
#define HERO_H


#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Application/Component.h"
#include "Engine/Graphics/OpenGL/Sprite.h"

#include "Enemy.h"
#include "Config.h"


enum
{
    HeroState_Stand = 1,
    HeroState_Walk,
    HeroState_Shoot,
    HeroState_Shoot_Fast,
    HeroState_Ready,
    HeroState_Keep,
    HeroState_Hurt,
    HeroState_Dizzy,
    HeroState_Die,
    HeroState_DieOver,
};


enum
{
    CollisionGroup_HeroBody    = 1,
    CollisionGroup_HeroAttack  = 1 << 1,
    CollisionGroup_EnemyBody   = 1 << 2,
    CollisionGroup_EnemyAttack = 1 << 3,
    CollisionGroup_ArrowAttack = 1 << 4,
};


enum
{
    CollisionBodyID_Heart,
    CollisionBodyID_Coin,
    CollisionBodyID_Stone,
    CollisionBodyID_Wood,
};


enum
{
    HeroNotify_Dizzy,
    HeroNotify_Die,
};


typedef enum
{
    ArrowHitType_Enemy,
    ArrowHitType_Ground,
    ArrowHitType_Null,
}
ArrowHitType;


enum
{
    ArrowTail_PosCount = 4,
    ArrowTail_PosLen   = ArrowTail_PosCount << 1,
};


typedef struct
{
    Sprite       sprite[1];
    PhysicsBody* body;
    Enemy*       enemy;
    ArrowHitType hitType;
    float        deformX;
    float        deformU;
    float        halfWidth;

    float        tailPos[ArrowTail_PosLen];
    int          tailPosCurCount;
    float        tailPosCurTime;
    Drawable     tailDrawable[1];
}
Arrow;


struct AHero
{
    Component               component[1];
    SkeletonAnimationPlayer player   [1];
    SkeletonAnimationPlayer hitFloor [1];

    int                     roundHP;
    int                     roundEnergy;
    int                     roundKillCount;
    int                     roundArrowCount;
    int                     roundMissCount;
    int                     roundCount;
    int                     roundScore;
    float                   roundTime;

    Drawable*               bowHandDrawable;
    PhysicsBody*            collisionBoxBody;
    Drawable*               collisionBoxDrawable;

    void (*Init)      (void);
    void (*Run)       (void);
    void (*Revive)    (void);
    void (*ResetArrow)(Arrow* arrow);
    void (*RoundScore)(void);
};


extern struct AHero AHero[1];


static inline Drawable* AHero_GetDrawable()
{
    return ASkeletonAnimationPlayer_GetDrawable(AHero->player);
}


#endif
