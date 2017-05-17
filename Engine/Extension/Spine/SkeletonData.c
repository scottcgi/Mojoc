/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2013-6-27
 * Author: scott.cgi
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Engine/Toolkit/Utils/Json.h>
#include <Engine/Toolkit/Utils/ArrayStrMap.h>

#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Toolkit/Utils/Json.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Extension/Spine/SkeletonTimeline.h"
#include "Engine/Graphics/OpenGL/GLTool.h"


static ArrayStrMap(filePath, SkeletonData*) skeletonDataMap[1] = AArrayStrMap_Init(SkeletonData*, 20);


static void Release(SkeletonData* skeletonData)
{
	bool isRemoved = AArrayStrMap->TryRemove(skeletonDataMap, skeletonData->filePath);
	ALog_A(isRemoved, "SkeletonData release not found %s in skeletonDataMap", skeletonData->filePath);

	// bone data
	ArrayStrMap* boneDataMap = skeletonData->boneDataMap;
	for (int i = 0; i < boneDataMap->elementList->size; i++)
	{
		free(AArrayStrMap_GetAt(boneDataMap, i, SkeletonBoneData*));
	}
	AArrayStrMap->Release(boneDataMap);

	// slot data
	ArrayStrMap* slotDataMap = skeletonData->slotDataMap;
	for (int i = 0; i < slotDataMap->elementList->size; i++)
	{
		SkeletonSlotData* slotData = AArrayStrMap_GetAt(slotDataMap, i, SkeletonSlotData*);
		if (slotData->attachmentDataList != NULL)
		{
			AArrayList->Release(slotData->attachmentDataList);
			free(slotData->attachmentDataList);
		}

		free(slotData);
	}
	AArrayStrMap->Release(slotDataMap);


	// event data
	ArrayStrMap* eventDataMap = skeletonData->eventDataMap;
	if (eventDataMap != NULL)
	{
		for(int i = 0; i < eventDataMap->elementList->size; i++)
		{
			free(AArrayStrMap_GetAt(eventDataMap, i, SkeletonEventData*));
		}

		AArrayStrMap->Release(eventDataMap);
	}

	// skin data
	ArrayStrMap* skinDataMap = skeletonData->skinDataMap;
	for (int i = 0; i < skinDataMap->elementList->size; i++)
	{
		SkeletonSkinData* skinData          = AArrayStrMap_GetAt(skinDataMap, i, SkeletonSkinData*);
		ArrayStrMap*      slotAttachmentMap = skinData->slotAttachmentMap;

		for (int j = 0; j < slotAttachmentMap->elementList->size; j++)
		{
			ArrayStrMap* attachmentDataMap = AArrayStrMap_GetAt(slotAttachmentMap, j, ArrayStrMap*);

			for (int k = 0; k < attachmentDataMap->elementList->size; k++)
			{
				free(AArrayStrMap_GetAt(attachmentDataMap, k, SkeletonAttachmentData*)->childPtr);
			}

			AArrayStrMap->Release(attachmentDataMap);
			free(attachmentDataMap);
		}

		AArrayStrMap->Release(slotAttachmentMap);
	}

	AArrayStrMap->Release(skinDataMap);

	free(skeletonData->boneDataOrderArr);
	skeletonData->boneDataOrderArr = NULL;

	free(skeletonData->slotDataOrderArr);
	skeletonData->slotDataOrderArr = NULL;


	// animation data
	ArrayStrMap* animationDataMap = skeletonData->animationDataMap;
	for (int j = 0; j < animationDataMap->elementList->size; j++)
	{
		SkeletonAnimationData* animationData = AArrayStrMap_GetAt(animationDataMap, j, SkeletonAnimationData*);
		ArrayList*             timelineList  = animationData->timelineArr;

		for (int i = 0; i < timelineList->size; i++)
		{
			SkeletonTimeline* timeline = AArrayList_Get(timelineList, i, SkeletonTimeline*);
			timeline->Release(timeline);
			free(timeline->childPtr);
		}
		AArrayList->Release(timelineList);

		free(animationData);
	}

	// attachment
	AArrayList->Release(skeletonData->attachmentDataList);\

	// texture atlas
	ATextureAtlas->Release(skeletonData->textureAtlas);
}


static SkeletonAttachmentData* GetAttachmentDataBySkinData(SkeletonSkinData* skinData, char* slotName, char* attachmentName)
{
	ArrayStrMap* slotAttachmentMap = skinData->slotAttachmentMap;
	ArrayStrMap* attachmentDataMap = AArrayStrMap_Get(slotAttachmentMap, slotName, ArrayStrMap*);

	if (attachmentDataMap == NULL)
	{
		// ALog_W("getAttachment from skin %s not found attachmentDataMap by slotName = %s", skinData->name, slotName);
		return NULL;
	}

	return AArrayStrMap_Get(attachmentDataMap, attachmentName, SkeletonAttachmentData*);
}


/**
 * return slot index in slotDataOrderArr or -1 not found
 */
static inline int FindSlotIndex(SkeletonData* skeletonData, char* slotName)
{
	SkeletonSlotData* slotData = AArrayStrMap_Get(skeletonData->slotDataMap, slotName, SkeletonSlotData*);
	ALog_A(slotData != NULL, "findSlotIndex SlotData = %s is not found", slotName);

	int  slotIndex             = -1;
	for (int j = 0; j < skeletonData->slotDataOrderArr->length; j++)
	{
		if (AArray_Get(skeletonData->slotDataOrderArr, j, SkeletonSlotData*) == slotData)
		{
			slotIndex = j;
			break;
		}
	}

	return slotIndex;
}


static inline void ReadBoneData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonArray*   boneArr            = AJsonObject->GetArray(root, "bones");
	ArrayStrMap* boneDataMap        = skeletonData->boneDataMap;
	AArrayStrMap->InitWithCapacity(sizeof(SkeletonBoneData*), boneArr->valueList->size, boneDataMap);
	
	// hold SkeletonBoneData in JOSN order
	skeletonData->boneDataOrderArr  = AArray->Create(sizeof(SkeletonBoneData*), boneArr->valueList->size);
	Array*       boneDataOrderArr   = skeletonData->boneDataOrderArr;

	for (int i = 0; i < boneArr->valueList->size; i++)
	{
		JsonObject*       jsonBone   = AJsonArray->GetObject(boneArr,  i);
		char*             parentName = AJsonObject->GetString(jsonBone, "parent", NULL);
		SkeletonBoneData* parent     = NULL;
		
		if (parentName != NULL)
		{
			parent = AArrayStrMap_Get(boneDataMap, parentName, SkeletonBoneData*);
			ALog_A(parent != NULL, "Parent SkeletonBoneData not fount: %s", parentName);
		}
		
		char* name                  =  AJsonObject->GetString(jsonBone, "name", NULL);
		int   nameLength            =  (int) strlen(name) + 1;

		SkeletonBoneData* boneData  = (SkeletonBoneData*) malloc(sizeof(SkeletonBoneData) + nameLength);
		boneData->name              = (char*) boneData + sizeof(SkeletonBoneData);
		memcpy(boneData->name, name, nameLength);

		boneData->parent            = parent;

		boneData->length            = AJsonObject->GetFloat(jsonBone, "length", 0.0f) * ASkeletonData->scale;
		boneData->x                 = AJsonObject->GetFloat(jsonBone, "x",      0.0f) * ASkeletonData->scale;
		boneData->y                 = AJsonObject->GetFloat(jsonBone, "y",      0.0f) * ASkeletonData->scale;

		boneData->length            = AGLTool_ToGLWidth (boneData->length);
		boneData->x                 = AGLTool_ToGLWidth (boneData->x);
		boneData->y                 = AGLTool_ToGLHeight(boneData->y);

		boneData->rotationZ         = AJsonObject->GetFloat(jsonBone, "rotation", 0.0f);
		boneData->scaleX            = AJsonObject->GetFloat(jsonBone, "scaleX",   1.0f);
		boneData->scaleY            = AJsonObject->GetFloat(jsonBone, "scaleY",   1.0f);

		boneData->isInheritScale    = AJsonObject->GetBool(jsonBone, "inheritScale",    true);
		boneData->isInheritRotation = AJsonObject->GetBool(jsonBone, "inheritRotation", true);

        AArrayStrMap_TryPut(boneDataMap,      boneData->name, boneData);
		AArray_Set         (boneDataOrderArr, i, boneData,    SkeletonBoneData*);

		ALog_D("BoneData name = %s", boneData->name);
		ALog_D
		(
			"SkeletonBoneData[%d] name = %s, parentName = %s, length = %f",
			i, boneData->name, parentName ? parentName : "NULL", boneData->length
		);
	}
}


