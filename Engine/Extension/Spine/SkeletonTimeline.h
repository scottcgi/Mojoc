/*
 *
 *
 *  Created on: 2013-7-3
 *      Author: scott.cgi
 */

#ifndef skeleton_timeline_h
#define skeleton_timeline_h


#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"


typedef enum
{
	skeleton_curve_linear  = -1,
	skeleton_curve_stepped = -2,
	skeleton_curve_bezier  = -3,
}
SkeletonCurveType;


typedef struct
{
	Array(float)* get_only curveArr;
}
SkeletonCurveTimeline;


typedef struct SkeletonTimeline SkeletonTimeline;
struct  SkeletonTimeline
{
	/**
	 * Set bone slot attachment effect for this animation in this time
	 * the mixPercent is percent to set the effect, range in [0, 1]
	 * the time is second
	 */
	void  (*Apply)  (SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent);

	/**
	 * Release self subclass member space
	 */
	void  (*Release)(SkeletonTimeline* skeletonTimeline);

	/**
	 * Subclass pointer which inheritance SkeletonTimeline
	 */
	void* get_only subPtr;
};


typedef struct
{
	SkeletonCurveTimeline get_only curveTimeline   [1];
	SkeletonTimeline      get_only skeletonTimeline[1];

	/**
	 * Each group has time, angle
	 */
	Array(float)*         get_only frameArr;

	/**
	 * Index in SkeletonData's boneDataOrderArr
	 */
	int                   get_only boneIndex;
}
SkeletonRotateTimeline;


typedef struct
{
	SkeletonCurveTimeline get_only curveTimeline   [1];
	SkeletonTimeline      get_only skeletonTimeline[1];

	/**
	 * Each group has time, x, y
	 */
	Array(float)*         get_only frameArr;

	/**
	 * Index in SkeletonData's boneDataOrderArr
	 */
	int                   get_only boneIndex;
}
SkeletonTranslateTimeline;



typedef struct
{
	SkeletonTranslateTimeline get_only translateTimeline[1];
}
SkeletonScaleTimeline;



typedef struct
{
	SkeletonCurveTimeline get_only curveTimeline   [1];
	SkeletonTimeline      get_only skeletonTimeline[1];

	/**
	 * Each group has time, r, g, b, a
	 */
	Array(float)*         get_only frameArr;

	/**
	 * Index in SkeletonData's slotDataOrderArr
	 */
	int                   get_only slotIndex;
}
SkeletonColorTimeline;



typedef struct
{
	SkeletonTimeline get_only skeletonTimeline[1];

	/**
	 * Each group has time
	 */
	Array(float)*    get_only frameArr;
	Array(char*)*    get_only attachmentNameArr;

	/**
	 * Index in SkeletonData's slotDataOrderArr
	 */
	int              get_only slotIndex;
	int              get_only preFrameIndex;
}
SkeletonAttachmentTimeline;



typedef struct
{
	SkeletonTimeline            get_only skeletonTimeline[1];

	/**
	 * Each group has time
	 */
	Array(float)*               get_only frameArr;
	Array(SkeletonEventData*)*  get_only eventArr;
	int                         get_only preFrameIndex;
}
SkeletonEventTimeline;



typedef struct
{
	SkeletonTimeline    get_only skeletonTimeline[1];

	/**
	 * Each group has time
	 */
	Array(float)*       get_only frameArr;

	/**
	 * Each frame has new drawOrder
	 */
	Array(Array(int)*)* get_only drawOrderArr;
	int                 get_only preFrameIndex;
}
SkeletonDrawOrderTimeline;



typedef struct
{
	SkeletonCurveTimeline   get_only curveTimeline   [1];
	SkeletonTimeline        get_only skeletonTimeline[1];

	/**
	 * Each group has time ...
	 */
	Array(float)*           get_only frameArr;

	/**
	 * Each frame has new vertices
	 */
	Array(Array(float)*)*   get_only vertexArr;

	/**
	 * Index in SkeletonData's slotDataOrderArr
	 */
	int                     get_only slotIndex;

	SkeletonAttachmentData* get_only attachmentData;
}
SkeletonDeformTimeline;


struct ASkeletonTimeline
{
	SkeletonRotateTimeline*     (*CreateRotate)      (int frameCount, int boneIndex);
	SkeletonTranslateTimeline*  (*CreateTranslate)   (int frameCount, int boneIndex);
	SkeletonScaleTimeline*      (*CreateScale)       (int frameCount, int boneIndex);
	SkeletonColorTimeline*      (*CreateColor)       (int frameCount);
	SkeletonAttachmentTimeline* (*CreateAttachment)  (int frameCount);
	SkeletonEventTimeline*      (*CreateEvent)       (int frameCount);
	SkeletonDrawOrderTimeline*  (*CreateDrawOrder)   (int frameCount);
	SkeletonDeformTimeline*     (*CreateDeform)      (int frameCount);


	void                        (*SetLinear)         (SkeletonCurveTimeline* curveTimeline, int frameIndex);
	void                        (*SetStepped)        (SkeletonCurveTimeline* curveTimeline, int frameIndex);

	/**
	 * Sets the control handle positions for an interpolation bezier curve
	 * used to transition from this keyframe to the next
	 *
	 * cx1 and cx2 are from 0 to 1, representing the percent of time between the two keyframes
	 * cy1 and cy2 are the percent of the difference between the keyframe's values
	 */
	void                        (*SetCurve)          (SkeletonCurveTimeline* curveTimeline, int frameIndex, float cx1, float cy1, float cx2, float cy2);


	/** Set the time and angle of the specified keyframe */
	void                        (*SetRotateFrame)    (SkeletonRotateTimeline*     rotateTimeline,     int frameIndex, float time, float angle);

	/** Set the time and value of the specified keyframe */
	void                        (*SetTranslateFrame) (SkeletonTranslateTimeline*  translateTimeline,  int frameIndex, float time, float x, float y);
	void                        (*SetColorFrame)     (SkeletonColorTimeline*      colorTimeline,      int frameIndex, float time, Color*             color);
	void                        (*SetAttachmentFrame)(SkeletonAttachmentTimeline* attachmentTimeline, int frameIndex, float time, char*              attachmentName);
	void                        (*SetEventFrame)     (SkeletonEventTimeline*      eventTimeline,      int frameIndex, float time, SkeletonEventData* eventData);
	void                        (*SetDrawOrderFrame) (SkeletonDrawOrderTimeline*  drawOrderTimeline,  int frameIndex, float time, Array(int)*        drawOrder);
	void                        (*SetDeformFrame)    (SkeletonDeformTimeline*     deformTimeline,     int frameIndex, float time, Array(float)*      vertices);
};


extern struct ASkeletonTimeline ASkeletonTimeline[1];


#endif
