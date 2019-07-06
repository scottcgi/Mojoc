/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-9-17
 * Author : scott.cgi
 */

#ifndef HERO_H
#define HERO_H


#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Engine/Application/Component.h"
#include "Engine/Extension/Particle/ParticleEmitter.h"
#include "Engine/Graphics/OpenGL/Sprite.h"

#include "Enemy.h"
#include "Config.h"


enum
{
    HeroState_Stand = 1,
    HeroState_Walk,
    HeroState_Shoot,
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
    CollisionGroup_Attack      = 1 << 4,
};


enum
{
    CollisionBodyId_Heart,
    CollisionBodyId_Coin,
    CollisionBodyId_Stone,
    CollisionBodyId_Wood,
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


typedef struct
{
    Sprite       sprite[1];
    PhysicsBody* body;
    Enemy*       enemy;
    ArrowHitType hitType;

    #ifdef APP_DEBUG
    Drawable     debugDrawable[1];
    #endif
}
Arrow;


struct AHero
{
    Component               component[1];
    SkeletonAnimationPlayer player   [1];
    SkeletonAnimationPlayer hitFloor [1];

//----------------------------------------------------------------------------------------------------------------------

    int                     roundHP;
    int                     roundEnergy;

    int                     roundKillCount;
    int                     roundArrowCount;
    int                     roundMissCount;
    float                   roundTime;

//----------------------------------------------------------------------------------------------------------------------

    Drawable*               bowHandDrawable;
    PhysicsBody*            collisionBoxBody;
    Drawable*               collisionBoxDrawable;

//----------------------------------------------------------------------------------------------------------------------

    void (*Init)  ();
    void (*Run)   ();
    void (*Revive)();
};


extern struct AHero AHero[1];


static inline Drawable* AHero_GetDrawable()
{
    return ASkeletonAnimationPlayer_GetDrawable(AHero->player);
}


#endif
