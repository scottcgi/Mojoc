/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-7-28
 * Author: scott.cgi
 */

#ifndef skeleton_animation_player_h
#define skeleton_animation_player_h


#include "Engine/Toolkit/Head/MacroDefine.h"
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


static inline SkeletonBone* ASkeletonAnimationPlayerGetBone(SkeletonAnimationPlayer* player, char* boneName)
{
	return AArrayStrMapGet(player->skeleton->boneMap, boneName, SkeletonBone*);
}


static inline SkeletonSlot* ASkeletonAnimationPlayerGetSlot(SkeletonAnimationPlayer* player, char* slotName)
{
	return AArrayStrMapGet(player->skeleton->slotMap, slotName, SkeletonSlot*);
}


static inline Drawable* ASkeletonAnimationPlayerGetDrawable(SkeletonAnimationPlayer* player)
{
	return player->skeleton->drawable;
}


static inline void ASkeletonAnimationPlayerUpdate(SkeletonAnimationPlayer* player, float deltaSeconds)
{
	ASkeletonAnimationPlayer->Update(player, deltaSeconds);
}


static inline SkeletonData* ASkeletonAnimationPlayerGetSkeletonData(SkeletonAnimationPlayer* player)
{
	return player->skeleton->skeletonData;
}


static inline char* ASkeletonAnimationPlayerGetPath(SkeletonAnimationPlayer* player)
{
	return player->skeleton->skeletonData->filePath;
}


static inline SubMesh* ASkeletonAnimationPlayerGetSubMesh(SkeletonAnimationPlayer* player, char* slotName, char* attachmentName)
{
    return ASkeletonGetSubMesh(player->skeleton, slotName, attachmentName);
}


static inline float ASkeletonAnimationPlayerGetWidth(SkeletonAnimationPlayer* player)
{
    return ASkeletonGetWidth(player->skeleton);
}


static inline float ASkeletonAnimationPlayerGetHeight(SkeletonAnimationPlayer* player)
{
    return ASkeletonGetHeight(player->skeleton);
}


#endif