static inline void ReadSlotData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonArray*   slotArr           = AJsonObject->GetArray(root, "slots");
	ArrayStrMap* slotDataMap       = skeletonData->slotDataMap;
	AArrayStrMap->InitWithCapacity(sizeof(SkeletonSlotData*), slotArr->valueList->size, slotDataMap);

	ArrayStrMap* boneDataMap       = skeletonData->boneDataMap;


	// hold SkeletonSlotData in JOSN order
	skeletonData->slotDataOrderArr = AArray->Create(sizeof(SkeletonSlotData*), slotArr->valueList->size);
	Array* slotDataOrderArr        = skeletonData->slotDataOrderArr;

	for (int i = 0; i < slotArr->valueList->size; i++)
	{
		JsonObject* jsonSlot       = AJsonArray->GetObject(slotArr, i);

		char* name                 = AJsonObject->GetString(jsonSlot, "name",       NULL);
		char* attachmentName       = AJsonObject->GetString(jsonSlot, "attachment", NULL);
		int   nameLength           = (int) strlen(name) + 1;
		int   attachmentNameLength = attachmentName != NULL ? ((int) strlen(attachmentName) + 1) : 0;

		SkeletonSlotData* slotData = (SkeletonSlotData*) malloc(sizeof(SkeletonSlotData) + nameLength + attachmentNameLength);
		slotData->name             = (char*) slotData + sizeof(SkeletonSlotData);
		memcpy(slotData->name, name, nameLength);

		slotData->attachmentDataList = NULL;

		if (attachmentName != NULL)
		{
			slotData->attachmentName = (char*) slotData + sizeof(SkeletonSlotData) + nameLength;
			memcpy(slotData->attachmentName, attachmentName, attachmentNameLength);
		}
		else
		{
			slotData->attachmentName = NULL;
		}

		char* bone                 = AJsonObject->GetString(jsonSlot, "bone", NULL);
		slotData->boneData         = AArrayStrMap_Get(boneDataMap, bone, SkeletonBoneData*);

		ALog_A(slotData->boneData  != NULL, "Slot bone not found: %s", bone);

		char* color                = AJsonObject->GetString(jsonSlot, "color", NULL);
		if (color == NULL)
		{
			slotData->color->a = 1.0f;
			slotData->color->r = 1.0f;
			slotData->color->g = 1.0f;
			slotData->color->b = 1.0f;
		}
		else
		{
			AColor->SetWithString(color, slotData->color);
			ALog_D
			(
				"SkeletonSlotData color = %s, r = %x, g = %x, b = %x, a = %x",
				color,
				(int) slotData->color->r,
				(int) slotData->color->g,
				(int) slotData->color->b,
				(int) slotData->color->a
			);
		}

		slotData->isAdditiveBlending = AJsonObject->GetBool(jsonSlot, "additive", false);
        AArrayStrMap_TryPut(slotDataMap, slotData->name, slotData);
		AArray_Set(slotDataOrderArr, i, slotData, SkeletonSlotData*);

		ALog_D
		(
		   "SkeletonSlotData[%d] name = %s, bone = %s, attachment = %s, color = %s",
		    i, slotData->name, bone, slotData->attachmentName ?  slotData->attachmentName : "NULL",
		    color ? color : "NULL"
		);
	}
}


static inline SkeletonAttachmentData* CreateAttachmentData(int length, SkeletonAttachmentDataType attachmentDataType)
{
	SkeletonAttachmentData* attachmentData;

	switch (attachmentDataType)
	{
		case skeleton_attachment_region:
		{
			SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*)
																  malloc(sizeof(SkeletonRegionAttachmentData) + length);

			attachmentData                                     = regionAttachmentData->attachmentData;
			attachmentData->childPtr                           = regionAttachmentData;

            break;
        }

		case skeleton_attachment_boundingbox:
		{
			SkeletonBoundingboxAttachmentData* boundingboxAttachmentData = (SkeletonBoundingboxAttachmentData*)
																			malloc(sizeof(SkeletonBoundingboxAttachmentData) + length);

			attachmentData                                               = boundingboxAttachmentData->attachmentData;
			attachmentData->childPtr                                     = boundingboxAttachmentData;

            break;
        }

		case skeleton_attachment_mesh:
		{
			SkeletonMeshAttachmentData* meshAttachmentData = (SkeletonMeshAttachmentData*) malloc(sizeof(SkeletonMeshAttachmentData) + length);
			attachmentData                                 = meshAttachmentData->attachmentData;
			attachmentData->childPtr                       = meshAttachmentData;

            break;
        }

		case skeleton_attachment_skinned_mesh:
		{
			SkeletonSkinnedMeshAttachmentData* skinnedMeshAttachmentData = (SkeletonSkinnedMeshAttachmentData*) malloc(sizeof(SkeletonSkinnedMeshAttachmentData) + length);
			attachmentData                                               = skinnedMeshAttachmentData->meshAttachmentData->attachmentData;
			attachmentData->childPtr                                     = skinnedMeshAttachmentData;

            break;
        }
	}

	attachmentData->type = attachmentDataType;

	return attachmentData;
}


