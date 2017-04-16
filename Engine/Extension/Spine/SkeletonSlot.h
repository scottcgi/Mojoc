/*
 *
 *
 *  Created on: 2013-7-7
 *      Author: scott.cgi
 */

#ifndef skeleton_slot_h
#define skeleton_slot_h


#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/Skeleton.h"


typedef struct
{
	Color                   get_only color[1];
	SkeletonSlotData*       get_only slotData;
	Skeleton*               get_only skeleton;
	SkeletonBone*           get_only bone;
	SkeletonAttachmentData* get_only attachmentData;
}
SkeletonSlot;


struct ASkeletonSlot
{
	SkeletonSlot* (*Create)           (SkeletonSlotData* slotData, Skeleton* skeleton);
	void          (*Init)             (SkeletonSlotData* slotData, Skeleton* skeleton, SkeletonSlot* out_param slot);

	void          (*SetAttachmentData)(SkeletonSlot* slot, SkeletonAttachmentData* attachmentData);
	void          (*SetToSetupPose)   (SkeletonSlot* slot);
};


extern struct ASkeletonSlot ASkeletonSlot[1];


static inline SkeletonBoundingboxAttachmentData* ASkeletonSlotGetBoundingBox(SkeletonSlot* slot)
{
	return (SkeletonBoundingboxAttachmentData*) slot->attachmentData->subPtr;
}


static inline SkeletonRegionAttachmentData* ASkeletonSlotGetRegion(SkeletonSlot* slot)
{
	return (SkeletonRegionAttachmentData*) slot->attachmentData->subPtr;
}


static inline SkeletonMeshAttachmentData* ASkeletonSlotGetMesh(SkeletonSlot* slot)
{
	return (SkeletonMeshAttachmentData*) slot->attachmentData->subPtr;
}


static inline SkeletonSkinnedMeshAttachmentData* ASkeletonSlotGetSkinned(SkeletonSlot* slot)
{
	return (SkeletonSkinnedMeshAttachmentData*) slot->attachmentData->subPtr;
}


#endif
