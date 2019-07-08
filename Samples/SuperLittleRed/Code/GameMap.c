/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since    : 2015-9-16
 * Author   : scott.cgi
 */

#include <stdio.h>

#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Application/Application.h"

#include "GameMap.h"
#include "Hero.h"
#include "Tool.h"


static Array(float)*  groundPosVertex;
static Matrix4*       groundPosModel;
static PhysicsBody*   groundBody;


static Drawable*      cloud1Drawable = NULL;
static Drawable*      cloud2Drawable = NULL;
static Drawable*      cloud3Drawable = NULL;


#ifdef APP_DEBUG
static Drawable       debugDrawable[1];
#endif


static void Update(Component* component, float deltaSeconds)
{
    ASkeletonAnimationPlayer_Update(AGameMap->mapPlayer, deltaSeconds);

    #ifdef APP_DEBUG
    ADrawable->Draw(debugDrawable);
    #endif
}


static void CloudAction(Drawable* cloudDrawable);


static void CloudActionOnComplete(TweenAction* action)
{
    Drawable* drawable = (Drawable*) action->userData->slot0->ptrValue;
    ADrawable_SetPositionX(drawable, AGLTool->screenRatio + AGameMap->scaleX * drawable->width);
    CloudAction(drawable);
}


static void CloudAction(Drawable* cloudDrawable)
{
    float pos = -AGLTool->screenRatio - cloudDrawable->width * AGameMap->scaleX;

    if (cloudDrawable == cloud1Drawable)
    {
        ATweenTool->AddMoveX       (pos, 100.0f)
                  ->SetEaseType    (TweenEaseType_SineOut)
                  ->SetRelative    (false)
                  ->SetOnComplete  (CloudActionOnComplete)
                  ->SetUserData0Ptr(cloud1Drawable)
                  ->RunActions     (cloud1Drawable);
    }
    else if (cloudDrawable == cloud2Drawable)
    {
        ATweenTool->AddMoveX       (pos, 150.0f)
                  ->SetEaseType    (TweenEaseType_SineInOut)
                  ->SetRelative    (false)
                  ->SetOnComplete  (CloudActionOnComplete)
                  ->SetUserData0Ptr(cloud2Drawable)
                  ->RunActions     (cloud2Drawable);
    }
    else if (cloudDrawable == cloud3Drawable)
    {
        ATweenTool->AddInterval    (20.0f)
                  ->AddMoveX       (pos, 200.0f)
                  ->SetRelative    (false)
                  ->SetEaseType    (TweenEaseType_SineIn)
                  ->SetOnComplete  (CloudActionOnComplete)
                  ->SetUserData0Ptr(cloud3Drawable)
                  ->RunActions     (cloud3Drawable);
    }
}


static void OnActionOver(SkeletonAnimationPlayer* player)
{
    ASkeletonAnimationPlayer->SetAnimation(player, "animation");
    player->OnActionOver = NULL;

    ADrawable_SetParent(AHero_GetDrawable(), AGameMap->groundPosDrawable);

    Drawable* clouds[] =
    {
        cloud1Drawable,
        cloud2Drawable,
        cloud3Drawable,
    };

    for (int i = 0; i < 3; i++)
    {
        Drawable* drawable = clouds[i];

        if (ATween->HasAction(drawable) == false)
        {
            CloudAction(drawable);
        }
    }
}


static SkeletonAnimationPlayer* maps[1];


static void RandomMap()
{
    AGameMap->mapPlayer               = maps[0];
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
    groundPosModel              = AGameMap->groundPosDrawable->modelMatrix;

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
                                         float,
                                         4,
                                         AGameMap->groundPosData[0],
                                         AGameMap->groundPosData[1],
                                         AGameMap->groundPosData[6],
                                         AGameMap->groundPosData[1]
                                     )
                                  );

    APhysicsBody_SetState         (groundBody, PhysicsBodyState_IsFixed);
    APhysicsBody_SetCollisionGroup(groundBody, CollisionGroup_HeroBody | CollisionGroup_EnemyBody);


    #ifdef APP_DEBUG
    ASkeletonAnimationPlayer->InitSlotBoundingBoxDrawable(AGameMap->mapPlayer, "GroudPos", debugDrawable);
    #endif
}


static void Init()
{
    AComponent->Init(AGameMap->component);
    AGameMap->component->curState->Update = Update;

    maps[0] = ASkeletonAnimationPlayer->Create("Stage/Stage11", "ShowIn");

    RandomMap();
}


static void Run()
{
    AApplication_AddChild(AGameMap->component, 10);
}


struct AGameMap AGameMap[1] =
{
    {
        .Init      = Init,
        .Run       = Run,
        .RandomMap = RandomMap,
    }
};
