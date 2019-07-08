/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-7-2
 * Update : 2019-2-13
 * Author : scott.cgi
 */


#ifndef SKELETON_BONE_H
#define SKELETON_BONE_H


#include "Engine/Extension/Spine/SkeletonData.h"
#include "Engine/Graphics/Draw/Drawable.h"


/**
 * The skeleton bone can transform and draw by drawable.
 */
typedef struct
{
    Drawable          drawable[1];
    SkeletonBoneData* boneData;
}
SkeletonBone;


/**
 * Control SkeletonBone.
 */
struct ASkeletonBone
{
    SkeletonBone* (*Create)        (SkeletonBoneData* boneData);
    void          (*Init)          (SkeletonBoneData* boneData, SkeletonBone* outBone);

    /**
     * Set bone to setup pose.
     */
    void          (*SetToSetupPose)(SkeletonBone* bone);
};


extern struct ASkeletonBone ASkeletonBone[1];


#endif
