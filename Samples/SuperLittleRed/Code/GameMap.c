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
 * Since    : 2015-9-16
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Application/Application.h"

#include "GameMap.h"
#include "Hero.h"
#include "Tool.h"


enum
{
    Cloud_Length = 3,
    Map_Length   = 1,
};


static Array(float)* groundPosVertex;
static PhysicsBody*  groundBody;
static Drawable*     cloud1Drawable = NULL;
static Drawable*     cloud2Drawable = NULL;
static Drawable*     cloud3Drawable = NULL;

#ifdef APP_DEBUG
static Drawable     debugDrawable[1];
#endif


static void Update(Component* component, float deltaSeconds)
{
    ASkeletonAnimationPlayer_Update(AGameMap->mapPlayer, deltaSeconds);

    #ifdef APP_DEBUG
    ADrawable->Draw(debugDrawable);
    #endif
}


// predefine
static void CloudAction(Drawable* cloudDrawable);


static void CloudActionOnComplete(TweenAction* action)
{
    Drawable* drawable = AUserData_GetSlotPtrWithType(action->userData, 0, Drawable*);
    ADrawable_SetPositionX(drawable, AGLTool->screenRatio + AGameMap->scaleX * drawable->width);
    CloudAction(drawable);
}


static void CloudAction(Drawable* cloudDrawable)
{
    float pos = -AGLTool->screenRatio - cloudDrawable->width * AGameMap->scaleX;

    if (cloudDrawable == cloud1Drawable)
    {
        ATweenTool->AddMoveX      (pos, 150.0f)
                  ->SetEaseType   (TweenEaseType_SineOut)
                  ->SetRelative   (false)
                  ->SetOnComplete (CloudActionOnComplete)
                  ->SetUserDataPtr(0, cloud1Drawable)
                  ->RunActions    (cloud1Drawable);
    }
    else if (cloudDrawable == cloud2Drawable)
    {
        ATweenTool->AddMoveX      (pos, 200.0f)
                  ->SetEaseType   (TweenEaseType_SineInOut)
                  ->SetRelative   (false)
                  ->SetOnComplete (CloudActionOnComplete)
                  ->SetUserDataPtr(0, cloud2Drawable)
                  ->RunActions    (cloud2Drawable);
    }
    else if (cloudDrawable == cloud3Drawable)
    {
        ATweenTool->AddInterval   (20.0f)
                  ->AddMoveX      (pos, 250.0f)
                  ->SetRelative   (false)
                  ->SetEaseType   (TweenEaseType_SineIn)
                  ->SetOnComplete (CloudActionOnComplete)
                  ->SetUserDataPtr(0, cloud3Drawable)
                  ->RunActions    (cloud3Drawable);
    }
}


static void OnActionOver(SkeletonAnimationPlayer* player)
{
    ASkeletonAnimationPlayer->SetAnimation(player, "animation");
    player->OnActionOver = NULL;

    ADrawable_SetParent(AHero_GetDrawable(), AGameMap->groundPosDrawable);

    Drawable* clouds[Cloud_Length] =
    {
        cloud1Drawable,
        cloud2Drawable,
        cloud3Drawable,
    };

    for (int i = 0; i < Cloud_Length; ++i)
    {
        Drawable* drawable = clouds[i];
        if (ATween->HasAction(drawable) == false)
        {
            CloudAction(drawable);
        }
    }
}


static const char* mapNames[Map_Length] =
{
    "Stage/Stage11",
};


static SkeletonAnimationPlayer* maps[Map_Length];


static void RandomMap()
{
    AGameMap->mapIndex                = AMath_RandomInt(0, Map_Length - 1);
    AGameMap->mapPlayer               = maps[AGameMap->mapIndex];
    AGameMap->mapPlayer->OnActionOver = OnActionOver;
    ASkeletonAnimationPlayer->SetAnimation(AGameMap->mapPlayer, "ShowIn");

    Drawable* drawable = ASkeletonAnimationPlayer_GetDrawable(AGameMap->mapPlayer);
    
    ADrawable_SetScale2
    (
        drawable,
        ATool->globalScaleX,
        ATool->globalScaleY
    );

    AGameMap->scaleX            = drawable->scaleX;
    AGameMap->scaleY            = drawable->scaleY;

    SkeletonSlot* groundPosSlot = ASkeletonAnimationPlayer_GetSlot(AGameMap->mapPlayer, "GroudPos");
    groundPosVertex             = ASkeletonSlot_GetBoundingBox(groundPosSlot)->vertexArr;

    AGameMap->groundPosData     = groundPosVertex->data;
    AGameMap->groundPosDrawable = groundPosSlot->bone->drawable;

    AGameMap->backDrawable      = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Back")  ->drawable;
    AGameMap->middleDrawable    = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Middle")->drawable;
    AGameMap->beforeDrawable    = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Before")->drawable;
    AGameMap->skyDrawable       = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Sky")   ->drawable;

    cloud1Drawable              = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Cloud1")->drawable;
    cloud2Drawable              = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Cloud2")->drawable;
    cloud3Drawable              = ASkeletonAnimationPlayer_GetBone(AGameMap->mapPlayer, "Cloud3")->drawable;

    AGameMap->moveAreaY         = AGameMap->groundPosData[1] * drawable->scaleY;
    AGameMap->groundY           = AGameMap->groundPosData[1];

    AGameMap->minX              = AGameMap->groundPosData[0];
    AGameMap->maxX              = AGameMap->groundPosData[6];

    groundBody                  = APhysicsWorld->AddBody
                                  (
                                     PhysicsShape_Line,
                                     AArray_Make
                                     (
                                         float, 4,
                                         AGameMap->groundPosData[0],
                                         AGameMap->groundPosData[1],
                                         AGameMap->groundPosData[6],
                                         AGameMap->groundPosData[1]
                                     )
                                  );

    groundBody->state = PhysicsBodyState_Fixed;
    APhysicsBody_SetCollisionGroup
    (
        groundBody,
        CollisionGroup_HeroBody | CollisionGroup_EnemyBody
    );

    #ifdef APP_DEBUG
    ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable(AGameMap->mapPlayer, "GroudPos", debugDrawable);
    #endif

    // arrow tail render requires AGameMap beforeDrawable mvpMatrix update
    ADrawable_AddState(AGameMap->beforeDrawable, DrawableState_IsUpdateMVPMatrix);
}


static void Init()
{
    AComponent->Init(AGameMap->component);
    AGameMap->component->curState->Update = Update;

    for (int i = 0; i < Map_Length; ++i)
    {
        maps[i] = ASkeletonAnimationPlayer->Create(mapNames[i], "ShowIn");
    }

    RandomMap();
}


static void Run()
{
    AApplication_AddChild(AGameMap->component, 10);
}


struct AGameMap AGameMap[1] =
{{
    .Init      = Init,
    .Run       = Run,
    .RandomMap = RandomMap,
}};