static inline ArrayStrMap* ReadSkinDataSlotAttachment(JsonObject* attachmentDataMapObject)
{
	ArrayStrMap* attachmentDataMap = AArrayStrMap->CreateWithCapacity(sizeof(SkeletonAttachmentData*), attachmentDataMapObject->valueMap->elementList->size);

	for (int i = 0; i < attachmentDataMapObject->valueMap->elementList->size; i++)
	{
		char* name                       = AJsonObject->GetKey          (attachmentDataMapObject, i);
		JsonObject* attachmentDataObject = AJsonObject->GetObjectByIndex(attachmentDataMapObject, i);
		char* attachmentType             = AJsonObject->GetString       (attachmentDataObject,    "type", NULL);
		char* attachmentName             = AJsonObject->GetString       (attachmentDataObject,    "name", NULL);
		int         length               = 0;
		int         nameLength           = 0;

		if (attachmentName == NULL)
		{
			attachmentName = name;
		}

		nameLength = (int) strlen(attachmentName) + 1;

		SkeletonAttachmentData* attachmentData;

		if (attachmentType == NULL)
		{
			// region
			attachmentData                                     = CreateAttachmentData(length + nameLength, skeleton_attachment_region);
			length                                            += sizeof(SkeletonRegionAttachmentData);

			SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*) attachmentData->childPtr;

			regionAttachmentData->x                            = AJsonObject->GetFloat(attachmentDataObject, "x", 0.0f)        * ASkeletonData->scale;
			regionAttachmentData->y                            = AJsonObject->GetFloat(attachmentDataObject, "y", 0.0f)        * ASkeletonData->scale;
			regionAttachmentData->scaleX                       = AJsonObject->GetFloat(attachmentDataObject, "scaleX",   1.0f);
			regionAttachmentData->scaleY                       = AJsonObject->GetFloat(attachmentDataObject, "scaleY",   1.0f);
			regionAttachmentData->rotationZ                    = AJsonObject->GetFloat(attachmentDataObject, "rotation", 0.0f);
			regionAttachmentData->width                        = AJsonObject->GetFloat(attachmentDataObject, "width",    0.0f)  * ASkeletonData->scale;
			regionAttachmentData->height                       = AJsonObject->GetFloat(attachmentDataObject, "height",   0.0f)  * ASkeletonData->scale;

			ALog_D
			(
				 "SkeletonRegionAttackmentData[%d], name = %s,  x = %f, y = %f, width = %f, height = %f rotation = %f",
				 i,
				 attachmentName,
				 regionAttachmentData->x,
				 regionAttachmentData->y,
                 regionAttachmentData->width,
                 regionAttachmentData->height,
                 regionAttachmentData->rotationZ
			);

			regionAttachmentData->x                            = AGLTool_ToGLWidth (regionAttachmentData->x);
			regionAttachmentData->y                            = AGLTool_ToGLHeight(regionAttachmentData->y);

			regionAttachmentData->width                        = AGLTool_ToGLWidth (regionAttachmentData->width);
			regionAttachmentData->height                       = AGLTool_ToGLHeight(regionAttachmentData->height);

		}
		else if (strcmp(attachmentType, "boundingbox") == 0)
		{
			JsonArray* jsonVertexArr                                     = AJsonObject->GetArray(attachmentDataObject, "vertices");
			length                                                      += jsonVertexArr->valueList->size * sizeof(float);

			attachmentData                                               = CreateAttachmentData(length + nameLength, skeleton_attachment_boundingbox);
			length                                                      += sizeof(SkeletonBoundingboxAttachmentData);

			SkeletonBoundingboxAttachmentData* boundingboxAttachmentData = (SkeletonBoundingboxAttachmentData*) attachmentData->childPtr;
			boundingboxAttachmentData->vertexArr->length                 = jsonVertexArr->valueList->size;
			boundingboxAttachmentData->vertexArr->data                   = (char*) boundingboxAttachmentData + sizeof(SkeletonBoundingboxAttachmentData);
			float* vertices                                              = AArray_GetData(boundingboxAttachmentData->vertexArr, float);

			for (int j = 0; j < jsonVertexArr->valueList->size; j += 2)
			{
				vertices[j]     = AGLTool_ToGLWidth (AJsonArray->GetFloat(jsonVertexArr, j)     * ASkeletonData->scale);
				vertices[j + 1] = AGLTool_ToGLHeight(AJsonArray->GetFloat(jsonVertexArr, j + 1) * ASkeletonData->scale);

				ALog_D("SkeletonBoundingboxAttachmentData vertices[%d] x = %f, y = %f", j, vertices[j], vertices[j + 1]);
			}

		}
		else if (strcmp(attachmentType, "mesh") == 0)
		{
			JsonArray* jsonVertexArr                       = AJsonObject->GetArray(attachmentDataObject, "vertices");
			JsonArray* jsonTriangleArr                     = AJsonObject->GetArray(attachmentDataObject, "triangles");
			JsonArray* jsonUVArr                           = AJsonObject->GetArray(attachmentDataObject, "uvs");

			int        verticesCount                       = jsonVertexArr->valueList->size   + (jsonVertexArr->valueList->size >> 1);
			int        verticesByte                        = verticesCount                    * sizeof(float);
			int        uvsByte                             = jsonUVArr->valueList->size       * sizeof(float);
			int        trianglesByte                       = jsonTriangleArr->valueList->size * sizeof(short);

			length                                        += verticesByte +  uvsByte +  trianglesByte;
			attachmentData                                 = CreateAttachmentData(length + nameLength, skeleton_attachment_mesh);
			length                                        += sizeof(SkeletonMeshAttachmentData);

			SkeletonMeshAttachmentData* meshAttachmentData = (SkeletonMeshAttachmentData*) attachmentData->childPtr;

			meshAttachmentData->isUVMappedInTexture        = false;
			meshAttachmentData->vertexArr->length          = verticesCount;
			meshAttachmentData->vertexArr->data            = (char*) meshAttachmentData + sizeof(SkeletonMeshAttachmentData);
			float* vertices                                = AArray_GetData(meshAttachmentData->vertexArr, float);

			for (int j = 0, k = 0; j < verticesCount; j += 3, k += 2)
			{
				vertices[j]     = AGLTool_ToGLWidth (AJsonArray->GetFloat(jsonVertexArr, k)     * ASkeletonData->scale);
				vertices[j + 1] = AGLTool_ToGLHeight(AJsonArray->GetFloat(jsonVertexArr, k + 1) * ASkeletonData->scale);
				vertices[j + 2] = 0.0f;

				ALog_D("SkeletonMeshAttachmentData vertices[%d] x = %f, y = %f", j, vertices[j], vertices[j + 1]);
			}

			meshAttachmentData->uvArr->length              = jsonUVArr->valueList->size;
			meshAttachmentData->uvArr->data                = (char*) vertices + verticesByte;
			float* uvs                                     = AArray_GetData(meshAttachmentData->uvArr, float);
			for (int j = 0; j < jsonUVArr->valueList->size; j += 2)
			{
				uvs[j]     = AJsonArray->GetFloat(jsonUVArr, j);
				uvs[j + 1] = AJsonArray->GetFloat(jsonUVArr, j + 1);

				ALog_D("SkeletonMeshAttachmentData uvs[%d] x = %f, y = %f", j, uvs[j], uvs[j + 1]);
			}

			meshAttachmentData->triangleArr->length        = jsonTriangleArr->valueList->size;
			meshAttachmentData->triangleArr->data          = (char*) uvs + uvsByte;
			short* triangles                               = AArray_GetData(meshAttachmentData->triangleArr, short);
			for (int j = 0; j < jsonTriangleArr->valueList->size; j++)
			{
				triangles[j] = (short) AJsonArray->GetInt(jsonTriangleArr, j);
				ALog_D("SkeletonMeshAttachmentData triangles[%d] x = %d", j, triangles[j]);
			}

			meshAttachmentData->width                      = AJsonObject->GetFloat(attachmentDataObject, "width",  0.0f) * ASkeletonData->scale;
			meshAttachmentData->height                     = AJsonObject->GetFloat(attachmentDataObject, "height", 0.0f) * ASkeletonData->scale;
			meshAttachmentData->width                      = AGLTool_ToGLWidth (meshAttachmentData->width);
			meshAttachmentData->height                     = AGLTool_ToGLHeight(meshAttachmentData->height);
		}
		else if (strcmp(attachmentType, "skinnedmesh") == 0)
		{
			// one block data is [boneCount, boneIndex, x, y, weight ...]
			JsonArray* jsonVertexArr                       = AJsonObject->GetArray(attachmentDataObject, "vertices");
			JsonArray* jsonTriangleArr                     = AJsonObject->GetArray(attachmentDataObject, "triangles");
			JsonArray* jsonUVArr                           = AJsonObject->GetArray(attachmentDataObject, "uvs");

			int        bonesCount                          = 0;
			int        weightsCount                        = 0;
			int        weightVerticesCount                 = 0;
			int        verticesCount                       = 0;

			for (int j = 0; j < jsonVertexArr->valueList->size; verticesCount++)
			{
				int boneCount        = AJsonArray->GetInt(jsonVertexArr, j);

				// contains boneCount and each bones index
				bonesCount          += boneCount + 1;
				weightsCount        += boneCount;
				weightVerticesCount += boneCount * 2;
				j                   += boneCount * 4 + 1;
			}

		    // how many vertices in mesh
		    verticesCount                                   = verticesCount * 3;
		    // how many vertices mix by each bone weight
		    weightVerticesCount                             = weightVerticesCount + (weightVerticesCount >> 1);

		    int        verticesByte                        = verticesCount                    * sizeof(float);
			int        weightVerticesByte                  = weightVerticesCount              * sizeof(float);
			int        trianglesByte                       = jsonTriangleArr->valueList->size * sizeof(short);
			int        uvsByte                             = jsonUVArr->valueList->size       * sizeof(float);
			int        bonesByte                           = bonesCount                       * sizeof(int);
			int        weightsByte                         = weightsCount                     * sizeof(float);

			length                                        += verticesByte
														  +  weightVerticesByte
														  +  bonesByte
														  +  weightsByte
									                      +  uvsByte
			                                              +  trianglesByte;

			attachmentData                                 = CreateAttachmentData(length + nameLength, skeleton_attachment_skinned_mesh);
			length                                        += sizeof(SkeletonSkinnedMeshAttachmentData);

			SkeletonSkinnedMeshAttachmentData* skinnedMeshAttachmentData
			                                               = (SkeletonSkinnedMeshAttachmentData*) attachmentData->childPtr;

			SkeletonMeshAttachmentData* meshAttachmentData = skinnedMeshAttachmentData->meshAttachmentData;

			meshAttachmentData->vertexArr->length          = verticesCount;
			meshAttachmentData->vertexArr->data            = (char*) skinnedMeshAttachmentData + sizeof(SkeletonSkinnedMeshAttachmentData);
		    memset(meshAttachmentData->vertexArr->data, 0, verticesByte);

			skinnedMeshAttachmentData->weightVertexArr->length = weightVerticesCount;
			skinnedMeshAttachmentData->weightVertexArr->data   = (char*) meshAttachmentData->vertexArr->data + verticesByte;
			float* weightVertices                              = AArray_GetData(skinnedMeshAttachmentData->weightVertexArr, float);

			skinnedMeshAttachmentData->boneArr->length         = bonesCount;
			skinnedMeshAttachmentData->boneArr->data           = (char*) weightVertices + weightVerticesByte;
			int*  bones                                        = AArray_GetData(skinnedMeshAttachmentData->boneArr, int);

			skinnedMeshAttachmentData->weightArr->length       = weightsCount;
			skinnedMeshAttachmentData->weightArr->data         = (char*) bones   + bonesByte;
			float* weights                                     = AArray_GetData(skinnedMeshAttachmentData->weightArr, float);

			for (int j = 0, b = 0, w = 0, v = 0; j < jsonVertexArr->valueList->size;)
			{
				ALog_D("SkeletonSkinnedMeshAttachmentData vertices [%d]", j);

				int boneCount   = AJsonArray->GetInt(jsonVertexArr, j++);
				bones[b++]      = boneCount;


				ALog_D("SkeletonSkinnedMeshAttachmentData bone count = %d", bones[b - 1]);

				for (int k = j + boneCount * 4; j < k; j += 4, v += 3)
				{
					// bone index
					bones  [b++] = AJsonArray->GetInt  (jsonVertexArr, j);
					weights[w++] = AJsonArray->GetFloat(jsonVertexArr, j + 3);

					ALog_D("SkeletonSkinnedMeshAttachmentData bone index = %d, weight = %f", bones[b - 1], weights[w - 1]);

					weightVertices[v]     = AGLTool_ToGLWidth (AJsonArray->GetFloat(jsonVertexArr, j + 1) * ASkeletonData->scale);
					weightVertices[v + 1] = AGLTool_ToGLHeight(AJsonArray->GetFloat(jsonVertexArr, j + 2) * ASkeletonData->scale);
					weightVertices[v + 2] = 0.0f;

					ALog_D("SkeletonSkinnedMeshAttachmentData vertices x = %f, y = %f", weightVertices[v], weightVertices[v + 1]);
				}
			}

			meshAttachmentData->uvArr->length              = jsonUVArr->valueList->size;
			meshAttachmentData->uvArr->data                = (char*) weights + weightsByte;
			float*    uvs                                  = AArray_GetData(meshAttachmentData->uvArr, float);
			for (int j = 0; j < jsonUVArr->valueList->size; j += 2)
			{
				uvs[j]     = AJsonArray->GetFloat(jsonUVArr, j);
				uvs[j + 1] = AJsonArray->GetFloat(jsonUVArr, j + 1);

				ALog_D("SkeletonSkinnedMeshAttachmentData uvs[%d] x = %f, y = %f", j, uvs[j], uvs[j + 1]);
			}

			meshAttachmentData->triangleArr->length        = jsonTriangleArr->valueList->size;
			meshAttachmentData->triangleArr->data          = (char*) uvs + uvsByte;
			short*     triangles                           = AArray_GetData(meshAttachmentData->triangleArr, short);
			for (int j = 0; j < jsonTriangleArr->valueList->size; j++)
			{
				triangles[j] = (short) AJsonArray->GetInt(jsonTriangleArr, j);
				ALog_D("SkeletonSkinnedMeshAttachmentData triangles[%d] x = %d", j, triangles[j]);
			}

			meshAttachmentData->width                      = AJsonObject->GetFloat(attachmentDataObject, "width",  0.0f)  * ASkeletonData->scale;
			meshAttachmentData->height                     = AJsonObject->GetFloat(attachmentDataObject, "height", 0.0f)  * ASkeletonData->scale;
			meshAttachmentData->width                      = AGLTool_ToGLWidth (meshAttachmentData->width);
			meshAttachmentData->height                     = AGLTool_ToGLHeight(meshAttachmentData->height);
		}

		attachmentData->name = ((char*) attachmentData->childPtr) + length;
		memcpy(attachmentData->name, attachmentName, nameLength);

        AArrayStrMap_TryPut(attachmentDataMap, name, attachmentData);
	}

	return attachmentDataMap;
}


