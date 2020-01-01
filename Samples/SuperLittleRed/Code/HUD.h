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
 * Since    : 2016-12-2
 * Update   : 2019-2-16
 * Author   : scott.cgi
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
    SubMesh*     subMesh;
    PhysicsBody* body;
    int          bodySize;
    int          collisionGroup;
    int          bodyID;

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

    void (*Init)         (void);
    void (*Run)          (void);
    bool (*CostPower)    (int cost);
    void (*OpenCurtain)  (SkeletonAnimationPlayerOnActionOver callback);
    void (*CloseCurtain) (SkeletonAnimationPlayerOnActionOver callback);
    void (*Reset)        (void);

    void (*DropStone)    (void);
    void (*DropWood)     (void);

    void (*DropPickHeart)(PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickCoin) (PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickStone)(PhysicsBody* arrowBody, float enemyScaleX);
    void (*DropPickWood) (PhysicsBody* arrowBody, float enemyScaleX);

    void (*ClearDrop)    (void);
    void (*FlyItem)      (DropCollisionItem* item);
};


extern struct AHUD AHUD[1];


#endif
