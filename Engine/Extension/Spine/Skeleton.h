/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-7-2
 * Author: scott.cgi
 */

#ifndef SKELETON_H
#define SKELETON_H


#include "Engine/Toolkit/Head/Define.h"
#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"


typedef struct Skeleton Skeleton;
struct  Skeleton
{
    /**
     * Has been implemented render if override must call the original
     */
    Drawable                             drawable[1];
    ArrayStrMap(boneName, SkeletonBone*) boneMap [1];
    ArrayStrMap(slotName, SkeletonSlot*) slotMap [1];

    SkeletonData*                        skeletonData;
    SkeletonSkinData*                    curSkinData;
    Array(SkeletonBone)*                 boneArr;
    Array(SkeletonSlot)*                 slotArr;
    Array(SkeletonSlot*)*                slotOrderArr;

    /**
     * Skeleton used Mesh list
     */
    ArrayList(Mesh)                      meshList[1];

    /**
     * If not NULL, callback when SkeletonEvent fire
     */
    void (*FireSkeletonEvent)(Skeleton* skeleton, SkeletonEventData* eventData, float mixPercent);
};


struct ASkeleton
{
    Skeleton*               (*Create)              (SkeletonData* skeletonData);
    void                    (*Init)                (SkeletonData* skeletonData, Skeleton* outSkeleton);
    void                    (*Release)             (Skeleton*     skeleton);
    void                    (*SetSkin)             (Skeleton*     skeleton,     char*     skinName);

    /**
     * Reset Skeleton bones to setup pose
     */
    void                    (*ResetBones)          (Skeleton* skeleton);

    /**
     * Reset Skeleton slots to setup pose
     */
    void                    (*ResetSlots)          (Skeleton* skeleton);


    /**
     * Find by curSkinData, if not found will find by default SkinData, not found then return NULL
     */
    SkeletonAttachmentData* (*GetAttachmentData)   (Skeleton* skeleton, char* slotName, char* attachmentName);

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


static inline void ASkeleton_Draw(Skeleton* skeleton)
{
    ADrawable->Draw(skeleton->drawable);
}


static inline SkeletonAnimationData* ASkeleton_GetAnimationData(Skeleton* skeleton, char* animationName)
{
    return AArrayStrMap_Get
           (
                skeleton->skeletonData->animationDataMap,
                animationName,
                SkeletonAnimationData*
           );
}


static inline SubMesh* ASkeleton_GetSubMesh(Skeleton* skeleton, char* slotName, char* attachmentName)
{
    return ASkeleton->GetAttachmentSubMesh
           (
               skeleton,
               ASkeleton->GetAttachmentData(skeleton, slotName, attachmentName)
           );
}


static inline float ASkeleton_GetWidth(Skeleton* skeleton)
{
    return skeleton->skeletonData->width;
}


static inline float ASkeleton_GetHeight(Skeleton* skeleton)
{
    return skeleton->skeletonData->height;
}


#endif