static inline void ReadSkinDataSlot(JsonObject* skinSlot, SkeletonSkinData* skinData)
{
	ArrayStrMap* slotAttachmentMap = skinData->slotAttachmentMap;
	AArrayStrMap->InitWithCapacity(sizeof(ArrayStrMap*), skinSlot->valueMap->elementList->size, slotAttachmentMap);

	for (int i = 0; i < skinSlot->valueMap->elementList->size; i++)
	{
		char* slotName = AJsonObject->GetKey(skinSlot, i);

		ALog_D("SkeletonSkinData slot[%d], slotName = %s", i, slotName);

		JsonObject*  attachmentDataMapObject = AJsonObject->GetObjectByIndex(skinSlot, i);
		ArrayStrMap* attachmentDataMap       = ReadSkinDataSlotAttachment(attachmentDataMapObject);

        AArrayStrMap_TryPut(slotAttachmentMap, slotName, attachmentDataMap);
	}
}


static inline void ReadSkinData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonObject*  skinDataObject  = AJsonObject->GetObject(root, "skins");
	ArrayStrMap* skinDataMap     = skeletonData->skinDataMap;
	AArrayStrMap->InitWithCapacity(sizeof(SkeletonSkinData*), skinDataObject->valueMap->elementList->size, skinDataMap);

	for (int i = 0; i < skinDataObject->valueMap->elementList->size; i++)
	{
		char* skinName             = AJsonObject->GetKey(skinDataObject, i);
		int   skinNameLength       = (int) strlen(skinName) + 1;

		SkeletonSkinData* skinData = (SkeletonSkinData*) malloc(sizeof(SkeletonSkinData) + skinNameLength);
		skinData->name             = (char*) skinData + sizeof(SkeletonSkinData);
		memcpy(skinData->name, skinName, skinNameLength);

		ALog_D("SkeletonSkinData[%d], name = %s", i, skinData->name);

		// skin all slots
		JsonObject*  skinDataSlot          = AJsonObject->GetObjectByIndex(skinDataObject, i);
		ReadSkinDataSlot(skinDataSlot, skinData);

        AArrayStrMap_TryPut(skinDataMap, skinData->name, skinData);

		if (strcmp(skinName, "default") == 0)
		{
			skeletonData->skinDataDefault = skinData;
		}
	}

}


