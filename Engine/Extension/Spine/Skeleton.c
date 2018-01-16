/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-7-3
 * Author: scott.cgi
 */

#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Head/Define.h"
#include "Engine/Extension/Spine/SkeletonTimeline.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"


static inline SubMesh* GetAttachmentSubMesh(Skeleton* skeleton, SkeletonAttachmentData* skeletonAttachmentData)
{
    int meshIndex    = *(int*) (
                                   (char*) (skeletonAttachmentData)->childPtr +
                                   skeletonAttachmentMeshOffset[(skeletonAttachmentData)->type]
                               );

    int subMeshIndex = *(int*) (
                                   (char*) (skeletonAttachmentData)->childPtr +
                                   skeletonAttachmentSubMeshOffset[(skeletonAttachmentData)->type]
                               );

    return AArrayList_Get
           (
               AArrayList_GetPtr(skeleton->meshList, meshIndex, Mesh)->childList,
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

    SkeletonBone*      bones       = AArray_GetData(skeleton->boneArr,              SkeletonBone);
    SkeletonBoneData** boneDatas   = AArray_GetData(skeletonData->boneDataOrderArr, SkeletonBoneData*);

    for (int i = 0; i < skeleton->boneArr->length; i++)
    {
        SkeletonBoneData* boneData = boneDatas[i];
        SkeletonBone*     bone     = bones + i;
        ASkeletonBone->Init(boneData, bone);

        AArrayStrMap_TryPut(boneMap, boneData->name, bone);

        if (boneData->parent == NULL)
        {
            ADrawable_SetParent(bone->drawable, root);
        }
        else
        {
            SkeletonBone* boneParent = AArrayStrMap_Get(boneMap, boneData->parent->name, SkeletonBone*);

            ALog_A(boneParent != NULL, "ASkeleton InitBone bone parent = %s, not found", boneData->parent->name);

            ADrawable_SetParent(bone->drawable, boneParent->drawable);
        }
    }
}


static inline void InitSlot(Skeleton* skeleton, SkeletonData* skeletonData)
{
    ArrayStrMap*       slotMap       = skeleton->slotMap;

    skeleton->slotArr                = AArray->Create(sizeof(SkeletonSlot),  skeletonData->slotDataOrderArr->length);
    skeleton->slotOrderArr           = AArray->Create(sizeof(SkeletonSlot*), skeletonData->slotDataOrderArr->length);

    AArrayStrMap->InitWithCapacity(sizeof(SkeletonSlot*), skeletonData->slotDataOrderArr->length, slotMap);

    SkeletonSlot**     slotOrders    = AArray_GetData(skeleton->slotOrderArr,         SkeletonSlot*);
    SkeletonSlotData** slotDatas     = AArray_GetData(skeletonData->slotDataOrderArr, SkeletonSlotData*);

    for (int i = 0; i < skeleton->slotArr->length; i++)
    {
        SkeletonSlotData* slotData   = slotDatas[i];
        SkeletonSlot*     slot       = AArray_GetPtr(skeleton->slotArr, i, SkeletonSlot);

        ASkeletonSlot->Init(slotData, skeleton, slot);

        slotOrders[i]                = slot;

        AArrayStrMap_TryPut(slotMap, slotData->name, slot);
    }
}


static void SetSkin(Skeleton* skeleton, char* skinName)
{
    ArrayStrMap*      skinDataMap = skeleton->skeletonData->skinDataMap;
    SkeletonSkinData* skinData    = AArrayStrMap_Get(skinDataMap, skinName, SkeletonSkinData*);

    ALog_A(skinData != NULL, "ASkeleton SetSkin not found skin by name = %s", skinName);

    if (skeleton->curSkinData == skinData)
    {
        return;
    }

    if (skeleton->curSkinData != skeleton->skeletonData->skinDataDefault)
    {
        // first invisible before skin
        for (int i = 0; i < skeleton->curSkinData->slotAttachmentMap->elementList->size; i++)
        {
            SkeletonSlot* slot = AArrayStrMap_Get
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
                    ADrawable_SetInVisible(subMesh->drawable);
                }

                slot->attachmentData = NULL;
            }
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    for (int i = 0; i < skinData->slotAttachmentMap->elementList->size; i++)
    {
        SkeletonSlot* slot = AArrayStrMap_Get
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
                AArrayStrMap_Get
                (
                    AArrayStrMap_GetAt(skinData->slotAttachmentMap, i, ArrayStrMap*),
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
            AArray_GetPtr(skeleton->boneArr, i, SkeletonBone)
        );
    }
}


static void ResetSlots(Skeleton* skeleton)
{
    for (int i = 0; i < skeleton->slotOrderArr->length; i++)
    {
        ASkeletonSlot->SetToSetupPose
        (
            AArray_Get(skeleton->slotOrderArr, i, SkeletonSlot*)
        );
    }
}


static SkeletonAttachmentData* GetAttachmentData(Skeleton* skeleton, char* slotName, char* attachmentName)
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
        AMesh->Release(AArrayList_GetPtr(skeleton->meshList, j, Mesh));
    }
    AArrayList->Release(skeleton->meshList);

    AArrayStrMap->Release(skeleton->boneMap);
    AArrayStrMap->Release(skeleton->slotMap);
}


