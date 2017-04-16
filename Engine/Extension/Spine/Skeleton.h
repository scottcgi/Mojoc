/*
 *
 *
 *  Created on: 2013-7-2
 *      Author: scott.cgi
 */

#ifndef skeleton_h
#define skeleton_h

#include "Engine/Toolkit/Head/MacroDefine.h"
#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"


typedef struct Skeleton Skeleton;
struct  Skeleton
{
	/**
	 * Has been implemented render if override must call the original
	 */
	Drawable                                      drawable[1];
	ArrayStrMap(boneName, SkeletonBone*) get_only boneMap [1];
	ArrayStrMap(slotName, SkeletonSlot*) get_only slotMap [1];

	SkeletonData*                        get_only skeletonData;
	SkeletonSkinData*                    get_only curSkinData;
	Array(SkeletonBone)*                 get_only boneArr;
	Array(SkeletonSlot)*                 get_only slotArr;
	Array(SkeletonSlot*)*                get_only slotOrderArr;

	ArrayList(Mesh)                      get_only meshList[1];

	/**
	 * If not NULL, callback when SkeletonEvent fire
	 */
	void (*FireSkeletonEvent)(Skeleton* skeleton, SkeletonEventData* eventData, float mixPersent);
};


struct ASkeleton
{
	Skeleton*               (*Create)              (SkeletonData* skeletonData);
	void                    (*Init)                (SkeletonData* skeletonData, Skeleton* out_param skeleton);
	void                    (*Release)             (Skeleton*     skeleton);
	void                    (*SetSkin)             (Skeleton* skeleton, const char* skinName);

	/**
	 * Reset Skeleton bones to setup pose
	 */
	void                    (*ResetBones)          (Skeleton* skeleton);

	/**
	 * Reset Skeleton slots to setup pose
	 */
	void                    (*ResetSlots)          (Skeleton* skeleton);


	/** Find by curSkinData, if not found will find by default SkinData, not found then return NULL  */
	SkeletonAttachmentData* (*GetAttachmentData)   (Skeleton* skeleton, const char* slotName, const char* attachmentName);

	/**
	 * Get SubMesh in Mesh bind in SkeletonAttachmentData
	 */
	SubMesh*                (*GetAttachmentSubMesh)(Skeleton* skeleton, SkeletonAttachmentData* skeletonAttachmentData);

	/**
	 * Poses the skeleton at the specified time for animationData
	 * mixPercent is  percent  poses effect to apply, range in [0, 1]
	 *
	 * mixPercent is 1.0f will 100% apply this time pose effect
	 * mixPercent is 0.0f will 0%   apply this time pose effect
	 *
	 * time is second
	 */
	void                     (*Apply)              (Skeleton* skeleton, SkeletonAnimationData* animationData, float time, float mixPercent);
};


extern struct ASkeleton ASkeleton[1];


static inline void ASkeletonDraw(Skeleton* skeleton)
{
	ADrawable->Draw(skeleton->drawable);
}


static inline SkeletonAnimationData* ASkeletonGetAnimationData(Skeleton* skeleton, const char* animationName)
{
    return AArrayStrMapGet
           (
                skeleton->skeletonData->animationDataMap,
                animationName,
                SkeletonAnimationData*
           );
}


static inline SubMesh* ASkeletonGetSubMesh(Skeleton* skeleton, const char* slotName, const char* attachmentName)
{
    return ASkeleton->GetAttachmentSubMesh
           (
               skeleton,
               ASkeleton->GetAttachmentData(skeleton, slotName, attachmentName)
           );
}

static inline float ASkeletonGetWidth(Skeleton* skeleton)
{
	return skeleton->skeletonData->width;
}


static inline float ASkeletonGetHeight(Skeleton* skeleton)
{
    return skeleton->skeletonData->height;
}


#endif
