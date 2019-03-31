/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The Mojoc game engine is a serious project with extreme code pursuit,
 * and will continue to iterate.
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
