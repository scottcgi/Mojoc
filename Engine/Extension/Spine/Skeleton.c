/*
 *
 *
 *  Created on: 2013-7-3
 *      Author: scott.cgi
 */

#include "Engine/Extension/Spine/Skeleton.h"

#include "Engine/Toolkit/Define/StructFrom.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Define/Def.h"
#include "Engine/Extension/Spine/SkeletonTimeline.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"

static inline SubMesh* GetAttachmentSubMesh(Skeleton* skeleton, SkeletonAttachmentData* skeletonAttachmentData)
{
	int meshIndex    = *(int*) ((char*) (skeletonAttachmentData)->subPtr + skeletonAttachmentMeshOffset   [(skeletonAttachmentData)->type]);
	int subMeshIndex = *(int*) ((char*) (skeletonAttachmentData)->subPtr + skeletonAttachmentSubMeshOffset[(skeletonAttachmentData)->type]);

	return AArrayListGet
		   (
			   AArrayListGetPtr(skeleton->meshList, meshIndex, Mesh)->children,
			   subMeshIndex,
			   SubMesh*
		   );
}


static inline void InitBone(Skeleton* skeleton, SkeletonData* skeletonData)
{
	Drawable*    root              = skeleton->drawable;
	ArrayStrMap* boneMap           = skeleton->boneMap;

	skeleton->boneArr              = AArray->Create(sizeof(SkeletonBone), skeletonData->boneDataOrderArr->length);
	AArrayStrMap->InitWithCapacity(sizeof(SkeletonBone*), skeletonData->boneDataOrderArr->length, boneMap);

	SkeletonBone*      bones       = AArrayGetData(skeleton->boneArr,              SkeletonBone);
	SkeletonBoneData** boneDatas   = AArrayGetData(skeletonData->boneDataOrderArr, SkeletonBoneData*);

	for (int i = 0; i < skeleton->boneArr->length; i++)
	{
		SkeletonBoneData* boneData = boneDatas[i];
		SkeletonBone*     bone     = bones + i;
		ASkeletonBone->Init(boneData, bone);

		ALogD("Create bone = %s", boneData->name);
		AArrayStrMapPut(boneMap, boneData->name, bone);

		if (boneData->parent == NULL)
		{
			ADrawableSetParent(bone->drawable, root);
		}
		else
		{
			SkeletonBone* boneParent = AArrayStrMapGet(boneMap, boneData->parent->name, SkeletonBone*);

			ALogA(boneParent != NULL, "bone parent = %s, not found", boneData->parent->name);

			ADrawableSetParent(bone->drawable, boneParent->drawable);
		}
	}
}


static inline void InitSlot(Skeleton* skeleton, SkeletonData* skeletonData)
{
	ArrayStrMap*       slotMap       = skeleton->slotMap;

	skeleton->slotArr                = AArray->Create(sizeof(SkeletonSlot),  skeletonData->slotDataOrderArr->length);
	skeleton->slotOrderArr           = AArray->Create(sizeof(SkeletonSlot*), skeletonData->slotDataOrderArr->length);

	AArrayStrMap->InitWithCapacity(sizeof(SkeletonSlot*), skeletonData->slotDataOrderArr->length, slotMap);

	SkeletonSlot**     slotOrders    = AArrayGetData(skeleton->slotOrderArr,         SkeletonSlot*);
	SkeletonSlotData** slotDatas     = AArrayGetData(skeletonData->slotDataOrderArr, SkeletonSlotData*);

	for (int i = 0; i < skeleton->slotArr->length; i++)
	{
		SkeletonSlotData* slotData   = slotDatas[i];
		SkeletonSlot*     slot       = AArrayGetPtr(skeleton->slotArr, i, SkeletonSlot);

		ASkeletonSlot->Init(slotData, skeleton, slot);

		slotOrders[i]                = slot;
		ALogD("Create slot = %s", slotData->name);

		AArrayStrMapPut(slotMap, slotData->name, slot);
	}
}


