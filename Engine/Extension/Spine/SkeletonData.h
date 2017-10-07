/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-6-27
 * Author : scott.cgi
 * Version: 0.0.0
 */

#ifndef SKELETON_DATA_H
#define SKELETON_DATA_H


#include <stdbool.h>
#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Extension/TextureAtlas.h"


typedef struct SkeletonBoneData SkeletonBoneData;
struct  SkeletonBoneData
{
    SkeletonBoneData* parent;
    char*             name;
    float             length;
    float             x;
    float             y;
    float             rotationZ;
    float             scaleX;
    float             scaleY;
    bool              isInheritScale;
    bool              isInheritRotation;
};


typedef struct
{
    char*                               name;
    char*                               attachmentName;
    bool                                isAdditiveBlending;
    SkeletonBoneData*                   boneData;
    Color                               color[1];

    /**
     * Each skin corresponding SkeletonAttachmentData may not exits
     */
    ArrayList(SkeletonAttachmentData*)* attachmentDataList;
}
SkeletonSlotData;


typedef struct
{
    char* name;
    char* stringValue;
    int   intValue;
    float floatValue;
}
SkeletonEventData;


typedef struct
{
    char*                        name;
    float                        duration;
    ArrayList(SkeletonTimeline*) timelineArr[1];
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

    char* name;
}
SkeletonSkinData;


//----------------------------------------------------------------------------------------------------------------------


typedef enum
{
    SkeletonAttachmentDataType_Region,
    SkeletonAttachmentDataType_Mesh,
    SkeletonAttachmentDataType_SkinnedMesh,
    SkeletonAttachmentDataType_BoundingBox,
}
SkeletonAttachmentDataType;


typedef struct
{
    /**
     * Attachment actual name
     */
    char*                      name;

    /**
     * Subclass attachment pointer
     */
    void*                      childPtr;
    SkeletonAttachmentDataType type;
}
SkeletonAttachmentData;


typedef struct
{
    float                  x;
    float                  y;
    float                  rotationZ;
    float                  scaleX;
    float                  scaleY;
    float                  width;
    float                  height;
    SkeletonAttachmentData attachmentData[1];

    /**
     * Index in meshList from Skeleton
     */
    int                    meshIndex;

    /**
     * Index in Mesh from meshList
     */
    int                    subMeshIndex;

    /**
     * Texture atlas quad info
     */
    Quad*                  quad;
}
SkeletonRegionAttachmentData;


typedef struct
{
    SkeletonAttachmentData attachmentData[1];
    Array(float)           vertexArr     [1];
}
SkeletonBoundingBoxAttachmentData;


typedef struct
{
    float                  width;
    float                  height;
    SkeletonAttachmentData attachmentData[1];

    Array(float)           vertexArr     [1];
    Array(float)           uvArr         [1];

    /**
     * Careful 4 byte aligned
     */
    Array(short)           triangleArr   [1];

    /**
     * Index in meshList from Skeleton
     */
    int                    meshIndex;

    /**
     * Index in Mesh from meshList
     */
    int                    subMeshIndex;

    /**
     * Texture atlas quad info
     */
    Quad*                  quad;

    /**
     * Whether convert uv data into TextureAtlas
     */
    bool                   isUVMappedInTexture;
}
SkeletonMeshAttachmentData;


typedef struct
{
    Array(int)                 boneArr           [1];
    Array(float)               weightArr         [1];
    Array(float)               weightVertexArr   [1];
    SkeletonMeshAttachmentData meshAttachmentData[1];
}
SkeletonSkinnedMeshAttachmentData;


typedef enum
{
    SkeletonAttachmentMeshOffset_RegionIndex      = offsetof(SkeletonRegionAttachmentData,      meshIndex),
    SkeletonAttachmentMeshOffset_MeshIndex        = offsetof(SkeletonMeshAttachmentData,        meshIndex),
    SkeletonAttachmentMeshOffset_SkinnedMeshIndex = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + SkeletonAttachmentMeshOffset_MeshIndex,
}
SkeletonAttachmentMeshOffset;


extern SkeletonAttachmentMeshOffset skeletonAttachmentMeshOffset[3];


typedef enum
{
    SkeletonAttachmentSubMeshOffset_RegionSubIndex      = offsetof(SkeletonRegionAttachmentData,      subMeshIndex),
    SkeletonAttachmentSubMeshOffset_MeshSubIndex        = offsetof(SkeletonMeshAttachmentData,        subMeshIndex),
    SkeletonAttachmentSubMeshOffset_SkinnedMeshSubIndex = offsetof(SkeletonSkinnedMeshAttachmentData, meshAttachmentData) + SkeletonAttachmentSubMeshOffset_MeshSubIndex,
}
SkeletonAttachmentSubMeshOffset;


extern SkeletonAttachmentSubMeshOffset skeletonAttachmentSubMeshOffset[3];


//----------------------------------------------------------------------------------------------------------------------


typedef struct
{
    float                                                 width;
    float                                                 height;
    char*                                                 filePath;
    TextureAtlas*                                         textureAtlas;
    SkeletonSkinData*                                     skinDataDefault;

    ArrayStrMap(boneName,      SkeletonBoneData*)         boneDataMap     [1];
    ArrayStrMap(slotName,      SkeletonSlotData*)         slotDataMap     [1];
    ArrayStrMap(skinName,      SkeletonSkinData*)         skinDataMap     [1];
    ArrayStrMap(animationName, SkeletonAnimationDataMap*) animationDataMap[1];
    ArrayStrMap(eventName,     SkeletonEventDataMap*)     eventDataMap    [1];

    /**
     * All SkeletonBoneData order same as in JOSN
     */
    Array(SkeletonBoneData*)*                             boneDataOrderArr;

    /**
     * All SkeletonSlotData order same as in JOSN
     */
    Array(SkeletonSlotData*)*                             slotDataOrderArr;

    /**
     * All slot attachments
     */
    ArrayList(SkeletonAttachmentData*)                    attachmentDataList[1];
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
    SkeletonData*           (*Get)                        (char*             filePath);
    void                    (*Release)                    (SkeletonData*     skeletonData);
    SkeletonAttachmentData* (*GetAttachmentDataBySkinData)(SkeletonSkinData* skinData, char* slotName, char* attachmentName);
};


extern struct ASkeletonData ASkeletonData[1];


#endif
