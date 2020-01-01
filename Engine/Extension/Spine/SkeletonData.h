/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-6-27
 * Update   : 2019-2-13
 * Author   : scott.cgi
 */


#ifndef SKELETON_DATA_H
#define SKELETON_DATA_H


#include <stdbool.h>
#include "Engine/Toolkit/Utils/ArrayStrMap.h"
#include "Engine/Graphics/Draw/Color.h"
#include "Engine/Graphics/OpenGL/Mesh.h"
#include "Engine/Extension/TextureAtlas.h"


typedef struct SkeletonBoneData SkeletonBoneData;

/**
 * The skeleton bone data read from json file.
 */
struct  SkeletonBoneData
{
    SkeletonBoneData* parent;
    const char*       name;
    float             length;
    float             x;
    float             y;
    float             rotationZ;
    float             scaleX;
    float             scaleY;
    bool              isInheritScale;
    bool              isInheritRotation;
};


/**
 * The skeleton slot data read from json file.
 */
typedef struct
{
    const char*                         name;
    const char*                         attachmentName;
    bool                                isAdditiveBlending;
    SkeletonBoneData*                   boneData;
    Color                               color[1];

    /**
     * The skin's SkeletonAttachmentData may not exits.
     */
    ArrayList(SkeletonAttachmentData*)* attachmentDataList;
}
SkeletonSlotData;


/**
 * The skeleton event data read from json file.
 */
typedef struct
{
    const char* name;
    const char* stringValue;
    int         intValue;
    float       floatValue;
}
SkeletonEventData;


/**
 * The skeleton animation data read from json file.
 */
typedef struct
{
    const char*                  name;
    float                        duration;
    ArrayList(SkeletonTimeline*) timelineArr[1];
}
SkeletonAnimationData;


/**
 * The skeleton skin data read from json file.
 */
typedef struct
{
    /**
     * The slotAttachmentMap key is slotData name,        value is attachmentDataMap.
     * the attachmentDataMap key is slot attachment name, value is SkeletonAttachmentData.
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


//----------------------------------------------------------------------------------------------------------------------


typedef enum
{
    SkeletonAttachmentDataType_Region,
    SkeletonAttachmentDataType_Mesh,
    SkeletonAttachmentDataType_SkinnedMesh,
    SkeletonAttachmentDataType_BoundingBox,
}
SkeletonAttachmentDataType;


/**
 * The abstract skeleton attachment data.
 */
typedef struct
{
    /**
     * Attachment actual name.
     */
    const char*                name;

    /**
     * Subclass attachment pointer.
     */
    void*                      childPtr;
    SkeletonAttachmentDataType type;
}
SkeletonAttachmentData;


/**
 * The skeleton region attachment data read from json file.
 */
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
     * Index of meshList in Skeleton.
     */
    int                    meshIndex;

    /**
     * Index of SubMeshList in Mesh.
     */
    int                    subMeshIndex;

    /**
     * Texture atlas quad.
     */
    Quad*                  quad;
}
SkeletonRegionAttachmentData;


/**
 * The skeleton boundingBox attachment data read from json file.
 */
typedef struct
{
    SkeletonAttachmentData attachmentData[1];
    Array(float)           vertexArr     [1];
}
SkeletonBoundingBoxAttachmentData;


/**
 * The skeleton mesh attachment data read from json file.
 */
typedef struct
{
    float                  width;
    float                  height;
    SkeletonAttachmentData attachmentData[1];

    Array(float)           vertexArr     [1];
    Array(float)           uvArr         [1];

    /**
     * Careful 4 byte aligned.
     */
    Array(short)           triangleArr   [1];

    /**
     * Index of meshList in Skeleton.
     */
    int                    meshIndex;

    /**
     * Index of SubMeshList in Mesh.
     */
    int                    subMeshIndex;

    /**
     * Texture atlas quad.
     */
    Quad*                  quad;

    /**
     * Whether convert uv data to TextureAtlas.
     */
    bool                   isUVMappedInTexture;
}
SkeletonMeshAttachmentData;


/**
 * The skeleton skinned mesh attachment data read from json file.
 */
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
    SkeletonAttachmentMeshOffset_RegionIndex      = (int) offsetof(SkeletonRegionAttachmentData, meshIndex),
    SkeletonAttachmentMeshOffset_MeshIndex        = (int) offsetof(SkeletonMeshAttachmentData,   meshIndex),
    SkeletonAttachmentMeshOffset_SkinnedMeshIndex = (int) offsetof
                                                          (
                                                              SkeletonSkinnedMeshAttachmentData,
                                                              meshAttachmentData
                                                          )
                                                          + SkeletonAttachmentMeshOffset_MeshIndex,
}
SkeletonAttachmentMeshOffset;


extern SkeletonAttachmentMeshOffset skeletonAttachmentMeshOffset[3];


typedef enum
{
    SkeletonAttachmentSubMeshOffset_RegionSubIndex      = (int) offsetof(SkeletonRegionAttachmentData, subMeshIndex),
    SkeletonAttachmentSubMeshOffset_MeshSubIndex        = (int) offsetof(SkeletonMeshAttachmentData,   subMeshIndex),
    SkeletonAttachmentSubMeshOffset_SkinnedMeshSubIndex = (int) offsetof
                                                                (
                                                                    SkeletonSkinnedMeshAttachmentData,
                                                                    meshAttachmentData
                                                                )
                                                                + SkeletonAttachmentSubMeshOffset_MeshSubIndex,
}
SkeletonAttachmentSubMeshOffset;


extern SkeletonAttachmentSubMeshOffset skeletonAttachmentSubMeshOffset[3];


//----------------------------------------------------------------------------------------------------------------------


/**
 * The skeleton data read from json file.
 */
typedef struct
{
    float                                                 width;
    float                                                 height;
    const char*                                           filePath;
    TextureAtlas*                                         textureAtlas;
    SkeletonSkinData*                                     skinDataDefault;

    ArrayStrMap(boneName,      SkeletonBoneData*)         boneDataMap     [1];
    ArrayStrMap(slotName,      SkeletonSlotData*)         slotDataMap     [1];
    ArrayStrMap(skinName,      SkeletonSkinData*)         skinDataMap     [1];
    ArrayStrMap(animationName, SkeletonAnimationDataMap*) animationDataMap[1];
    ArrayStrMap(eventName,     SkeletonEventDataMap*)     eventDataMap    [1];

    /**
     * All SkeletonBoneData order same as in JSON.
     */
    Array(SkeletonBoneData*)*                             boneDataOrderArr;

    /**
     * All SkeletonSlotData order same as in JSON.
     */
    Array(SkeletonSlotData*)*                             slotDataOrderArr;

    /**
     * All slot attachments.
     */
    ArrayList(SkeletonAttachmentData*)                    attachmentDataList[1];
}
SkeletonData;


/**
 * Control and mange SkeletonData.
 */
struct ASkeletonData
{
    /**
     * Scales the bones, images, and animations as they are loaded, default 1.0.
     */
    float scale;

    /**
     * Get SkeletonData by jsonFilePath, not found will create one.
     *
     * jsonFilePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    SkeletonData*           (*Get)                        (const char*          jsonFilePath);
    void                    (*Release)                    (SkeletonData*        skeletonData);
    SkeletonAttachmentData* (*GetAttachmentDataBySkinData)(
                                                              SkeletonSkinData* skinData,
                                                              const char*       slotName,
                                                              const char*       attachmentName
                                                          );
};


extern struct ASkeletonData ASkeletonData[1];


#endif
