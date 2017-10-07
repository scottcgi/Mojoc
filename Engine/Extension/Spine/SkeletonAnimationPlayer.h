/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-7-28
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef SKELETON_ANIMATION_PLAYER_H
#define SKELETON_ANIMATION_PLAYER_H


#include "Engine/Toolkit/Head/Define.h"
#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"


typedef struct SkeletonAnimationPlayer SkeletonAnimationPlayer;
typedef void (*SkeletonAnimationPlayerOnActionOver)(SkeletonAnimationPlayer* player);


struct  SkeletonAnimationPlayer
{
    Skeleton              skeleton[1];

    /**
     * Default -1
     * if a negative infinite loop
     * if a positive will loop count to 0
     * if loop to 0 animation stop forever
     */
    int                    loop;

    SkeletonAnimationData* curAnimationData;
    SkeletonAnimationData* preAnimationData;

    /**
     * curAnimationData current time
     */
    float                  curTime;

    /**
     * preAnimationData current time
     */
    float                  preTime;

    /** T
     * he preAnimationData mix to preAnimationData current time
     */
    float                  mixTime;

    /**
     * The preAnimationData mix to preAnimationData duration time
     */
    float                  mixDuration;

    /**
     * Callback when action over
     */
    SkeletonAnimationPlayerOnActionOver OnActionOver;
};


struct ASkeletonAnimationPlayer
{
    SkeletonAnimationPlayer* (*Create)                     (char*                    filePath,     char* animationName);
    SkeletonAnimationPlayer* (*CreateWithData)             (SkeletonData*            skeletonData, char* animationName);
    void                     (*Init)                       (char*                    filePath,     char* animationName, SkeletonAnimationPlayer* outPlayer);
    void                     (*Release)                    (SkeletonAnimationPlayer* player);

    /**
     * Update skeleton apply and draw
     */
    void                     (*Update)                     (SkeletonAnimationPlayer* player, float deltaSeconds);

    /**
     * Set player's animationData in Skeleton
     */
    void                     (*SetAnimation)               (SkeletonAnimationPlayer* player, char* animationName);

    /**
     * Set player's animationData in Skeleton, Skeleton will from preAnimation mix to this animation
     * the mixDuration is cross fade time in second
     */
    void                     (*SetAnimationMix)            (SkeletonAnimationPlayer* player, char* animationName, float mixDuration);

    /**
     * Initialize Drawable that can render slot bounding box with primitive call
     */
    void                     (*InitSlotBoundingBoxDrawable)(SkeletonAnimationPlayer* player, char* slotName, Drawable* outDrawable);
};


extern struct ASkeletonAnimationPlayer ASkeletonAnimationPlayer[1];


static inline SkeletonBone* ASkeletonAnimationPlayer_GetBone(SkeletonAnimationPlayer* player, char* boneName)
{
    return AArrayStrMap_Get(player->skeleton->boneMap, boneName, SkeletonBone*);
}


static inline SkeletonSlot* ASkeletonAnimationPlayer_GetSlot(SkeletonAnimationPlayer* player, char* slotName)
{
    return AArrayStrMap_Get(player->skeleton->slotMap, slotName, SkeletonSlot*);
}


static inline Drawable* ASkeletonAnimationPlayer_GetDrawable(SkeletonAnimationPlayer* player)
{
    return player->skeleton->drawable;
}


static inline void ASkeletonAnimationPlayer_Update(SkeletonAnimationPlayer* player, float deltaSeconds)
{
    ASkeletonAnimationPlayer->Update(player, deltaSeconds);
}


static inline SkeletonData* ASkeletonAnimationPlayer_GetSkeletonData(SkeletonAnimationPlayer* player)
{
    return player->skeleton->skeletonData;
}


static inline char* ASkeletonAnimationPlayer_GetPath(SkeletonAnimationPlayer* player)
{
    return player->skeleton->skeletonData->filePath;
}


static inline SubMesh* ASkeletonAnimationPlayer_GetSubMesh(SkeletonAnimationPlayer* player, char* slotName, char* attachmentName)
{
    return ASkeleton_GetSubMesh(player->skeleton, slotName, attachmentName);
}


static inline float ASkeletonAnimationPlayer_GetWidth(SkeletonAnimationPlayer* player)
{
    return ASkeleton_GetWidth(player->skeleton);
}


static inline float ASkeletonAnimationPlayer_GetHeight(SkeletonAnimationPlayer* player)
{
    return ASkeleton_GetHeight(player->skeleton);
}


#endif
