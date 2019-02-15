/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-7-2
 * Update: 2019-2-13
 * Author: scott.cgi
 */


#ifndef SKELETON_BONE_H
#define  SKELETON_BONE_H


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