static inline void ReadCurve(SkeletonCurveTimeline* curveTimeline, int frameIndex, JsonObject* jsonTimeline)
{
	JsonType type = AJsonObject->GetType(jsonTimeline, "curve");
	switch (type)
	{
		case JsonType_Null:
			ASkeletonTimeline->SetLinear(curveTimeline, frameIndex);
			ALog_D("frameIndex[%d] curve is linear", frameIndex);
			break;

		case JsonType_String:
			if (strcmp(AJsonObject->GetString(jsonTimeline, "curve", ""), "stepped") == 0)
			{
				ASkeletonTimeline->SetStepped(curveTimeline, frameIndex);
				ALog_D("frameIndex[%d] curve is stepped", frameIndex);
			}
			break;


		case JsonType_Array:
		{
			JsonArray* curveArr = AJsonObject->GetArray(jsonTimeline, "curve");
			ALog_D("frameIndex[%d] curve is bezier", frameIndex);

			ASkeletonTimeline->SetCurve
			(
				curveTimeline,
				frameIndex,
				AJsonArray->GetFloat(curveArr, 0),
				AJsonArray->GetFloat(curveArr, 1),
				AJsonArray->GetFloat(curveArr, 2),
				AJsonArray->GetFloat(curveArr, 3)
			);

            break;
        }
	}
}


static inline void ReadAnimationBones
(
    SkeletonData*          skeletonData,
    JsonObject*            jsonBones,
    SkeletonAnimationData* animationData,
    ArrayList*             skeletonTimelineArr
)
{

	ArrayStrMap* boneDataMap = skeletonData->boneDataMap;

	for (int i = 0; i < jsonBones->valueMap->elementList->size; i++)
	{
		char*             boneName = AJsonObject->GetKey(jsonBones, i);
		SkeletonBoneData* boneData = AArrayStrMap_Get(boneDataMap, boneName, SkeletonBoneData*);
		ALog_A(boneData != NULL, "BoneData = %s is not found", boneName);

		int  boneIndex             = -1;
		for (int j = 0; j < skeletonData->boneDataOrderArr->length; j++)
		{
			if (AArray_Get(skeletonData->boneDataOrderArr, j, SkeletonBoneData*) == boneData)
			{
				boneIndex = j;
				break;
			}
		}

		ALog_A(boneIndex != -1, "readAnimationBones not found boneData = %s in boneDataOrderArr", boneName);

		JsonObject* jsonBoneTimeline = AJsonObject->GetObjectByIndex(jsonBones, i);
		for (int j = 0; j < jsonBoneTimeline->valueMap->elementList->size; j++)
		{
			char* timelineType     = AJsonObject->GetKey(jsonBoneTimeline, j);
			ALog_D("bone name = %s, timeline type = %s", boneName, timelineType);

			JsonArray* timelineArr = AJsonObject->GetArrayByIndex(jsonBoneTimeline, j);
			bool       isOne       = false;
			bool       isTwo       = false;

			if (strcmp(timelineType, "rotate") == 0)
			{
				SkeletonRotateTimeline* rotateTimeline = ASkeletonTimeline->CreateRotate(timelineArr->valueList->size, boneIndex);

				for (int k = 0, frameIndex = 0; k < timelineArr->valueList->size; k++, frameIndex++)
				{
					JsonObject* jsonTimeline = AJsonArray->GetObject(timelineArr,  k);
					float time               = AJsonObject->GetFloat(jsonTimeline, "time",  0.0f);
					float angle              = AJsonObject->GetFloat(jsonTimeline, "angle", 0.0f);

					ALog_D("frameIndex[%d][time: %f, angle: %f]", frameIndex, time, angle);

					ASkeletonTimeline->SetRotateFrame(rotateTimeline, frameIndex, time, angle);
					ReadCurve(rotateTimeline->curveTimeline, frameIndex, jsonTimeline);
				}

				// the array name is const no address, so &rotateTimeline->skeletonTimeline not working
				SkeletonTimeline* skeletonTimeline = rotateTimeline->skeletonTimeline;
				AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

				animationData->duration = AMath_Max
						                  (
											  animationData->duration,
											  AArray_Get(rotateTimeline->frameArr, rotateTimeline->frameArr->length - 2, float)
										  );

			}
			else if ((isOne = strcmp(timelineType, "translate") == 0) || (isTwo = strcmp(timelineType, "scale") == 0))
			{
				SkeletonTranslateTimeline* translateTimeline;
				SkeletonTimeline*          skeletonTimeline;

				if (isOne)
				{
					translateTimeline = ASkeletonTimeline->CreateTranslate(timelineArr->valueList->size, boneIndex);

				}
				else if (isTwo)
				{
					SkeletonScaleTimeline* scaleTimeline  = ASkeletonTimeline->CreateScale(timelineArr->valueList->size, boneIndex);
					translateTimeline                     = scaleTimeline->translateTimeline;
				}

				skeletonTimeline = translateTimeline->skeletonTimeline;
				AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

				for (int k = 0, frameIndex = 0; k < timelineArr->valueList->size; k++, frameIndex++)
				{
					JsonObject* jsonTimeline = AJsonArray->GetObject(timelineArr,  k);
					float       time         = AJsonObject->GetFloat(jsonTimeline, "time", 0.0f);
					float       x            = AJsonObject->GetFloat(jsonTimeline, "x",    0.0f);
					float       y            = AJsonObject->GetFloat(jsonTimeline, "y",    0.0f);

					ALog_D("frameIndex[%d][time: %f, x: %f, y: %f]", frameIndex, time, x, y);

					if (isOne)
					{
						x = AGLTool_ToGLWidth (x) * ASkeletonData->scale;
						y = AGLTool_ToGLHeight(y) * ASkeletonData->scale;
					}

					ASkeletonTimeline->SetTranslateFrame(translateTimeline, frameIndex, time, x, y);
					ReadCurve(translateTimeline->curveTimeline, frameIndex, jsonTimeline);
				}


				animationData->duration = AMath_Max
									      (
											animationData->duration,
											AArray_Get(translateTimeline->frameArr, translateTimeline->frameArr->length - 3, float)
										  );

			}
			else
			{
				ALog_A(false, "Invalid timeline type for a bone: %s", timelineType);
			}

			ALog_D("duration = %f", animationData->duration);
		}
	}

}


static inline void ReadAnimationSlots
(
    SkeletonData*          skeletonData,
    JsonObject*            jsonSlots,
    SkeletonAnimationData* animationData,
    ArrayList*             skeletonTimelineArr
)
{
	for (int i = 0; i < jsonSlots->valueMap->elementList->size; i++)
	{
		char* slotName = AJsonObject->GetKey(jsonSlots, i);
		int  slotIndex = FindSlotIndex(skeletonData, slotName);

		ALog_A(slotIndex != -1, "readAnimationBones not found slotData = %s in slotDataOrderArr", slotName);

		JsonObject* jsonSlotTimeline = AJsonObject->GetObjectByIndex(jsonSlots, i);
		for (int j = 0; j < jsonSlotTimeline->valueMap->elementList->size; j++)
		{
			char* timelineType = AJsonObject->GetKey(jsonSlotTimeline, j);
			ALog_D("slot name = %s, timeline type = %s", slotName, timelineType);

			JsonArray* timelineArr = AJsonObject->GetArrayByIndex(jsonSlotTimeline, j);

			if (strcmp(timelineType, "color") == 0)
			{
				SkeletonColorTimeline* colorTimeline = ASkeletonTimeline->CreateColor(timelineArr->valueList->size);
				colorTimeline->slotIndex             = slotIndex;

				for (int k = 0, frameIndex = 0; k < timelineArr->valueList->size; k++, frameIndex++)
				{
					JsonObject* jsonTimeline = AJsonArray->GetObject(timelineArr,  k);
					float time               = AJsonObject->GetFloat (jsonTimeline, "time", 0.0);
					char* color              = AJsonObject->GetString(jsonTimeline, "color", NULL);

					Color c[1];
					AColor->SetWithString(color, c);
					ASkeletonTimeline->SetColorFrame(colorTimeline, frameIndex, time, c);
					ALog_D("frameIndex[%d][time: %f, color: %s]", frameIndex, time, color);

					ReadCurve(colorTimeline->curveTimeline, frameIndex, jsonTimeline);
				}

				SkeletonTimeline* skeletonTimeline = colorTimeline->skeletonTimeline;
				AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

				animationData->duration = AMath_Max
										  (
											  animationData->duration,
											  AArray_Get(colorTimeline->frameArr, colorTimeline->frameArr->length - 5, float)
										  );

			}
			else if (strcmp(timelineType, "attachment") == 0)
			{
				SkeletonAttachmentTimeline* attachmentTimeline = ASkeletonTimeline->CreateAttachment(timelineArr->valueList->size);
				attachmentTimeline->slotIndex                  = slotIndex;

				for (int k = 0, frameIndex = 0; k < timelineArr->valueList->size; k++, frameIndex++)
				{
					JsonObject* jsonTimeline = AJsonArray->GetObject(timelineArr,  k);
					float time               = AJsonObject->GetFloat (jsonTimeline, "time", 0.0f);
					char* name               = AJsonObject->GetString(jsonTimeline, "name", NULL);

					ASkeletonTimeline->SetAttachmentFrame(attachmentTimeline, frameIndex, time, name);
					ALog_D("frameIndex[%d][time: %f, name: %s]", frameIndex, time, name);
				}

				SkeletonTimeline* skeletonTimeline = attachmentTimeline->skeletonTimeline;
				AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

				animationData->duration = AMath_Max
										  (
											  animationData->duration,
											  AArray_Get(attachmentTimeline->frameArr, attachmentTimeline->frameArr->length - 1, float)
										  );

			}
			else
			{
				ALog_A(false, "Invalid timeline type for a slot: %s", timelineType);
			}

			ALog_D("duration = %f", animationData->duration);
		}
	}
}


