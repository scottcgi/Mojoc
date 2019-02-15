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

#include "Engine/Extension/Spine/SkeletonBone.h"


static void SetToSetupPose(SkeletonBone* bone)
{
    Drawable*         drawable = bone->drawable;
    SkeletonBoneData* boneData = bone->boneData;

    ADrawable_SetPosition2(drawable, boneData->x,      boneData->y);
    ADrawable_SetScale2   (drawable, boneData->scaleX, boneData->scaleY);
    ADrawable_SetRotationZ(drawable, boneData->rotationZ);
}


static void Init(SkeletonBoneData* boneData, SkeletonBone* outBone)
{
    ADrawable->Init(outBone->drawable);
    outBone->boneData = boneData;
    SetToSetupPose(outBone);
}


static SkeletonBone* Create(SkeletonBoneData* boneData)
{
    SkeletonBone* bone = malloc(sizeof(SkeletonBone));
    Init(boneData, bone);

    return bone;
}


struct ASkeletonBone ASkeletonBone[1] =
{
    Create,
    Init,
    SetToSetupPose,
};
