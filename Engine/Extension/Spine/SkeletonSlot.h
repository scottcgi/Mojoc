/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-7-7
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef SKELETON_SLOT_H
#define SKELETON_SLOT_H


#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/Skeleton.h"


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
    void          (*SetToSetupPose)   (SkeletonSlot* slot);
};


extern struct ASkeletonSlot ASkeletonSlot[1];


static inline SkeletonBoundingBoxAttachmentData* ASkeletonSlot_GetBoundingBox(SkeletonSlot* slot)
{
    return (SkeletonBoundingBoxAttachmentData*) slot->attachmentData->childPtr;
}


static inline SkeletonRegionAttachmentData* ASkeletonSlot_GetRegion(SkeletonSlot* slot)
{
    return (SkeletonRegionAttachmentData*) slot->attachmentData->childPtr;
}


static inline SkeletonMeshAttachmentData* ASkeletonSlot_GetMesh(SkeletonSlot* slot)
{
    return (SkeletonMeshAttachmentData*) slot->attachmentData->childPtr;
}


static inline SkeletonSkinnedMeshAttachmentData* ASkeletonSlot_GetSkinned(SkeletonSlot* slot)
{
    return (SkeletonSkinnedMeshAttachmentData*) slot->attachmentData->childPtr;
}


#endif
