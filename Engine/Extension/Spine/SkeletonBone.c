/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-7-2
 * Author : scott.cgi
 * Version: 0.0.0
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
    SkeletonBone* bone = (SkeletonBone*) malloc(sizeof(SkeletonBone));
    Init(boneData, bone);

    return bone;
}


struct ASkeletonBone ASkeletonBone[1] =
{
    Create,
    Init,
    SetToSetupPose,
};
