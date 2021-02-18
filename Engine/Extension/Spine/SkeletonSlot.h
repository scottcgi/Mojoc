/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-7-7
 * Update   : 2019-2-12
 * Author   : scott.cgi
 */


#ifndef SKELETON_SLOT_H
#define SKELETON_SLOT_H


#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/Skeleton.h"


/**
 * The skeleton slot can provide color and texture.
 */
typedef struct
{
    Color                   color[1];
    SkeletonBone*           bone;
    SkeletonSlotData*       slotData;
    Skeleton*               skeleton;
    SkeletonAttachmentData* attachmentData;
}
SkeletonSlot;


struct ASkeletonSlot
{
    SkeletonSlot* (*Create)           (SkeletonSlotData* slotData, Skeleton* skeleton);
    void          (*Init)             (SkeletonSlotData* slotData, Skeleton* skeleton, SkeletonSlot* outSlot);

    void          (*SetAttachmentData)(SkeletonSlot* slot, SkeletonAttachmentData* attachmentData);

    /**
     * Set slot to setup pose.
     */
    void          (*SetToSetupPose)   (SkeletonSlot* slot);
};


extern struct ASkeletonSlot ASkeletonSlot[1];


/**
 * Get boundingBox in slot.
 */
static inline SkeletonBoundingBoxAttachmentData* ASkeletonSlot_GetBoundingBox(SkeletonSlot* slot)
{
    return slot->attachmentData->childPtr;
}


/**
 * Get region in slot.
 */
static inline SkeletonRegionAttachmentData* ASkeletonSlot_GetRegion(SkeletonSlot* slot)
{
    return slot->attachmentData->childPtr;
}


/**
 * Get mesh in slot.
 */
static inline SkeletonMeshAttachmentData* ASkeletonSlot_GetMesh(SkeletonSlot* slot)
{
    return slot->attachmentData->childPtr;
}


/**
 * Get skinned mesh in slot.
 */
static inline SkeletonSkinnedMeshAttachmentData* ASkeletonSlot_GetSkinnedMesh(SkeletonSlot* slot)
{
    return slot->attachmentData->childPtr;
}


#endif
