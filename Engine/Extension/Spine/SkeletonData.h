/*
 *
 *
 *  Created on: 2013-6-27
 *      Author: scott.cgi
 */

#ifndef skeleton_data_h
#define skeleton_data_h

#include <stdbool.h>

#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Extension/TextureAtlas.h"


typedef struct SkeletonBoneData SkeletonBoneData;
struct  SkeletonBoneData
{
	SkeletonBoneData* get_only parent;
	const char*       get_only name;
	float             get_only length;
	float             get_only x;
	float             get_only y;
	float             get_only rotationZ;
	float             get_only scaleX;
	float             get_only scaleY;
	bool              get_only isInheritScale;
	bool              get_only isInheritRotation;
};


typedef struct
{
	Color                               get_only color[1];

	const char*                         get_only name;
	const char*                         get_only attachmentName;
	bool                                get_only isAdditiveBlending;
	SkeletonBoneData*                   get_only boneData;

	/**
	 * Each skin corresponding SkeletonAttachmentData may not exits
	 */
	ArrayList(SkeletonAttachmentData*)* get_only attachmentDataList;
}
SkeletonSlotData;


typedef struct
{
	char* get_only name;
	char* get_only stringValue;
	int   get_only intValue;
	float get_only floatValue;
}
SkeletonEventData;


typedef struct
{
	ArrayList(SkeletonTimeline*) get_only timelineArr[1];
	const char*                  get_only name;
	float                        get_only duration;
}
SkeletonAnimationData;


typedef struct
{
	/**
	 * The slotAttachmentMap key is slotData name,        value is attachmentDataMap
	 * the attachmentDataMap key is slot attachment name, value is SkeletonAttachmentData
	 */
	ArrayStrMap
	(
        slotName,
        ArrayStrMap(attachmentName, SkeletonAttachmentData*)*
    )
    slotAttachmentMap[1];
	const char* name;
}
SkeletonSkinData;


//--------------------------------------------------------------------------------------------------


typedef enum
{
	skeleton_attachment_region,
	skeleton_attachment_mesh,
	skeleton_attachment_skinned_mesh,
	skeleton_attachment_boundingbox,
}
SkeletonAttachmentDataType;


typedef struct
{
	/**
	 * Attachment actual name
	 */
	const char*                get_only name;

	/**
	 * Subclass attachment pointer
	 */
	void*                      get_only subPtr;
	SkeletonAttachmentDataType get_only type;
}
SkeletonAttachmentData;


typedef struct
{
	SkeletonAttachmentData get_only attachmentData[1];
	float                  get_only x;
	float                  get_only y;
	float                  get_only rotationZ;
	float                  get_only scaleX;
	float                  get_only scaleY;
	float                  get_only width;
	float                  get_only height;

	/**
	 * Index in meshList from Skeleton
	 */
	int                    get_only meshIndex;

	/**
	 * Index in Mesh from meshList
	 */
	int                    get_only subMeshIndex;

	/**
	 * Texture atlas quad info
	 */
	Quad*                  get_only quad;
}
SkeletonRegionAttachmentData;


typedef struct
{
	SkeletonAttachmentData get_only attachmentData[1];
	Array(float)           get_only vertexArr     [1];
}
SkeletonBoundingboxAttachmentData;


typedef struct
{
	SkeletonAttachmentData get_only attachmentData[1];

	Array(float)           get_only vertexArr  [1];
	Array(float)           get_only uvArr      [1];

	/**
	 * Careful 4 byte aligned
	 */
	Array(short)           get_only triangleArr[1];

	/**
	 * Index in meshList from Skeleton
	 */
	int                    get_only meshIndex;

	/**
	 * Index in Mesh from meshList
	 */
	int                    get_only subMeshIndex;

	/**
	 * Texture atlas quad info
	 */
	Quad*                  get_only quad;

	float                  get_only width;
	float                  get_only height;

	/**
	 * Whether convert uv data into TextureAtlas
	 */
	bool                   get_only isUVMappedInTexture;
}
SkeletonMeshAttachmentData;


typedef struct
{
	SkeletonMeshAttachmentData get_only meshAttachmentData[1];
	Array(int)                 get_only boneArr           [1];
	Array(float)               get_only weightArr         [1];
	Array(float)               get_only weightVertexArr   [1];
}
SkeletonSkinnedMeshAttachmentData;


typedef enum
{
	skeleton_attachment_region_mesh_index       = offsetof(SkeletonRegionAttachmentData,      meshIndex),
	skeleton_attachment_mesh_mesh_index         = offsetof(SkeletonMeshAttachmentData,        meshIndex),
	skeleton_attachment_skinned_mesh_mesh_index = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + skeleton_attachment_mesh_mesh_index,
}
SkeletonAttachmentMeshOffset;


extern SkeletonAttachmentMeshOffset skeletonAttachmentMeshOffset[3];


typedef enum
{
	skeleton_attachment_region_sub_mesh_index       = offsetof(SkeletonRegionAttachmentData,      subMeshIndex),
	skeleton_attachment_mesh_sub_mesh_index         = offsetof(SkeletonMeshAttachmentData,        subMeshIndex),
	skeleton_attachment_skinned_mesh_sub_mesh_index = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + skeleton_attachment_mesh_sub_mesh_index,
}
SkeletonAttachmentSubMeshOffset;


extern SkeletonAttachmentSubMeshOffset skeletonAttachmentSubMeshOffset[3];


//--------------------------------------------------------------------------------------------------


typedef struct
{
	ArrayStrMap(boneName,      SkeletonBoneData*)         get_only boneDataMap     [1];
	ArrayStrMap(slotName,      SkeletonSlotData*)         get_only slotDataMap     [1];
	ArrayStrMap(skinName,      SkeletonSkinData*)         get_only skinDataMap     [1];
	ArrayStrMap(animationName, SkeletonAnimationDataMap*) get_only animationDataMap[1];
	ArrayStrMap(eventName,     SkeletonEventDataMap*)     get_only eventDataMap    [1];

	SkeletonSkinData*                      get_only skinDataDefault;

	/**
	 * All SkeletonBoneData order same as in JOSN
	 */
	Array(SkeletonBoneData*)*              get_only boneDataOrderArr;

	/**
	 * All SkeletonSlotData order same as in JOSN
	 */
	Array(SkeletonSlotData*)*              get_only slotDataOrderArr;

	/**
	 * All slot attachments
	 */
	ArrayList(SkeletonAttachmentData*)     get_only attachmentDataList[1];

	TextureAtlas*                          get_only textureAtlas;
	float                                  get_only width;
	float                                  get_only height;
	const char*                            get_only filePath;
}
SkeletonData;


struct ASkeletonData
{
	/**
	 * Scales the bones, images, and animations as they are loaded, default 1.0
	 */
	float scale;

	/**
	 * Get SkeletonData by filePath, not found will create one
	 */
	SkeletonData*           (*Get)                        (const char*       filePath);
	void                    (*Release)                    (SkeletonData*     skeletonData);
	SkeletonAttachmentData* (*GetAttachmentDataBySkinData)(SkeletonSkinData* skinData, const char* slotName, const char* attachmentName);
};


extern struct ASkeletonData ASkeletonData[1];


#endif