static void Apply(Skeleton* skeleton, SkeletonAnimationData* animationData, float time, float mixPercent)
{
    for (int i = 0; i < animationData->timelineArr->size; i++)
    {
        SkeletonTimeline* timeline = AArrayList_Get(animationData->timelineArr, i, SkeletonTimeline*);
        timeline->Apply(timeline, skeleton, time, mixPercent);
    }
}


static void Draw(Drawable* drawable)
{
    Skeleton* skeleton = AStruct_GetParent2(drawable, Skeleton);

    for (int i = 0; i < skeleton->boneArr->length; i++)
    {
        ADrawable->Draw(AArray_GetPtr(skeleton->boneArr, i, SkeletonBone)->drawable);
    }

    Mesh*    preMesh      = NULL;
    SubMesh* preSubMesh   = NULL;
    SubMesh* startSubMesh = NULL;
    SubMesh* subMesh;

    for (int i = 0; i < skeleton->slotOrderArr->length; i++)
    {
        SkeletonSlot* slot = AArray_Get(skeleton->slotOrderArr, i, SkeletonSlot*);

        if (slot->attachmentData == NULL || slot->attachmentData->type == SkeletonAttachmentDataType_BoundingBox)
        {
            continue;
        }

        subMesh = GetAttachmentSubMesh(skeleton, slot->attachmentData);

        if (subMesh->parent != preMesh)
        {
            if (preMesh != NULL)
            {
                AMesh_PushDrawRange(preSubMesh->parent, startSubMesh->index, preSubMesh->index);
                ADrawable->Draw(preSubMesh->parent->drawable);
            }

            startSubMesh = subMesh;
            preMesh      = subMesh->parent;
        }

        preSubMesh = subMesh;
    }

    if (preSubMesh != NULL)
    {
        AMesh_PushDrawRange(preSubMesh->parent, startSubMesh->index, preSubMesh->index);
        ADrawable->Draw(preSubMesh->parent->drawable);
    }
}


static inline void InitMeshList(Skeleton* skeleton, SkeletonData* skeletonData)
{
    AArrayList->InitWithCapacity(sizeof(Mesh), skeletonData->textureAtlas->textureList->size, skeleton->meshList);

    for (int i = 0; i < skeletonData->textureAtlas->textureList->size; i++)
    {
        Texture* texture = AArrayList_Get      (skeletonData->textureAtlas->textureList, i, Texture*);
        Mesh*    mesh    = AArrayList_GetPtrAdd(skeleton->meshList, Mesh);

        AMesh->Init(texture, mesh);
    }

    for (int i = 0; i < skeletonData->attachmentDataList->size; i++)
    {
        SkeletonAttachmentData* attachmentData = AArrayList_Get(skeletonData->attachmentDataList, i, SkeletonAttachmentData*);
        SubMesh*                subMesh        = NULL;

        switch (attachmentData->type)
        {
            case SkeletonAttachmentDataType_BoundingBox:
                break;
                
            case SkeletonAttachmentDataType_Region:
            {
                SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*) attachmentData->childPtr;
                subMesh                                            =  AMesh->AddChildWithQuad
                                                                      (
                                                                         AArrayList_GetPtr
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
                        AArray_Get(subMesh->positionArr, l, float) *= ASkeletonData->scale;
                    }

                    subMesh->drawable->width  = regionAttachmentData->width;
                    subMesh->drawable->height = regionAttachmentData->height;
                }

                break;
            }

            SkeletonMeshAttachmentData* meshData;

            case SkeletonAttachmentDataType_SkinnedMesh:
                meshData = ((SkeletonSkinnedMeshAttachmentData*) attachmentData->childPtr)->meshAttachmentData;
                
            case SkeletonAttachmentDataType_Mesh:
            {
                meshData = (SkeletonMeshAttachmentData*) attachmentData->childPtr;

                if (meshData->isUVMappedInTexture == false)
                {
                    float texData[Quad_UVNum];
                    AQuad->GetQuadUV
                    (
                        meshData->quad,
                        AArrayList_Get(skeletonData->textureAtlas->textureList, meshData->meshIndex, Texture*),
                        texData
                    );

                    float  left    = texData[0];
                    float  top     = texData[1];
                    float  width   = texData[4] - left;
                    float  height  = texData[5] - top;

                    float* uvs     = AArray_GetData(meshData->uvArr, float);
                    for (int l = 0; l < meshData->uvArr->length; l += 2)
                    {
                        uvs[l]     = left + uvs[l]     * width;
                        uvs[l + 1] = top  + uvs[l + 1] * height;
                    }

                    meshData->isUVMappedInTexture = true;
                }

                subMesh = AMesh->AddChildWithData
                          (
                              AArrayList_GetPtr
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

                break;
            }
        }

        // first born invisible
        ADrawable_SetInVisible(subMesh->drawable);
    }

    for (int i = 0; i < skeleton->meshList->size; i++)
    {
        AMesh->GenerateBuffer(AArrayList_GetPtr(skeleton->meshList, i, Mesh));
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

    // Release(outSkeleton);
    // ALog_A(false, "stop");
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
