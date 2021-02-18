/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2013-7-3
 * Update   : 2019-2-1
 * Author   : scott.cgi
 */


#ifndef SKELETON_TIMELINE_H
#define SKELETON_TIMELINE_H


#include "Engine/Extension/Spine/Skeleton.h"
#include "Engine/Toolkit/Utils/Array.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"


typedef enum
{
    SkeletonCurveType_Linear  = -1,
    SkeletonCurveType_Stepped = -2,
    SkeletonCurveType_Bezier  = -3,
}
SkeletonCurveType;


typedef struct
{
    Array(float)* curveArr;
}
SkeletonCurveTimeline;


typedef struct SkeletonTimeline SkeletonTimeline;

/**
 * The abstract skeleton animation timeline.
 * the subclass implement the functions.
 */
struct SkeletonTimeline
{
    /**
     * Set bone slot attachment effect for this animation in this time.
     * the mixPercent is percent to set the effect, range in [0, 1].
     * the time is seconds
     */
    void  (*Apply)  (SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent);

    /**
     * Release member memory space.
     */
    void  (*Release)(SkeletonTimeline* skeletonTimeline);

    /**
     * Subclass pointer which inheritance SkeletonTimeline.
     */
    void* childPtr;
};


/**
 * The skeleton animation rotation timeline.
 */
typedef struct
{
    SkeletonCurveTimeline curveTimeline   [1];
    SkeletonTimeline      skeletonTimeline[1];

    /**
     * Each group has time angle, as [time, angle...]
     */
    Array(float)*         frameArr;

    /**
     * Index in SkeletonData's boneDataOrderArr.
     */
    int                   boneIndex;
}
SkeletonRotateTimeline;


/**
 * The skeleton animation translate timeline.
 */
typedef struct
{
    SkeletonCurveTimeline curveTimeline   [1];
    SkeletonTimeline      skeletonTimeline[1];

    /**
     * Each group has time x y, as [time, x, y...]
     */
    Array(float)*         frameArr;

    /**
     * Index in SkeletonData's boneDataOrderArr.
     */
    int                   boneIndex;
}
SkeletonTranslateTimeline;


/**
 * The skeleton animation scale timeline.
 */
typedef struct
{
    SkeletonTranslateTimeline translateTimeline[1];
}
SkeletonScaleTimeline;


/**
 * The skeleton animation color timeline.
 */
typedef struct
{
    SkeletonCurveTimeline curveTimeline   [1];
    SkeletonTimeline      skeletonTimeline[1];

    /**
     * Each group has time r g b a, as [time, r, g, b, a...]
     */
    Array(float)*         frameArr;

    /**
     * Index in SkeletonData's slotDataOrderArr.
     */
    int                   slotIndex;
}
SkeletonColorTimeline;


/**
 * The skeleton animation attachment timeline.
 */
typedef struct
{
    SkeletonTimeline    skeletonTimeline[1];

    /**
     * Each group has time, as [time...].
     */
    Array(float)*       frameArr;
    Array(const char*)* attachmentNameArr;

    /**
     * Index in SkeletonData's slotDataOrderArr.
     */
    int                 slotIndex;
    int                 preFrameIndex;
}
SkeletonAttachmentTimeline;


/**
 * The skeleton animation event timeline.
 */
typedef struct
{
    SkeletonTimeline           skeletonTimeline[1];

    /**
     * Each group has time, as [time...].
     */
    Array(float)*              frameArr;
    Array(SkeletonEventData*)* eventArr;
    int                        preFrameIndex;
}
SkeletonEventTimeline;


/**
 * The skeleton animation drawOrder timeline.
 */
typedef struct
{
    SkeletonTimeline    skeletonTimeline[1];

    /**
     * Each group has time, as [time...].
     */
    Array(float)*       frameArr;

    /**
     * Each frame has a new drawOrder array.
     */
    Array(Array(int)*)* drawOrderArr;
    int                 preFrameIndex;
}
SkeletonDrawOrderTimeline;


/**
 * The skeleton animation deform timeline.
 */
typedef struct
{
    SkeletonCurveTimeline   curveTimeline   [1];
    SkeletonTimeline        skeletonTimeline[1];

    /**
     * Each group has time, as [time...].
     */
    Array(float)*           frameArr;

    /**
     * Each frame has a new vertex array.
     */
    Array(Array(float)*)*   vertexArr;

    /**
     * Index in SkeletonData's slotDataOrderArr.
     */
    int                     slotIndex;


    SkeletonAttachmentData* attachmentData;
}
SkeletonDeformTimeline;


/**
 * Control and manage SkeletonTimeline.
 */
struct ASkeletonTimeline
{
    SkeletonRotateTimeline*     (*CreateRotate)    (int frameCount, int boneIndex);
    SkeletonTranslateTimeline*  (*CreateTranslate) (int frameCount, int boneIndex);
    SkeletonScaleTimeline*      (*CreateScale)     (int frameCount, int boneIndex);
    SkeletonColorTimeline*      (*CreateColor)     (int frameCount);
    SkeletonAttachmentTimeline* (*CreateAttachment)(int frameCount);
    SkeletonEventTimeline*      (*CreateEvent)     (int frameCount);
    SkeletonDrawOrderTimeline*  (*CreateDrawOrder) (int frameCount);
    SkeletonDeformTimeline*     (*CreateDeform)    (int frameCount);

    void (*SetLinear) (SkeletonCurveTimeline* curveTimeline, int frameIndex);
    void (*SetStepped)(SkeletonCurveTimeline* curveTimeline, int frameIndex);

    /**
     * Sets the control handle positions for an interpolation bezier curve.
     * used to transition from this keyframe to the next.
     *
     * cx1 and cx2 are from 0 to 1, representing the percent of time between the two keyframes.
     * cy1 and cy2 are the percent of the difference between the keyframe's values.
     */
    void (*SetCurve)          (
                                  SkeletonCurveTimeline* curveTimeline,
                                  int                    frameIndex,
                                  float                  cx1,
                                  float                  cy1,
                                  float                  cx2,
                                  float                  cy2
                              );

    /**
     * Set the time and angle of the specified keyframe.
     */
    void (*SetRotateFrame)    (SkeletonRotateTimeline* rotateTimeline, int frameIndex, float time, float angle);

    /*  Set the time and value of the specified keyframe. */
    
    void (*SetTranslateFrame) (
                                  SkeletonTranslateTimeline*  translateTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  float                       x,
                                  float                       y
                              );
    void (*SetColorFrame)     (
                                  SkeletonColorTimeline*      colorTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  Color*                      color
                              );
    void (*SetAttachmentFrame)(
                                  SkeletonAttachmentTimeline* attachmentTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  const char*                 attachmentName
                              );
    void (*SetEventFrame)     (
                                  SkeletonEventTimeline*      eventTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  SkeletonEventData*          eventData
                              );
    void (*SetDrawOrderFrame) (
                                  SkeletonDrawOrderTimeline*  drawOrderTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  Array(int)*                 drawOrder
                              );
    void (*SetDeformFrame)    (
                                  SkeletonDeformTimeline*     deformTimeline,
                                  int                         frameIndex,
                                  float                       time,
                                  Array(float)*               vertexArr
                              );
};


extern struct ASkeletonTimeline ASkeletonTimeline[1];


#endif