static void SetSkin(Skeleton* skeleton, const char* skinName)
{
	ArrayStrMap*      skinDataMap = skeleton->skeletonData->skinDataMap;
	SkeletonSkinData* skinData    = AArrayStrMapGet(skinDataMap, skinName, SkeletonSkinData*);

	ALogA(skinData != NULL, "SetSkin not found skin by name = %s", skinName);

	if (skeleton->curSkinData == skinData)
	{
		return;
	}

    if (skeleton->curSkinData != skeleton->skeletonData->skinDataDefault)
    {
        // first invisible before skin
        for (int i = 0; i < skeleton->curSkinData->slotAttachmentMap->arrayList->size; i++)
        {
            SkeletonSlot* slot = AArrayStrMapGet
                                 (
                                    skeleton->slotMap,
                                    AArrayStrMap->GetKey(skeleton->curSkinData->slotAttachmentMap, i),
                                    SkeletonSlot*
                                 );

            if (slot->attachmentData != NULL)
            {
                SubMesh* subMesh = ASkeleton->GetAttachmentSubMesh(slot->skeleton, slot->attachmentData);

                if (subMesh != NULL)
                {
                    ADrawableSetInVisible(subMesh->drawable);
                }

                slot->attachmentData = NULL;
            }
        }
    }

//--------------------------------------------------------------------------------------------------

	for (int i = 0; i < skinData->slotAttachmentMap->arrayList->size; i++)
	{
		SkeletonSlot* slot = AArrayStrMapGet
                             (
                                 skeleton->slotMap,
                                 AArrayStrMap->GetKey(skinData->slotAttachmentMap, i),
                                 SkeletonSlot*
                             );

		if (slot != NULL && slot->slotData->attachmentName != NULL)
		{
			ASkeletonSlot->SetAttachmentData
			(
				slot,
				AArrayStrMapGet
				(
                    AArrayStrMapGetAt(skinData->slotAttachmentMap, i, ArrayStrMap*),
					slot->slotData->attachmentName,
					SkeletonAttachmentData*
				)
			);
		}
	}

    skeleton->curSkinData = skinData;
}


static void ResetBones(Skeleton* skeleton)
{
	for (int i = 0; i < skeleton->boneArr->length; i++)
	{
		ASkeletonBone->SetToSetupPose
		(
			AArrayGetPtr(skeleton->boneArr, i, SkeletonBone)
		);
	}
}

static void ResetSlots(Skeleton* skeleton)
{
	for (int i = 0; i < skeleton->slotOrderArr->length; i++)
	{
		ASkeletonSlot->SetToSetupPose
		(
			AArrayGet(skeleton->slotOrderArr, i, SkeletonSlot*)
		);
	}
}

static SkeletonAttachmentData* GetAttachmentData(Skeleton* skeleton, const char* slotName, const char* attachmentName)
{
	SkeletonAttachmentData* attachmentData = ASkeletonData->GetAttachmentDataBySkinData
			                                 (
												 skeleton->curSkinData,
												 slotName,
												 attachmentName
											 );

	if (attachmentData == NULL && skeleton->skeletonData->skinDataDefault != skeleton->curSkinData)
	{
		attachmentData = ASkeletonData->GetAttachmentDataBySkinData
				         (
							 skeleton->skeletonData->skinDataDefault,
							 slotName,
							 attachmentName
						 );
	}

	return attachmentData;
}

static void Release(Skeleton* skeleton)
{
	free(skeleton->boneArr);
	skeleton->boneArr      = NULL;

	free(skeleton->slotArr);
	skeleton->slotArr      = NULL;

	free(skeleton->slotOrderArr);
	skeleton->slotOrderArr = NULL;

	for (int j = 0; j < skeleton->meshList->size; j++)
	{
		AMesh->Release(AArrayListGetPtr(skeleton->meshList, j, Mesh));
	}
	AArrayList->Release(skeleton->meshList);

	AArrayStrMap->Release(skeleton->boneMap);
	AArrayStrMap->Release(skeleton->slotMap);
}

static void Apply(Skeleton* skeleton, SkeletonAnimationData* animationData, float time, float mixPercent)
{
	for (int i = 0; i < animationData->timelineArr->size; i++)
	{
		SkeletonTimeline* timeline = AArrayListGet(animationData->timelineArr, i, SkeletonTimeline*);
		timeline->Apply(timeline, skeleton, time, mixPercent);
	}
}

static void Draw(Drawable* drawable)
{
	Skeleton* skeleton = StructFrom2(drawable, Skeleton);

	for (int i = 0; i < skeleton->boneArr->length; i++)
	{
		ADrawable->Draw(AArrayGetPtr(skeleton->boneArr, i, SkeletonBone)->drawable);
	}

	Mesh*    preMesh     = NULL;
	SubMesh* preSubMesh  = NULL;
	SubMesh* startSubMesh;
	SubMesh* subMesh;

	for (int i = 0; i < skeleton->slotOrderArr->length; i++)
	{
		SkeletonSlot* slot = AArrayGet(skeleton->slotOrderArr, i, SkeletonSlot*);

		if (slot->attachmentData == NULL || slot->attachmentData->type == skeleton_attachment_boundingbox)
		{
			continue;
		}

		subMesh = GetAttachmentSubMesh(skeleton, slot->attachmentData);

		if (subMesh->parent != preMesh)
		{
			if (preMesh != NULL)
			{
				AMeshPushDrawRange(preSubMesh->parent, startSubMesh->index, preSubMesh->index);
				ADrawable->Draw(preSubMesh->parent->drawable);
			}

			startSubMesh = subMesh;
			preMesh      = subMesh->parent;
		}

		preSubMesh = subMesh;
	}

	if (preSubMesh != NULL)
	{
		AMeshPushDrawRange(preSubMesh->parent, startSubMesh->index, preSubMesh->index);
		ADrawable->Draw(preSubMesh->parent->drawable);
	}
}


