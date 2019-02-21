/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-7-2
 * Update: 2019-2-14
 * Author: scott.cgi
 */


#ifndef SKELETON_H
#define SKELETON_H


#include "Engine/Toolkit/HeaderUtils/Define.h"
#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"


typedef struct Skeleton Skeleton;

/**
 * Skeleton consists of bones and slots.
 * the bones can transform.
 * the slots can provide color and texture.
 * and the bones and slots read from boneData and slotData of skeletonData.
 *
 * the transform, color and texture will render by the mesh list.
 */
struct  Skeleton
{
    /**
     * The base class for provide draw functions.
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
     * The meshes that Skeleton needs to use.
     */
    ArrayList(Mesh)                      meshList[1];

    /**
     * If not NULL, callback when SkeletonEvent fired.
     */
    void (*FireSkeletonEvent)(Skeleton* skeleton, SkeletonEventData* eventData, float mixPercent);
};


/**
 * Control and manager Skeleton.
 */
struct ASkeleton
{
    Skeleton*               (*Create)              (SkeletonData* skeletonData);
    void                    (*Init)                (SkeletonData* skeletonData, Skeleton*   outSkeleton);
    void                    (*Release)             (Skeleton*     skeleton);
    void                    (*SetSkin)             (Skeleton*     skeleton,     const char* skinName);

    /**
     * Reset Skeleton bones to setup pose.
     */
    void                    (*ResetBones)          (Skeleton* skeleton);

    /**
     * Reset Skeleton slots to setup pose.
     */
    void                    (*ResetSlots)          (Skeleton* skeleton);


    /**
     * Find SkeletonAttachmentData by curSkinData,
     * if not found will find by default SkinData,
     * if not found yet then return NULL.
     */
    SkeletonAttachmentData* (*GetAttachmentData)   (
                                                       Skeleton*   skeleton,
                                                       const char* slotName,
                                                       const char* attachmentName
                                                   );

    /**
     * Get SubMesh in Mesh that bind in SkeletonAttachmentData.
     */
    SubMesh*                (*GetAttachmentSubMesh)(Skeleton* skeleton, SkeletonAttachmentData* skeletonAttachmentData);

    /**
     * Poses the skeleton at the specified time for animationData.
     *
     * mixPercent: percent  poses effect to apply, range in [0, 1]
     * mixPercent: 1.0f will 100% apply this time pose effect
     * mixPercent: 0.0f will 0%   apply this time pose effect
     * time      : seconds
     */
    void                     (*Apply)              (
                                                        Skeleton*              skeleton,
                                                        SkeletonAnimationData* animationData,
                                                        float                   time,
                                                        float                   mixPercent
                                                   );
};


extern struct ASkeleton ASkeleton[1];


/**
 * Draw Skeleton.
 */
static inline void ASkeleton_Draw(Skeleton* skeleton)
{
    ADrawable->Draw(skeleton->drawable);
}


/**
 * Get SkeletonAnimationData by animationName.
 */
static inline SkeletonAnimationData* ASkeleton_GetAnimationData(Skeleton* skeleton, const char* animationName)
{
    return AArrayStrMap_Get
           (
                skeleton->skeletonData->animationDataMap,
                animationName,
                SkeletonAnimationData*
           );
}


/**
 * Get SubMesh in Mesh that bind in SkeletonAttachmentData.
 */
static inline SubMesh* ASkeleton_GetSubMesh(Skeleton* skeleton, const char* slotName, const char* attachmentName)
{
    return ASkeleton->GetAttachmentSubMesh
           (
               skeleton,
               ASkeleton->GetAttachmentData(skeleton, slotName, attachmentName)
           );
}


/**
 * Get Skeleton width.
 */
static inline float ASkeleton_GetWidth(Skeleton* skeleton)
{
    return skeleton->skeletonData->width;
}


/**
 * Get Skeleton height.
 */
static inline float ASkeleton_GetHeight(Skeleton* skeleton)
{
    return skeleton->skeletonData->height;
}


#endif
