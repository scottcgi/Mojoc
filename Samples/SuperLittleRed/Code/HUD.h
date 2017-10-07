/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2016-12-2
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef HUD_H
#define HUD_H


#include "Engine/Application/Component.h"
#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"
#include "Engine/Extension/Font.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Config.h"


typedef struct
{
    Drawable*    drawable;
    PhysicsBody* body;
    int          bodySize;
    int          collisionGroup;
    int          bodyId;

    #ifdef APP_DEBUG
    Drawable     debugDrawable[1];
    #endif
}
DropCollisionItem;


struct AHUD
{
    Component               component    [1];
    SkeletonAnimationPlayer hudPlayer    [1];
    SkeletonAnimationPlayer curtainPlayer[1];

    Drawable*               energyLengthDrawable;
    Drawable*               energyDrawable;
    Drawable*               tombstoneDrawable;
    Drawable*               upDrawable;
    Drawable*               downDrawable;
    Drawable*               energyPointerDrawable;

    float                   tombstoneHeight;
    float                   tombstoneBornY;
    int                     tryAgainCount;

    FontText*               heartText;
    FontText*               coinText;
    FontText*               woodText;
    FontText*               stoneText;
    FontText*               killText;

    Drawable*               heartDrawable;
    Drawable*               coinDrawable;
    Drawable*               stoneDrawable;
    Drawable*               woodDrawable;
    Drawable*               killDrawable;

    void (*Init)         ();
    void (*Run)          ();
    bool (*CostPower)    (float cost);
    void (*OpenCurtain)  (SkeletonAnimationPlayerOnActionOver callback);
    void (*CloseCurtain) (SkeletonAnimationPlayerOnActionOver callback);
    void (*Reset)        ();

    void (*DropStone)    ();
    void (*DropWood)     ();

    void (*DropPickHeart)(PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickCoin) (PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickStone)(PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickWood) (PhysicsBody* arrowBody, float enemyScaleX);

    void (*ClearDrop)    ();
    void (*FlyItem)      (DropCollisionItem* item);
};


extern struct AHUD AHUD[1];


#endif
