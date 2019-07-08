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
 * Since    : 2013-7-28
 * Update   : 2019-2-14
 * Author   : scott.cgi
 */


#include <math.h>
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Extension/Spine/SkeletonAnimationPlayer.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/GLPrimitive.h"


static inline void UpdateNormal(SkeletonAnimationPlayer* player, float deltaSeconds)
{
    if (player->curTime < player->curAnimationData->duration)
    {
        ASkeleton->Apply(player->skeleton, player->curAnimationData, player->curTime, 1.0f);
        ASkeleton_Draw   (player->skeleton);

        player->curTime += deltaSeconds;

    }
    else
    {
        ASkeleton->Apply(player->skeleton, player->curAnimationData, player->curAnimationData->duration, 1.0f);
        ASkeleton_Draw  (player->skeleton);

        if (player->OnActionOver != NULL)
        {
            // action over function may call SetAnimationMix
            // so we need keep cuTime value
            player->OnActionOver(player);
        }

        if (player->loop > 0)
        {
            --player->loop;
        }

        player->curTime = 0.0f;
    }
}


static inline void UpdateMix(SkeletonAnimationPlayer* player, float deltaSeconds)
{
    if (player->preTime < player->preAnimationData->duration)
    {
        ASkeleton->Apply(player->skeleton, player->preAnimationData, player->preTime, 1.0f);
        player->preTime += deltaSeconds;
    }
    else
    {
        ASkeleton->Apply(player->skeleton, player->preAnimationData, player->preAnimationData->duration, 1.0f);
    }

    float mixPercent = player->mixTime / player->mixDuration;
    if (mixPercent < 1.0f)
    {
        player->mixTime += deltaSeconds;
    }
    else
    {
        mixPercent               = 1.0f;
        player->preAnimationData = NULL;
    }

    if (player->curTime < player->curAnimationData->duration)
    {
        ASkeleton->Apply(player->skeleton, player->curAnimationData, player->curTime, mixPercent);
        ASkeleton_Draw  (player->skeleton);

        player->curTime += deltaSeconds;
    }
    else
    {
        ASkeleton->Apply(player->skeleton, player->curAnimationData, player->curAnimationData->duration, mixPercent);
        ASkeleton_Draw  (player->skeleton);

        if (player->OnActionOver != NULL)
        {
            // action over function may call SetAnimationMix
            // so we need keep cuTime value
            player->OnActionOver(player);
        }

        player->curTime = 0.0f;
    }
}


static void Update(SkeletonAnimationPlayer* player, float deltaSeconds)
{
    if (player->loop != 0)
    {
        if (player->preAnimationData == NULL)
        {
            UpdateNormal(player, deltaSeconds);
        }
        else
        {
            UpdateMix(player, deltaSeconds);
        }
    }
}


static inline void InitSkeletonAnimationPlayer
(
    SkeletonAnimationPlayer* player, 
    SkeletonData*            skeletonData,
    const char*              animationName
)
{
    ASkeleton->Init(skeletonData, player->skeleton);
    player->preAnimationData = NULL;
    player->curAnimationData = ASkeleton_GetAnimationData(player->skeleton, animationName);

    ALog_A
    (
        player->curAnimationData != NULL,
        "ASkeletonAnimationPlayer cannot find animationData by name = %s",
        animationName
    );

    player->curTime      = 0.0f;
    player->preTime      = 0.0f;
    player->mixTime      = 0.0f;
    player->mixDuration  = 0.0f;
    player->OnActionOver = NULL;
    player->loop         = -1;

    //ASkeletonAnimationPlayer->Release(player);
    //ALog_A(0, "stop");
}


static void Release(SkeletonAnimationPlayer* player)
{
    ASkeleton->Release(player->skeleton);
}


static void SetAnimation(SkeletonAnimationPlayer* player, const char* animationName)
{
    player->curAnimationData = ASkeleton_GetAnimationData(player->skeleton,  animationName);

    ALog_A
    (
        player->curAnimationData != NULL,
        "ASkeletonAnimationPlayer SetAnimation cannot find animationData by name = %s",
        animationName
    );
    
    player->curTime = 0.0f;
}


static void SetAnimationMix(SkeletonAnimationPlayer* player, const char* animationName, float mixDuration)
{
    player->preAnimationData = player->curAnimationData;
    player->preTime          = player->curTime;
    player->curTime          = 0.0f;
    player->mixTime          = 0.0f;
    player->mixDuration      = mixDuration;
    player->curAnimationData = ASkeleton_GetAnimationData(player->skeleton,  animationName);
    
    ALog_A
    (
        player->curAnimationData != NULL,
        "SkeletonAnimationPlayer SetAnimationMix cannot find animationData by name = %s",
        animationName
    );
}


static SkeletonAnimationPlayer* CreateWithData(SkeletonData* skeletonData, const char* animationName)
{
    SkeletonAnimationPlayer* player = malloc(sizeof(SkeletonAnimationPlayer));
    InitSkeletonAnimationPlayer(player, skeletonData, animationName);

    return player;
}


static SkeletonAnimationPlayer* Create(const char* jsonFilePath, const char* animationName)
{
    SkeletonAnimationPlayer* player = malloc(sizeof(SkeletonAnimationPlayer));
    InitSkeletonAnimationPlayer(player, ASkeletonData->Get(jsonFilePath), animationName);

    return player;
}


static void Init(const char* jsonFilePath, const char* animationName, SkeletonAnimationPlayer* outPlayer)
{
    InitSkeletonAnimationPlayer(outPlayer, ASkeletonData->Get(jsonFilePath), animationName);
}


static void Render(Drawable* drawable)
{
    SkeletonSlot* slot = AUserData_GetSlotPtrWithType(drawable->userData, 0, SkeletonSlot*);

    AGLPrimitive->RenderPolygon
    (
        ASkeletonSlot_GetBoundingBox(slot)->vertexArr,
        slot->bone->drawable->mvpMatrix,
        slot->color,
        1.0f
    );
}


static void InitSlotBoundingBoxDrawable(SkeletonAnimationPlayer* player, const char* slotName, Drawable* outDrawable)
{
    SkeletonSlot* slot = ASkeletonAnimationPlayer_GetSlot(player, slotName);
    
    ALog_A
    (
        slot != NULL,
        "SkeletonAnimationPlayer InitSlotBoundingBoxDrawable not found SkeletonSlot by slotName = %s",
        slotName
    );

    ADrawable->Init(outDrawable);
    AUserData_SetSlotPtr(outDrawable->userData, 0, slot);
    outDrawable->Render = Render;

    // render requires slot mvpMatrix update
    ADrawable_AddState(slot->bone->drawable, DrawableState_IsUpdateMVPMatrix);
}


struct ASkeletonAnimationPlayer ASkeletonAnimationPlayer[1] =
{
    Create,
    CreateWithData,
    Init,

    Release,
    Update,

    SetAnimation,
    SetAnimationMix,

    InitSlotBoundingBoxDrawable,
 };