static inline void ReadAnimationEvents
(
    SkeletonData*          skeletonData,
    JsonArray*             jsonEvents,
    SkeletonAnimationData* animationData,
    ArrayList*             skeletonTimelineArr
)
{

	SkeletonEventTimeline* eventTimeline = ASkeletonTimeline->CreateEvent(jsonEvents->valueList->size);

	for (int i = 0, frameIndex = 0; i < jsonEvents->valueList->size; i++, frameIndex++)
	{
		JsonObject* eventObject      = AJsonArray->GetObject(jsonEvents,  i);
		char*       eventName        = AJsonObject->GetString(eventObject, "name", NULL);
		float       time             = AJsonObject->GetFloat (eventObject, "time", 0.0f);
		ALog_D("SkeletonEventTimeline frameIndex[%d], [time = %f, eventName = %s]", frameIndex, time, eventName);

		SkeletonEventData* eventData = AArrayStrMap_Get(skeletonData->eventDataMap, eventName, SkeletonEventData*);

		if (eventData == NULL)
		{
			eventData = (SkeletonEventData[]) { "", "", 0, 0.0f };
		}

		char* stringValue = AJsonObject->GetString(eventObject, "string", eventData->stringValue);
		int   intValue    = AJsonObject->GetInt   (eventObject, "int",    eventData->intValue);
		float floatValue  = AJsonObject->GetFloat (eventObject, "float",  eventData->floatValue);
		ALog_D("[stringValue = %s, intValue = %d, floatValue = %f]", stringValue, intValue, floatValue);

		int                eventLength  = (int) strlen(eventName)   + 1;
		int                valueLength  = (int) strlen(stringValue) + 1;
		SkeletonEventData* newEventData = (SkeletonEventData*)
										   malloc(sizeof(SkeletonEventData) + eventLength + valueLength);

		newEventData->name              = (char*) newEventData + sizeof(SkeletonEventData);
		memcpy(newEventData->name, eventName, eventLength);

		newEventData->stringValue       = (char*) newEventData + sizeof(SkeletonEventData) + eventLength;
		memcpy(newEventData->stringValue, stringValue, valueLength);

		newEventData->intValue          = intValue;
		newEventData->floatValue        = floatValue;

		ASkeletonTimeline->SetEventFrame(eventTimeline, frameIndex, time, newEventData);
	}

	SkeletonTimeline* skeletonTimeline = eventTimeline->skeletonTimeline;
	AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

	animationData->duration = AMath_Max
			                  (
								animationData->duration,
								AArray_Get(eventTimeline->frameArr, eventTimeline->frameArr->length - 1, float)
							  );

	ALog_D("duration = %f", animationData->duration);
}


static inline void ReadAnimationDrawOrders
(
    SkeletonData*          skeletonData,
    JsonArray*             jsonDrawOrder,
    SkeletonAnimationData* animationData,
    ArrayList*             skeletonTimelineArr
)
{

	SkeletonDrawOrderTimeline* drawOrderTimeline = ASkeletonTimeline->CreateDrawOrder(jsonDrawOrder->valueList->size);
	int                        slotCount         = skeletonData->slotDataOrderArr->length;

	// collect slot index unchanged, but moved slot will push other move as whole
	int                        unchanged[slotCount];

	for (int i = 0, frameIndex = 0; i < jsonDrawOrder->valueList->size; i++, frameIndex++)
	{
		JsonObject* drawOrderObject      = AJsonArray->GetObject(jsonDrawOrder,    i);
		float       time                 = AJsonObject->GetFloat (drawOrderObject,  "time", 0.0f);

		ALog_D("SkeletonDrawOrderTimeline frameIndex[%d], [time = %f]", frameIndex, time);

		JsonArray*  offsets              = AJsonObject->GetArray(drawOrderObject, "offsets");
		Array(int)* drawOrderArr         = NULL;

		if (offsets != NULL)
		{
			drawOrderArr                 = AArray->Create(sizeof(int), slotCount);
			int*    drawOrder            = AArray_GetData(drawOrderArr, int);

			// set default value
			memset(drawOrder, -1, slotCount * sizeof(int));
			memset(unchanged, -1, slotCount * sizeof(int));

			int originalIndex   = 0;
			int unchangedIndex  = 0;
			int slotIndex;

			for (int j = 0; j < offsets->valueList->size; j++)
			{
				JsonObject* offset       = AJsonArray->GetObject(offsets, j);
				char*       slotName     = AJsonObject->GetString(offset,  "slot",   NULL);
				int         offsetIndex  = AJsonObject->GetInt   (offset,  "offset", 0);

				ALog_D("[name = %s, offset = %d]", slotName, offsetIndex);

				slotIndex                = FindSlotIndex(skeletonData, slotName);

				ALog_A(slotIndex != -1, "Can't find slot by offset name = %s", slotName);

				// collect unchanged index until the offset index
				while (originalIndex != slotIndex)
				{
					unchanged[unchangedIndex++] = originalIndex++;
				}

				// set changed index to new index in drawOrder
				// drawOrder first collect offset index to the right place
				drawOrder[originalIndex + offsetIndex] = originalIndex;
				originalIndex++;
			}

			// collect remaining unchanged index
			// then unchanged collect all index not offset
			while (originalIndex < slotCount)
			{
				unchanged[unchangedIndex++] = originalIndex++;
			}

			// drawOrder have all offset index
			// fill in unchanged index to drawOrder by reverse order
			for (int j = slotCount - 1; j > -1; j--)
			{
				if (drawOrder[j] == -1)
				{
					drawOrder[j] = unchanged[--unchangedIndex];
				}
			}

		}
		else
		{
			ALog_D("drawOrder offsets not found");
		}

		ASkeletonTimeline->SetDrawOrderFrame(drawOrderTimeline, frameIndex, time, drawOrderArr);
	}

	SkeletonTimeline* skeletonTimeline = drawOrderTimeline->skeletonTimeline;
	AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

	animationData->duration = AMath_Max
			                  (
								animationData->duration,
								AArray_Get(drawOrderTimeline->frameArr, drawOrderTimeline->frameArr->length - 1, float)
							  );

	ALog_D("duration = %f", animationData->duration);
	// ALog_A(0, "stop");
}