static inline void InitMeshList(Skeleton* skeleton, SkeletonData* skeletonData)
{
	AArrayList->InitWithCapacity(sizeof(Mesh), skeletonData->textureAtlas->textureList->size, skeleton->meshList);

	for (int i = 0; i < skeletonData->textureAtlas->textureList->size; i++)
	{
		Texture* texture = AArrayListGet(skeletonData->textureAtlas->textureList, i, Texture*);
		Mesh*    mesh    = AArrayListGetPtrAdd(skeleton->meshList, Mesh);

		AMesh->Init(texture, mesh);
	}

	for (int i = 0; i < skeletonData->attachmentDataList->size; i++)
	{
		SkeletonAttachmentData* attachmentData = AArrayListGet(skeletonData->attachmentDataList, i, SkeletonAttachmentData*);
		SubMesh*                subMesh;

		switch (attachmentData->type)
		{
			case skeleton_attachment_region:
			{
				SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*) attachmentData->subPtr;
				subMesh                                            =  AMesh->AddChildWithQuad
																	  (
																		 AArrayListGetPtr
																		 (
																			 skeleton->meshList,
																			 regionAttachmentData->meshIndex,
																			 Mesh
																		 ),
																		 regionAttachmentData->quad
																	  );

				if (ASkeletonData->scale != 1.0f)
				{
					for (int l = 0; l < subMesh->positionArr->length; l++)
					{
						AArrayGet(subMesh->positionArr, l, float) *= ASkeletonData->scale;
					}

					subMesh->drawable->width  = regionAttachmentData->width;
					subMesh->drawable->height = regionAttachmentData->height;
				}
			}
			break;


			SkeletonMeshAttachmentData* meshData;
			/* no break */

			case skeleton_attachment_skinned_mesh:
				meshData = ((SkeletonSkinnedMeshAttachmentData*) attachmentData->subPtr)->meshAttachmentData;
				/* no break */
			case skeleton_attachment_mesh:
			{
				meshData = (SkeletonMeshAttachmentData*) attachmentData->subPtr;

				if (meshData->isUVMappedInTexture == false)
				{
					float texData[quad_uv_num];
					AQuad->GetQuadUV
					(
						meshData->quad,
						AArrayListGet(skeletonData->textureAtlas->textureList, meshData->meshIndex, Texture*),
						texData
					);

					float  left    = texData[0];
					float  top     = texData[1];
					float  width   = texData[4] - left;
					float  height  = texData[5] - top;

					float* uvs     = AArrayGetData(meshData->uvArr, float);
					for (int l = 0; l < meshData->uvArr->length; l += 2)
					{
						uvs[l]     = left + uvs[l]     * width;
						uvs[l + 1] = top  + uvs[l + 1] * height;
					}

					meshData->isUVMappedInTexture = true;
				}

				subMesh = AMesh->AddChildWithData
						  (
							  AArrayListGetPtr
							  (
								  skeleton->meshList,
								  meshData->meshIndex,
								  Mesh
							  ),
							  meshData->vertexArr,
							  meshData->uvArr,
							  meshData->triangleArr
						  );

				if (ASkeletonData->scale != 1.0f)
				{
					subMesh->drawable->width  = meshData->width;
					subMesh->drawable->height = meshData->height;
				}
			}
			break;

			default:
				break;
		}

		// first born invisible
		ADrawableSetInVisible(subMesh->drawable);
	}

	for (int i = 0; i < skeleton->meshList->size; i++)
	{
		AMesh->GenerateBuffer(AArrayListGetPtr(skeleton->meshList, i, Mesh));
	}
}


static void Init(SkeletonData* skeletonData, Skeleton* outSkeleton)
{
	ADrawable->Init(outSkeleton->drawable);

	outSkeleton->drawable->Draw     = Draw;
	outSkeleton->drawable->width    = skeletonData->width;
	outSkeleton->drawable->height   = skeletonData->height;
	outSkeleton->skeletonData       = skeletonData;
	outSkeleton->curSkinData        = outSkeleton->skeletonData->skinDataDefault;
	outSkeleton->FireSkeletonEvent  = NULL;

	InitMeshList(outSkeleton, skeletonData);
	InitBone    (outSkeleton, skeletonData);
	InitSlot    (outSkeleton, skeletonData);

	//	Release(outSkeleton);
	//	ALogA(0, "stop");
}


static Skeleton* Create(SkeletonData* skeletonData)
{
	Skeleton* skeleton = (Skeleton*) malloc(sizeof(Skeleton));
	Init(skeletonData, skeleton);

	return skeleton;
}

struct ASkeleton ASkeleton[1] =
{
	Create,
	Init,
	Release,
	SetSkin,
	ResetBones,
	ResetSlots,
	GetAttachmentData,
	GetAttachmentSubMesh,
	Apply,
};
