/*
 *
 *
 *  Created on: 2013-7-28
 *      Author: scott.cgi
 */

#ifndef skeleton_animation_player_h
#define skeleton_animation_player_h


#include "Engine/Toolkit/Define/Def.h"
#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "SkeletonData.h"


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

	/** curAnimationData current time */
	float                  curTime;
	/** preAnimationData current time */
	float                  preTime;

	/** The preAnimationData mix to preAnimationData current time */
	float                  mixTime;
	/** The preAnimationData mix to preAnimationData duration time */
	float                  mixDuration;

	/**
	 * Callback when action over
	 */
    SkeletonAnimationPlayerOnActionOver OnActionOver;
};


struct ASkeletonAnimationPlayer
{
	SkeletonAnimationPlayer* (*Create)                     (const char*              filePath,     const char* animationName);
	SkeletonAnimationPlayer* (*CreateWithData)             (SkeletonData*            skeletonData, const char* animationName);
	void                     (*Init)                       (const char*              filePath,     const char* animationName, SkeletonAnimationPlayer* outPlayer);
	void                     (*Release)                    (SkeletonAnimationPlayer* player);

    /**
     * Update skeleton apply and draw
     */
	void                     (*Update)                     (SkeletonAnimationPlayer* player, float deltaTime);

	/**
	 * Set player's animationData in Skeleton
	 */
	void                     (*SetAnimation)               (SkeletonAnimationPlayer* player, const char* animationName);

	/**
	 * Set player's animationData in Skeleton, Skeleton will from preAnimation mix to this animation
	 * the mixDuration is cross fade time in second
	 */
	void                     (*SetAnimationMix)            (SkeletonAnimationPlayer* player, const char* animationName, float mixDuration);

	/**
	 * Initialize Drawable that can render slot bounding box with primitive call
	 */
	void                     (*InitSlotBoundingBoxDrawable)(SkeletonAnimationPlayer* player, const char* slotName, Drawable* outDrawable);
};

extern struct ASkeletonAnimationPlayer ASkeletonAnimationPlayer[1];



static inline SkeletonBone* ASkeletonAnimationPlayerGetBone(SkeletonAnimationPlayer* player, const char* boneName)
{
	return AArrayStrMapGet(player->skeleton->boneMap, boneName, SkeletonBone*);
}


static inline SkeletonSlot* ASkeletonAnimationPlayerGetSlot(SkeletonAnimationPlayer* player, const char* slotName)
{
	return AArrayStrMapGet(player->skeleton->slotMap, slotName, SkeletonSlot*);
}


static inline Drawable* ASkeletonAnimationPlayerGetDrawable(SkeletonAnimationPlayer* player)
{
	return player->skeleton->drawable;
}

static inline void ASkeletonAnimationPlayerUpdate(SkeletonAnimationPlayer* player, float deltaTime)
{
	ASkeletonAnimationPlayer->Update(player, deltaTime);
}

static inline SkeletonData* ASkeletonAnimationPlayerGetSkeletonData(SkeletonAnimationPlayer* player)
{
	return player->skeleton->skeletonData;
}

static inline const char* ASkeletonAnimationPlayerGetPath(SkeletonAnimationPlayer* player)
{
	return player->skeleton->skeletonData->filePath;
}

static inline SubMesh* ASkeletonAnimationPlayerGetSubMesh(SkeletonAnimationPlayer* player, const char* slotName, const char* attachmentName)
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
