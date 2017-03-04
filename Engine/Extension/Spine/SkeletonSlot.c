/*
 *
 *
 *  Created on: 2013-7-7
 *      Author: scott.cgi
 */

#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"

static inline void SetAttachmentToBone(SkeletonSlot* slot)
{
	SkeletonAttachmentData* attachmentData = slot->attachmentData;
	Drawable*               drawable;

	switch (attachmentData->type)
	{
		case skeleton_attachment_region:
		{
			SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*) attachmentData->subPtr;
			SubMesh*                      subMesh              = AArrayListGet
																 (
																	 AArrayListGetPtr(slot->skeleton->meshList, regionAttachmentData->meshIndex, Mesh)->children,
																	 regionAttachmentData->subMeshIndex,
																	 SubMesh*
																 );

			drawable                                           = subMesh->drawable;
			drawable->width                                    = regionAttachmentData->width;
			drawable->height                                   = regionAttachmentData->height;

			ADrawableSetParent   (drawable, slot->bone->drawable);
			ADrawableSetPosition2(drawable, regionAttachmentData->x, regionAttachmentData->y);
			ADrawableSetRotationZ(drawable, regionAttachmentData->rotationZ);
			ADrawableSetScale2   (drawable, regionAttachmentData->scaleX, regionAttachmentData->scaleY);
		}
		break;

		case skeleton_attachment_mesh:
		{
			SkeletonMeshAttachmentData* meshAttachmentData = (SkeletonMeshAttachmentData*) attachmentData->subPtr;
			SubMesh*                    subMesh            = AArrayListGet
															 (
																 AArrayListGetPtr(slot->skeleton->meshList, meshAttachmentData->meshIndex, Mesh)->children,
																 meshAttachmentData->subMeshIndex,
																 SubMesh*
															 );

			drawable                                       = subMesh->drawable;
            drawable->width                                = meshAttachmentData->width;
            drawable->height                               = meshAttachmentData->height;
			ADrawableSetParent(drawable, slot->bone->drawable);
		}
		break;

		case skeleton_attachment_skinned_mesh:
		{
			SkeletonSkinnedMeshAttachmentData* skinnedMeshAttachmentData = (SkeletonSkinnedMeshAttachmentData*) attachmentData->subPtr;
			SkeletonMeshAttachmentData*        meshAttachmentData        = skinnedMeshAttachmentData->meshAttachmentData;
			SubMesh*                           subMesh                   = AArrayListGet
																	   	   (
																			 AArrayListGetPtr(slot->skeleton->meshList, meshAttachmentData->meshIndex, Mesh)->children,
																			 meshAttachmentData->subMeshIndex,
																			 SubMesh*
																		   );

			float*                             positions                 = AArrayGetData(subMesh->positionArr,                       float);
			int*                               bones                     = AArrayGetData(skinnedMeshAttachmentData->boneArr,         int);
			float*                             weights                   = AArrayGetData(skinnedMeshAttachmentData->weightArr,       float);
			float*                             weightVertices            = AArrayGetData(skinnedMeshAttachmentData->weightVertexArr, float);

			for (int i = 0, w = 0, v = 0, p = 0; i < skinnedMeshAttachmentData->boneArr->length; p += 3)
			{
				float wx = 0.0f;
				float wy = 0.0f;
				int   k  = bones[i] + i + 1;
				i++;

				for (; i < k; i++, w++, v += 3)
				{
					int           boneIndex  = bones[i];
					SkeletonBone* bone       = AArrayGetPtr(slot->skeleton->boneArr, boneIndex, SkeletonBone);
					float         weight     = weights[w];
					Vector2       v2[1];

					AMatrix->MultiplyMV2
					(
						bone->drawable->modelMatrix,
						weightVertices[v],
						weightVertices[v + 1],
						v2
					);

					wx += v2->x * weight;
					wy += v2->y * weight;
				}

				positions[p]     = wx;
				positions[p + 1] = wy;
			}

			drawable = subMesh->drawable;
			// we compute final world coordinate, so no parent
			ADrawableSetParent(drawable, NULL);
		}
		break;

		default:
			return;
	}

	if (memcmp(drawable->color, slot->color, sizeof(Color)) != 0)
	{
		ADrawableSetColor(drawable, slot->color);
	}

	if (ADrawableCheckVisible(drawable) == false)
	{
		ADrawableSetVisible(drawable);
	}
}


static void SetAttachmentData(SkeletonSlot* slot, SkeletonAttachmentData* attachmentData)
{
	if (slot->attachmentData == attachmentData)
	{
		return;
	}

	if (slot->attachmentData != NULL)
	{
		SubMesh* subMesh = ASkeleton->GetAttachmentSubMesh(slot->skeleton, slot->attachmentData);

		if (subMesh != NULL)
		{
			ADrawableSetInVisible(subMesh->drawable);
		}
	}

	slot->attachmentData = attachmentData;
	*slot->color         = *slot->slotData->color;

	if (attachmentData != NULL)
	{
		SetAttachmentToBone(slot);
	}
}


static void SetToSetupPose(SkeletonSlot* slot)
{
	const char* attachmentName = slot->slotData->attachmentName;

	if (attachmentName != NULL)
	{
		SetAttachmentData
		(
			slot,
			ASkeleton->GetAttachmentData(slot->skeleton, slot->slotData->name, attachmentName)
		);
	}
}


static void Init(SkeletonSlotData* slotData, Skeleton* skeleton, SkeletonSlot* outSlot)
{
	outSlot->slotData       = slotData;
	outSlot->skeleton       = skeleton;
	outSlot->attachmentData = NULL;
	outSlot->bone           = AArrayStrMapGet(skeleton->boneMap, slotData->boneData->name, SkeletonBone*);

	SetToSetupPose(outSlot);
}


static SkeletonSlot* Create(SkeletonSlotData* slotData, Skeleton* skeleton)
{
	SkeletonSlot* slot = (SkeletonSlot*) malloc(sizeof(SkeletonSlot));
	Init(slotData, skeleton, slot);

	return slot;
}

struct ASkeletonSlot ASkeletonSlot[1] =
{
	Create,
	Init,
	SetAttachmentData,
	SetToSetupPose,
};

