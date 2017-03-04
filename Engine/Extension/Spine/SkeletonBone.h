/*
 *
 *
 *  Created on: 2013-7-2
 *      Author: scott.cgi
 */

#ifndef skeleton_bone_h
#define skeleton_bone_h

#include "Mojoc/Extension/Spine/SkeletonData.h"
#include "Mojoc/Graphics/Draw/Drawable.h"

typedef struct
{
	Drawable          drawable[1];
	SkeletonBoneData* boneData;
}
SkeletonBone;


struct ASkeletonBone
{
	SkeletonBone* (*Create)        (SkeletonBoneData* boneData);
	void          (*Init)          (SkeletonBoneData* boneData, SkeletonBone* outBone);

	/** Set bone to setup pose */
	void          (*SetToSetupPose)(SkeletonBone* bone);
};

extern struct ASkeletonBone ASkeletonBone[1];

#endif