static inline void ReadAnimationDeform
(
	SkeletonData*          skeletonData,
	JsonObject*            jsonDeform,
	SkeletonAnimationData* animationData,
	ArrayList*             skeletonTimelineArr
)
{
	for (int i = 0; i < jsonDeform->valueMap->elementList->size; i++)
	{
		char*             skinName  = AJsonObject->GetKey(jsonDeform, i);
		SkeletonSkinData* skinData  = AArrayStrMap_Get(skeletonData->skinDataMap, skinName, SkeletonSkinData*);

		ALog_A(skinData != NULL, "Deform skin not found %s", skinName);

		JsonObject* jsonSlots       = AJsonObject->GetObjectByIndex(jsonDeform, i);
		for (int j = 0; j < jsonSlots->valueMap->elementList->size; j++)
		{
			char* slotName  = AJsonObject->GetKey(jsonSlots, j);
			int   slotIndex = FindSlotIndex(skeletonData, slotName);

			ALog_A(slotIndex != -1, "Deform not found slot = %s in slotDataOrderArr", slotName);

			JsonObject* jsonMeshs   = AJsonObject->GetObjectByIndex(jsonSlots, j);
			for (int k = 0; k < jsonMeshs->valueMap->elementList->size; k++)
			{
				JsonArray*              jsonMeshArr    = AJsonObject->GetArrayByIndex   (jsonMeshs, k);
				char*                   meshName       = AJsonObject->GetKey            (jsonMeshs, k);
				SkeletonDeformTimeline* deformTimeline = ASkeletonTimeline->CreateDeform(jsonMeshArr->valueList->size);

				SkeletonAttachmentData* attchmentData  = GetAttachmentDataBySkinData(skinData, slotName, meshName);
				ALog_A(attchmentData != NULL, "Mesh attachment %s not found, slotName = %s, skinName = %s", meshName, slotName, skinName);

				deformTimeline->slotIndex              = slotIndex;
				deformTimeline->attachmentData         = attchmentData;

				Array* meshVertices;

				switch (attchmentData->type)
				{
					case skeleton_attachment_region:
					case skeleton_attachment_boundingbox:
						ALog_A(false, "readAnimationDeform wrong attchmentData->attachmentDataType");
					break;

					case skeleton_attachment_mesh:
						meshVertices = ((SkeletonMeshAttachmentData*) attchmentData->childPtr)->vertexArr;
                        break;


					case skeleton_attachment_skinned_mesh:
						meshVertices = ((SkeletonSkinnedMeshAttachmentData*) attchmentData->childPtr)->weightVertexArr;
                        break;
				}

				for (int l = 0, frameIndex = 0; l < jsonMeshArr->valueList->size; l++, frameIndex++)
				{
					JsonObject* jsonMesh      = AJsonArray->GetObject(jsonMeshArr, l);
					JsonArray*  jsonVertexArr = AJsonObject->GetArray(jsonMesh,    "vertices");
					Array*      vertexArr     = AArray->Create(sizeof(float), meshVertices->length);

					memcpy(vertexArr->data, meshVertices->data, sizeof(float) * meshVertices->length);

					if (jsonVertexArr != NULL)
					{
						float* vertexData     = AArray_GetData(vertexArr, float);

						// "offset" is how many entries are zeros at the start
						// also, zeros at the end are not output
						int    start          = AJsonObject->GetInt(jsonMesh, "offset", 0);

						for (int m = 0; m < jsonVertexArr->valueList->size; m++, start++)
						{
							// jsonVertices is x,y
							// vertexData   is x,y,z
							// so convert   index in jsonVertices to vertexData
							int   index        = (start >> 1) + start;

							float vertex       = AJsonArray->GetFloat(jsonVertexArr, m) * ASkeletonData->scale;
							vertexData[index] += AGLTool_ToGLWidth(vertex);

							ALog_D("Mesh vertices[%d] = %f", l, vertex);
						}
					}

					float time = AJsonObject->GetFloat(jsonMesh, "time", 0.0f);
					ALog_D("SkeletonDeformTimeline frameIndex[%d], [time = %f], vertices count = %d", frameIndex, time, vertexArr->length);

					ASkeletonTimeline->SetDeformFrame(deformTimeline, frameIndex, time, vertexArr);
					ReadCurve(deformTimeline->curveTimeline, frameIndex, jsonMesh);
				}

				SkeletonTimeline* skeletonTimeline = deformTimeline->skeletonTimeline;
				AArrayList_Add(skeletonTimelineArr, skeletonTimeline);

				animationData->duration = AMath_Max
										  (
										  	  animationData->duration,
											  AArray_Get(deformTimeline->frameArr, deformTimeline->frameArr->length - 1, float)
										  );

				ALog_D("duration = %f", animationData->duration);
			}
		}
	}
}


static inline void ReadAnimationData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonObject*  animationDataObject = AJsonObject->GetObject(root, "animations");
	ArrayStrMap* animationDataMap    = skeletonData->animationDataMap;

	ALog_A(animationDataObject, "readAnimationData not found animations");
	ALog_D("AnimationData size = %d", animationDataObject->arrayStrMap->elementList->size);

	AArrayStrMap->InitWithCapacity
	(
        sizeof(SkeletonAnimationData*),
        animationDataObject->valueMap->elementList->size,
        animationDataMap
    );

	for (int i = 0; i < animationDataObject->valueMap->elementList->size; i++)
	{
		char*                  animationName       = AJsonObject->GetKey(animationDataObject, i);
		int                    animationNameLength = (int) strlen(animationName) + 1;
		SkeletonAnimationData* animationData       = (SkeletonAnimationData*) malloc(sizeof(SkeletonAnimationData) + animationNameLength);

		animationData->name                        = (char*) animationData + sizeof(SkeletonAnimationData);
		memcpy(animationData->name, animationName, animationNameLength);
		ALog_D("AnimationData[%d] name = %s", i, animationData->name);

		ArrayList*             skeletonTimelineArr = animationData->timelineArr;

		AArrayList->Init(sizeof(SkeletonTimeline*), skeletonTimelineArr);

		animationData->duration                    = 0.0f;

		JsonObject* animationValue = AJsonObject->GetObjectByIndex(animationDataObject, i);

		JsonObject* jsonBones      = AJsonObject->GetObject(animationValue, "bones");
		if (jsonBones != NULL)
		{
			ReadAnimationBones(skeletonData, jsonBones, animationData, skeletonTimelineArr);
		}


		JsonObject* jsonSlots      = AJsonObject->GetObject(animationValue, "slots");
		if (jsonSlots != NULL)
		{
			ReadAnimationSlots(skeletonData, jsonSlots, animationData, skeletonTimelineArr);
		}

		JsonArray* jsonEvents      = AJsonObject->GetArray(animationValue, "events");
		if (jsonEvents != NULL)
		{
			ReadAnimationEvents(skeletonData, jsonEvents, animationData, skeletonTimelineArr);
		}

		JsonArray* jsonDrawOrder   = AJsonObject->GetArray(animationValue, "drawOrder");
		if (jsonDrawOrder != NULL)
		{
			ReadAnimationDrawOrders(skeletonData, jsonDrawOrder, animationData, skeletonTimelineArr);
		}

		JsonObject* jsonDeform     = AJsonObject->GetObject(animationValue, "deform");
		if (jsonDeform != NULL)
		{
			ReadAnimationDeform(skeletonData, jsonDeform, animationData, skeletonTimelineArr);
		}

		AArrayList->Shrink(skeletonTimelineArr);
        AArrayStrMap_TryPut(animationDataMap, animationName, animationData);
	}
}


