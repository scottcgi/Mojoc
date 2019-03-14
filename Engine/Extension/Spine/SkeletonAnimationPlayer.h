/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-7-28
 * Update: 2019-2-13
 * Author: scott.cgi
 */


#ifndef SKELETON_ANIMATION_PLAYER_H
#define SKELETON_ANIMATION_PLAYER_H


#include "Engine/Toolkit/HeaderUtils/Define.h"
#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"


typedef struct SkeletonAnimationPlayer SkeletonAnimationPlayer;

/**
 * Callback when SkeletonAnimationPlayer current action over.
 */
typedef void (*SkeletonAnimationPlayerOnActionOver)(SkeletonAnimationPlayer* player);


/**
 * Drive skeleton animation play by animationData.
 */
struct SkeletonAnimationPlayer
{
    Skeleton              skeleton[1];

    /**
     * Action play loops, default -1.
     * 
     * if negative then infinite loop.
     * if positive then loop to 0.
     * if 0 then stop forever.
     */
    int                    loop;

    SkeletonAnimationData* curAnimationData;
    SkeletonAnimationData* preAnimationData;

    /**
     * The curAnimationData current time.
     */
    float                  curTime;

    /**
     * The preAnimationData current time.
     */
    float                  preTime;

    /**
     * The preAnimationData current mix time.
     */
    float                  mixTime;

    /**
     * The preAnimationData mix duration time.
     */
    float                  mixDuration;

    /**
     * Callback when action over.
     */
    SkeletonAnimationPlayerOnActionOver OnActionOver;
};


/**
 * Control and mange SkeletonAnimationPlayer.
 */
struct ASkeletonAnimationPlayer
{
    /**
     * Create SkeletonAnimationPlayer by jsonFilePath.
     *
     * jsonFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    SkeletonAnimationPlayer* (*Create)         (const char*   jsonFilePath, const char* animationName);
    SkeletonAnimationPlayer* (*CreateWithData) (SkeletonData* skeletonData, const char* animationName);

    /**
     * Init SkeletonAnimationPlayer by jsonFilePath.
     *
     * jsonFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    void                     (*Init)           (
                                                   const char*              jsonFilePath,
                                                   const char*              animationName,
                                                   SkeletonAnimationPlayer* outPlayer
                                               );
    
    void                     (*Release)        (SkeletonAnimationPlayer*    player);

    /**
     * Apply and draw skeleton.
     */
    void                     (*Update)         (SkeletonAnimationPlayer* player, float deltaSeconds);

    /**
     * Set player's animationData in Skeleton by animationName.
     */
    void                     (*SetAnimation)   (SkeletonAnimationPlayer* player, const char* animationName);

    /**
     * Set player's animationData in Skeleton by animationName.
     * the skeleton will from preAnimation mix to this animation.
     * the mixDuration is cross fade time in seconds.
     */
    void                     (*SetAnimationMix)(
                                                   SkeletonAnimationPlayer* player,
                                                   const char*              animationName,
                                                   float                    mixDuration
                                               );

    /**
     * Init Drawable that can render slot bounding box with primitive call.
     */
    void                     (*InitSlotBoundingBoxDrawable)(
                                                               SkeletonAnimationPlayer* player,
                                                               const char*              slotName,
                                                               Drawable*                outDrawable
                                                           );
};


extern struct ASkeletonAnimationPlayer ASkeletonAnimationPlayer[1];


/**
 * Get bone from player by boneName.
 */
static inline SkeletonBone* ASkeletonAnimationPlayer_GetBone(SkeletonAnimationPlayer* player, const char* boneName)
{
    return AArrayStrMap_Get(player->skeleton->boneMap, boneName, SkeletonBone*);
}


/**
 * Get slot from player by slotName.
 */
static inline SkeletonSlot* ASkeletonAnimationPlayer_GetSlot(SkeletonAnimationPlayer* player, const char* slotName)
{
    return AArrayStrMap_Get(player->skeleton->slotMap, slotName, SkeletonSlot*);
}


/**
 * Get drawable from player.
 */
static inline Drawable* ASkeletonAnimationPlayer_GetDrawable(SkeletonAnimationPlayer* player)
{
    return player->skeleton->drawable;
}


/**
 * Call player update method.
 */
static inline void ASkeletonAnimationPlayer_Update(SkeletonAnimationPlayer* player, float deltaSeconds)
{
    ASkeletonAnimationPlayer->Update(player, deltaSeconds);
}


/**
 * Get skeletonData from player.
 */
static inline SkeletonData* ASkeletonAnimationPlayer_GetSkeletonData(SkeletonAnimationPlayer* player)
{
    return player->skeleton->skeletonData;
}


/**
 * Get the player loading path.
 */
static inline const char* ASkeletonAnimationPlayer_GetPath(SkeletonAnimationPlayer* player)
{
    return player->skeleton->skeletonData->filePath;
}


/**
 * Get the SubMesh from player by slotName and attachmentName.
 */
static inline SubMesh* ASkeletonAnimationPlayer_GetSubMesh
(
    SkeletonAnimationPlayer* player, 
    const char*              slotName,
    const char*              attachmentName
)
{
    return ASkeleton_GetSubMesh(player->skeleton, slotName, attachmentName);
}


/**
 * Get the player width.
 */
static inline float ASkeletonAnimationPlayer_GetWidth(SkeletonAnimationPlayer* player)
{
    return ASkeleton_GetWidth(player->skeleton);
}


/**
 * Get the player height.
 */
static inline float ASkeletonAnimationPlayer_GetHeight(SkeletonAnimationPlayer* player)
{
    return ASkeleton_GetHeight(player->skeleton);
}


#endif