static inline void ReadEventData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonObject*  eventObject  = AJsonObject->GetObject(root, "events");
	ArrayStrMap* eventDataMap = skeletonData->eventDataMap;

	int size = 0;
	if (eventObject != NULL)
	{
		size = eventObject->valueMap->elementList->size;
	}

	AArrayStrMap->InitWithCapacity(sizeof(SkeletonEventData*), size, eventDataMap);

	for (int i = 0; i < size; i++)
	{
		JsonObject* eventValue = AJsonObject->GetObjectByIndex(eventObject, i);

		if (eventValue->valueMap->elementList->size == 0)
		{
			continue;
		}

		char*              stringValue = AJsonObject->GetString(eventValue,  "string", NULL);
		char*              name        = AJsonObject->GetKey   (eventObject, i);
		int                nameLength  = (int) strlen(name)        + 1;
		int                valueLength = (int) strlen(stringValue) + 1;

		SkeletonEventData* eventData   = (SkeletonEventData*) malloc(sizeof(SkeletonEventData) + nameLength + valueLength);

		eventData->name                = (char*) eventData + sizeof(SkeletonEventData);
		memcpy(eventData->name, name, nameLength);

		eventData->stringValue         = (char*) eventData + sizeof(SkeletonEventData) + nameLength;
		memcpy(eventData->stringValue, stringValue, valueLength);

		eventData->intValue            = AJsonObject->GetInt  (eventValue, "int",   0);
		eventData->floatValue          = AJsonObject->GetFloat(eventValue, "float", 0.0f);

        AArrayStrMap_TryPut(eventDataMap, name, eventData);

		ALog_D
		(
			"SkeletonEventData name = %s, int = %d float = %f, string = %s",
			name, eventData->intValue, eventData->floatValue, stringValue
		);
	}
}


static inline void ReadSkeletonData(JsonObject* root, SkeletonData* skeletonData)
{
	JsonObject* skeletonObject = AJsonObject->GetObject(root, "skeleton");

	if (skeletonObject != NULL)
	{
		skeletonData->width  = AJsonObject->GetFloat(skeletonObject, "width",  0.0f);
		skeletonData->height = AJsonObject->GetFloat(skeletonObject, "height", 0.0f);

		ALog_D("SkeletonData width = %f, height = %f", skeletonData->width , skeletonData->height);

		skeletonData->width  = AGLTool_ToGLWidth (skeletonData->width);
		skeletonData->height = AGLTool_ToGLHeight(skeletonData->height);
	}
}


static inline void Parse(SkeletonData* skeletonData, char* jsonPath)
{
	JsonValue*  value = AJson->ParseWithFile(jsonPath);
	JsonObject* root  = value->jsonObject;

	// skeleton
	ReadSkeletonData(root, skeletonData);

	// bones
	ReadBoneData(root, skeletonData);

	// slots
	ReadSlotData(root, skeletonData);

	// events
	ReadEventData(root, skeletonData);

	// skins
	ReadSkinData(root, skeletonData);

	// animations
	ReadAnimationData(root, skeletonData);

	// json
	AJson->Release(value);
}


static inline void InitAtlas(SkeletonData* skeletonData, char* atlasPath)
{
	skeletonData->textureAtlas    = ATextureAtlas->Get(atlasPath);

	ArrayStrMap* skinDataMap      = skeletonData->skinDataMap;
	Array*       slotDataOrderArr = skeletonData->slotDataOrderArr;

	AArrayList->InitWithCapacity(sizeof(SkeletonAttachmentData*), 20, skeletonData->attachmentDataList);

	int quadCounts[skeletonData->textureAtlas->textureList->size];
	memset(quadCounts, 0, sizeof(int) * skeletonData->textureAtlas->textureList->size);

	for (int i = 0; i < slotDataOrderArr->length; i++)
	{
		SkeletonSlotData* slotData   = AArray_Get(slotDataOrderArr, i, SkeletonSlotData*);
		slotData->attachmentDataList = AArrayList->Create(sizeof(SkeletonAttachmentData*));

		// search all skin attachment
		for (int j = 0; j < skinDataMap->elementList->size; j++)
		{
            char*             skinName          = AArrayStrMap->GetKey(skinDataMap, j);
			SkeletonSkinData* skinData          = AArrayStrMap_GetAt(skinDataMap, j, SkeletonSkinData*);
			ArrayStrMap*      slotAttachmentMap = skinData->slotAttachmentMap;
			ArrayStrMap*      attachmentMap     = AArrayStrMap_Get(slotAttachmentMap, slotData->name, ArrayStrMap*);

			ALog_D("skin name = %s", skinName);

			if (attachmentMap == NULL)
			{
				continue;
			}

			ALog_D("Skin %s Can find AttachmentMap by slot name = %s", skinName, slotData->name);

			for (int k = 0; k < attachmentMap->elementList->size; k++)
			{
				// char*                   attachmentName = AArrayStrMap->GetKey(attachmentMap, k);
				SkeletonAttachmentData* attachmentData = AArrayStrMap_GetAt(attachmentMap, k, SkeletonAttachmentData*);

				if (attachmentData->type == skeleton_attachment_boundingbox)
				{
					continue;
				}

				// collect each skin attachmentData
				AArrayList_Add(slotData->attachmentDataList,     attachmentData);
				// collect all skin attachmentData
				AArrayList_Add(skeletonData->attachmentDataList, attachmentData);

				ALog_D("Set skin name = %s, attachment name = %s", skinName, attachmentData->name);

				TextureAtlasQuad* atlasQuad = ATextureAtlasGetQuad(skeletonData->textureAtlas, attachmentData->name);
				ALog_A(atlasQuad != NULL, "Can not find attachment in TextureAtlas by name = %s", attachmentData->name);

				switch (attachmentData->type)
				{
					case skeleton_attachment_region:
					{
						SkeletonRegionAttachmentData* regionAttachmentData = (SkeletonRegionAttachmentData*) attachmentData->childPtr;
						regionAttachmentData->meshIndex                    = atlasQuad->textureIndex;
						regionAttachmentData->quad                         = atlasQuad->quad;
						regionAttachmentData->subMeshIndex                 = quadCounts[atlasQuad->textureIndex]++;
					}
					break;

					SkeletonMeshAttachmentData* meshData;

					case skeleton_attachment_skinned_mesh:
						meshData = ((SkeletonSkinnedMeshAttachmentData*) attachmentData->childPtr)->meshAttachmentData;

					case skeleton_attachment_mesh:
						meshData                        = (SkeletonMeshAttachmentData*) attachmentData->childPtr;
						meshData->meshIndex             = atlasQuad->textureIndex;
						meshData->quad                  = atlasQuad->quad;
						meshData->subMeshIndex          = quadCounts[atlasQuad->textureIndex]++;
                        break;
				}
			}
		}

		AArrayList->Shrink(slotData->attachmentDataList);
		AArrayList->Shrink(skeletonData->attachmentDataList);
	}
}


static SkeletonData* Get(char* filePath)
{
	SkeletonData* skeletonData = AArrayStrMap_Get(skeletonDataMap, filePath, SkeletonData*);

	if (skeletonData == NULL)
	{
		skeletonData = (SkeletonData*) malloc(sizeof(SkeletonData));

		char path[strlen(filePath) + sizeof(".atlas.json")];

		sprintf(path, "%s.json",  filePath);
		Parse(skeletonData, path);

		sprintf(path, "%s.atlas", filePath);
		InitAtlas(skeletonData, path);

		skeletonData->filePath = AArrayStrMap_GetKey
								 (
									 AArrayStrMap_TryPut(skeletonDataMap, filePath, skeletonData),
									 skeletonDataMap->valueTypeSize
								 );
	}

//	Release(skeletonData);
//	ALog_A(0, "Stop");

	return skeletonData;
}


struct ASkeletonData ASkeletonData[1] =
{
	1.0f,
	Get,
	Release,
	GetAttachmentDataBySkinData,
};


SkeletonAttachmentMeshOffset skeletonAttachmentMeshOffset[3] =
{
	skeleton_attachment_region_mesh_index,
	skeleton_attachment_mesh_mesh_index,
	skeleton_attachment_skinned_mesh_mesh_index,
};


SkeletonAttachmentSubMeshOffset skeletonAttachmentSubMeshOffset[3] =
{
	skeleton_attachment_region_sub_mesh_index,
	skeleton_attachment_mesh_sub_mesh_index,
	skeleton_attachment_skinned_mesh_sub_mesh_index,
};
