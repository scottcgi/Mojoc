/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-7-3
 * Author: scott.cgi
 */

#include <stdbool.h>
#include <string.h>

#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/Spine/SkeletonTimeline.h"
#include "Engine/Toolkit/Platform/Log.h"


/**
 * The target After the first and before the last entry
 */
static inline int BinarySearchByStep(float* values, int valuesLength, float target, int step)
{
    int low  = 0;
    int high = valuesLength / step - 2;

    if (high <= 0)
    {
        return step;
    }

    int current = high >> 1;

    while (true)
    {
        if (values[(current + 1) * step] <= target)
        {
            low  = current + 1;
        }
        else
        {
            high = current;
        }

        if (low == high)
        {
            return (low + 1) * step;
        }

        current = (low + high) >> 1;
    }

    return 0;
}


/**
 * The target After the first and before the last entry
 */
static inline int BinarySearch(float* values, int valuesLength, float target)
{
    int low  = 0;
    int high = valuesLength - 2;

    if (high <= 0)
    {
        return 1;
    }

    int current = high >> 1;

    while (true)
    {
        if (values[current + 1] <= target)
        {
            low  = current + 1;
        }
        else
        {
            high = current;
        }

        if (low == high)
        {
            return low + 1;
        }

        current = (low + high) >> 1;
    }

    return 0;
}


#define BEZIER_SEGMENTS 12
static const int bezierSize = (BEZIER_SEGMENTS + 1) * 2 - 1;


static inline void SetLinear(SkeletonCurveTimeline* curveTimeline, int frameIndex)
{
    AArray_Set(curveTimeline->curveArr, frameIndex * bezierSize, SkeletonCurveType_Linear, float);
}


static inline void SetStepped(SkeletonCurveTimeline* curveTimeline, int frameIndex)
{
    AArray_Set(curveTimeline->curveArr, frameIndex * bezierSize, SkeletonCurveType_Stepped, float);
}


/*
------------------------------------------------------------------------------------------------------------------------

static inline int GetFrameCount(SkeletonCurveTimeline* curveTimeline)
{
    return curveTimeline->curveArr->length / bezierSize;
}


static inline SkeletonCurveType GetCurveType(SkeletonCurveTimeline* curveTimeline, int frameIndex)
{
    int index = frameIndex * bezierSize;

    if (index == curveTimeline->curveArr->length)
    {
        return SkeletonCurveType_Linear;
    }

    int type = (int) AArray_Get(curveTimeline->curveArr, index, float);

    if (type == SkeletonCurveType_Linear)
    {
        return SkeletonCurveType_Linear;
    }
    else if (type == SkeletonCurveType_Stepped)
    {
        return SkeletonCurveType_Stepped;
    }

    return SkeletonCurveType_Bezier;
}
------------------------------------------------------------------------------------------------------------------------
*/


static const float  subDivStep  = 1.0f        / BEZIER_SEGMENTS;
static const float  subDivStep2 = subDivStep  * subDivStep;
static const float  subDivStep3 = subDivStep2 * subDivStep;
static const float  pre1        = 3           * subDivStep;
static const float  pre2        = 3           * subDivStep2;
static const float  pre4        = 6           * subDivStep2;
static const float  pre5        = 6           * subDivStep3;
static const float  subDivPre   = subDivStep3 / pre5;


static void SetCurve(SkeletonCurveTimeline* curveTimeline, int frameIndex, float cx1, float cy1, float cx2, float cy2)
{
    float  tmpx   = -cx1 * 2 + cx2;
    float  tmpy   = -cy1 * 2 + cy2;

    float  dddfx  = ((cx1 - cx2) * 3 + 1) * pre5;
    float  dddfy  = ((cy1 - cy2) * 3 + 1) * pre5;

    float  ddfx   = tmpx * pre4 + dddfx;
    float  ddfy   = tmpy * pre4 + dddfy;

    float  dfx    = cx1 * pre1 + tmpx * pre2 + dddfx * subDivPre;
    float  dfy    = cy1 * pre1 + tmpy * pre2 + dddfy * subDivPre;

    float* curves = AArray_GetData(curveTimeline->curveArr, float);
    int    i      = frameIndex * bezierSize;
    curves[i++]   = SkeletonCurveType_Bezier;

    float  x      = dfx;
    float  y      = dfy;

    for (int n = i + bezierSize - 1; i < n; i += 2)
    {
        curves[i]     = x;
        curves[i + 1] = y;

        dfx          += ddfx;
        dfy          += ddfy;
        ddfx         += dddfx;
        ddfy         += dddfy;

        x            += dfx;
        y            += dfy;
    }
}


static inline float GetCurvePercent(SkeletonCurveTimeline* curveTimeline, int frameIndex, float percent)
{
    int    i      = frameIndex * bezierSize;
    float* curves = AArray_GetData(curveTimeline->curveArr, float);
    float  dfx    = curves[i];

    switch ((int) dfx)
    {
        case SkeletonCurveType_Linear:
            return AMath_Clamp(percent, 0.0f, 1.0f);

        case SkeletonCurveType_Bezier:
        {
            i++;
            percent = AMath_Clamp(percent, 0.0f, 1.0f);

            float x = 0.0f;

            for (int start = i, n = i + bezierSize - 1; i < n; i += 2)
            {
                x = curves[i];

                if (x >= percent)
                {
                    float preX;
                    float preY;

                    if (i == start)
                    {
                        preX = 0;
                        preY = 0;
                    }
                    else
                    {
                        preX = curves[i - 2];
                        preY = curves[i - 1];
                    }

                    return preY + (curves[i + 1] - preY) * (percent - preX) / (x - preX);
                }
            }

            float y = curves[i - 1];

            // last point is 1, 1
            return y + (1 - y) * (percent - x) / (1 - x);
        }

        case SkeletonCurveType_Stepped:
            break;
    }

    return 0.0f;
}


static inline void CurveRelease(SkeletonCurveTimeline* curveTimeline)
{
    free(curveTimeline->curveArr);
    curveTimeline->curveArr = NULL;
}


static inline void CurveTimelineInit(SkeletonCurveTimeline* curveTimeline, int frameCount)
{
    curveTimeline->curveArr = AArray->Create(sizeof(float), frameCount * bezierSize);
}


#undef BEZIER_SEGMENTS


//----------------------------------------------------------------------------------------------------------------------


#define ROTATE_FRAME_VALUE  1
#define ROTATE_FRAME_TIMES  2


static void SetRotateFrame(SkeletonRotateTimeline* rotateTimeline, int frameIndex, float time, float angle)
{
    float* frames          = AArray_GetData(rotateTimeline->frameArr, float);

    frameIndex            *= ROTATE_FRAME_TIMES;
    frames[frameIndex]     = time;
    frames[frameIndex + 1] = angle;
}


static void RotateApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonRotateTimeline* rotateTimeline = (SkeletonRotateTimeline*) skeletonTimeline->childPtr;

    float* frames      = AArray_GetData( rotateTimeline->frameArr, float);
    int    frameLength = rotateTimeline->frameArr->length;

    if (time < frames[0])
    {
        // time is before first frame
        return;
    }

    SkeletonBone*     bone     = AArray_GetPtr(skeleton->boneArr, rotateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData = bone->boneData;
    Drawable*         drawable = bone->drawable;

    if (time >= frames[frameLength - ROTATE_FRAME_TIMES])
    {
        // time is after last frame

        float amount = boneData->rotationZ + frames[frameLength - 1] - drawable->rotationZ;

        while (amount > 180)
        {
            amount -= 360;
        }

        while (amount < -180)
        {
            amount += 360;
        }

        ADrawable_SetRotationZ(drawable, drawable->rotationZ + amount * mixPercent);
        return;
    }

    // interpolate between the last frame and the current frame
    int frameIndex       = BinarySearchByStep(frames, frameLength, time, ROTATE_FRAME_TIMES);

    float lastFrameValue = frames[frameIndex - 1];
    float frameTime      = frames[frameIndex];
    float percent        = GetCurvePercent
                           (
                               rotateTimeline->curveTimeline, (frameIndex >> 1) - 1,
                               1.0f - (time - frameTime) / (frames[frameIndex - ROTATE_FRAME_TIMES] - frameTime)
                           );

    float amount         = frames[frameIndex + ROTATE_FRAME_VALUE] - lastFrameValue;

    while (amount > 180)
    {
        amount -= 360;
    }

    while (amount < -180)
    {
        amount += 360;
    }

    amount = boneData->rotationZ + (lastFrameValue + amount * percent) - drawable->rotationZ;

    while (amount > 180)
    {
        amount -= 360;
    }

    while (amount < -180)
    {
        amount += 360;
    }

    ADrawable_SetRotationZ(drawable, drawable->rotationZ + amount * mixPercent);
}


static void RotateRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonRotateTimeline* rotateTimeline = (SkeletonRotateTimeline*) skeletonTimeline->childPtr;

    CurveRelease(rotateTimeline->curveTimeline);
    free(rotateTimeline->frameArr);
    rotateTimeline->frameArr = NULL;
}


static SkeletonRotateTimeline* CreateRotate(int frameCount, int boneIndex)
{
    SkeletonRotateTimeline* rotateTimeline     = (SkeletonRotateTimeline*) malloc(sizeof(SkeletonRotateTimeline));

    CurveTimelineInit(rotateTimeline->curveTimeline, frameCount);

    rotateTimeline->skeletonTimeline->Apply    = RotateApply;
    rotateTimeline->skeletonTimeline->Release  = RotateRelease;
    rotateTimeline->skeletonTimeline->childPtr = rotateTimeline;

    rotateTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * ROTATE_FRAME_TIMES);
    rotateTimeline->boneIndex                  = boneIndex;

    return rotateTimeline;
}

#undef ROTATE_FRAME_VALUE
#undef ROTATE_FRAME_TIMES


//----------------------------------------------------------------------------------------------------------------------


#define TRANSLATE_FRAME_X      1
#define TRANSLATE_FRAME_Y      2
#define TRANSLATE_FRAME_TIMES  3


static void SetTranslateFrame(SkeletonTranslateTimeline* translateTimeline, int frameIndex, float time, float x, float y)
{
    float* frames          = AArray_GetData(translateTimeline->frameArr, float);

    frameIndex            *= TRANSLATE_FRAME_TIMES;
    frames[frameIndex]     = time;
    frames[frameIndex + 1] = x;
    frames[frameIndex + 2] = y;
}


static void TranslateApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonTranslateTimeline* translateTimeline = (SkeletonTranslateTimeline*) skeletonTimeline->childPtr;
    float*                     frames            = AArray_GetData(translateTimeline->frameArr, float);

    if (time < frames[0])
    {
        return; // time is before first frame
    }

    int               frameLength = translateTimeline->frameArr->length;

    SkeletonBone*     bone        = AArray_GetPtr(skeleton->boneArr, translateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData    = bone->boneData;
    Drawable*         drawable    = bone->drawable;


    if (time >= frames[frameLength - TRANSLATE_FRAME_TIMES])
    {
        // time is after last frame
        // each frame value is relative boneData x or y
        ADrawable_SetPosition2
        (
            drawable,
            drawable->positionX + (boneData->x + frames[frameLength - TRANSLATE_FRAME_Y] - drawable->positionX) * mixPercent,
            drawable->positionY + (boneData->y + frames[frameLength - TRANSLATE_FRAME_X] - drawable->positionY) * mixPercent
        );

        return;
    }

    // interpolate between the last frame and the current frame.
    int   frameIndex  = BinarySearchByStep(frames, frameLength, time, TRANSLATE_FRAME_TIMES);
    float lastFrameX  = frames[frameIndex - TRANSLATE_FRAME_Y];
    float lastFrameY  = frames[frameIndex - TRANSLATE_FRAME_X];
    float frameTime   = frames[frameIndex];

    float percent     = GetCurvePercent
                        (
                            translateTimeline->curveTimeline, frameIndex / TRANSLATE_FRAME_TIMES - 1,
                            1.0f - (time - frameTime) / (frames[frameIndex - TRANSLATE_FRAME_TIMES] - frameTime)
                        );

    ADrawable_SetPosition2
    (
        drawable,
        drawable->positionX + (boneData->x + lastFrameX + (frames[frameIndex + TRANSLATE_FRAME_X] - lastFrameX) * percent - drawable->positionX) * mixPercent,
        drawable->positionY + (boneData->y + lastFrameY + (frames[frameIndex + TRANSLATE_FRAME_Y] - lastFrameY) * percent - drawable->positionY) * mixPercent
    );
}


static void TranslateRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonTranslateTimeline* translateTimeline = (SkeletonTranslateTimeline*) skeletonTimeline->childPtr;

    CurveRelease(translateTimeline->curveTimeline);
    free(translateTimeline->frameArr);
    translateTimeline->frameArr = NULL;
}


static inline void TranslateTimelineInit(SkeletonTranslateTimeline* translateTimeline, int frameCount, int boneIndex)
{
    CurveTimelineInit(translateTimeline->curveTimeline, frameCount);

    translateTimeline->skeletonTimeline->Apply    = TranslateApply;
    translateTimeline->skeletonTimeline->Release  = TranslateRelease;
    translateTimeline->skeletonTimeline->childPtr = translateTimeline;

    translateTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * TRANSLATE_FRAME_TIMES);
    translateTimeline->boneIndex                  = boneIndex;
}


static SkeletonTranslateTimeline* CreateTranslate(int frameCount, int boneIndex)
{
    SkeletonTranslateTimeline* translateTimeline = (SkeletonTranslateTimeline*) malloc(sizeof(SkeletonTranslateTimeline));
    TranslateTimelineInit(translateTimeline, frameCount, boneIndex);

    return translateTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void ScaleApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonScaleTimeline*     scaleTimeline     = (SkeletonScaleTimeline*) skeletonTimeline->childPtr;
    SkeletonTranslateTimeline* translateTimeline = scaleTimeline->translateTimeline;

    float* frames = AArray_GetData(translateTimeline->frameArr, float);

    if (time < frames[0])
    {
        return; // time is before first frame
    }

    int               frameLength = translateTimeline->frameArr->length;

    SkeletonBone*     bone        = AArray_GetPtr(skeleton->boneArr, translateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData    = bone->boneData;
    Drawable*         drawable    = bone->drawable;

    if (time >= frames[frameLength - TRANSLATE_FRAME_TIMES])
    {
        // time is after last frame
        // each frame scale value is relative 1.0 not boneData scale
        // so we use boneData scale - 1.0f
        ADrawable_SetScale2
        (
            drawable,
            drawable->scaleX + (boneData->scaleX - 1.0f + frames[frameLength - TRANSLATE_FRAME_Y] - drawable->scaleX) * mixPercent,
            drawable->scaleY + (boneData->scaleY - 1.0f + frames[frameLength - TRANSLATE_FRAME_X] - drawable->scaleY) * mixPercent
        );

        return;
    }

    // interpolate between the last frame and the current frame.
    int   frameIndex  = BinarySearchByStep(frames, frameLength, time, TRANSLATE_FRAME_TIMES);
    float lastFrameX  = frames[frameIndex - TRANSLATE_FRAME_Y];
    float lastFrameY  = frames[frameIndex - TRANSLATE_FRAME_X];
    float frameTime   = frames[frameIndex];

    float percent     = 1.0f - (time - frameTime) / (frames[frameIndex - TRANSLATE_FRAME_TIMES] - frameTime);
    percent           = GetCurvePercent(translateTimeline->curveTimeline, frameIndex / TRANSLATE_FRAME_TIMES - 1, percent);

    ADrawable_SetScale2
    (
        drawable,
        drawable->scaleX + (boneData->scaleX - 1.0f + lastFrameX + (frames[frameIndex + TRANSLATE_FRAME_X] - lastFrameX) * percent - drawable->scaleX) * mixPercent,
        drawable->scaleY + (boneData->scaleY - 1.0f + lastFrameY + (frames[frameIndex + TRANSLATE_FRAME_Y] - lastFrameY) * percent - drawable->scaleY) * mixPercent
    );
}


#undef TRANSLATE_FRAME_X
#undef TRANSLATE_FRAME_Y
#undef TRANSLATE_FRAME_TIMES


static void ScaleRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonScaleTimeline* scaleTimeline = (SkeletonScaleTimeline*) skeletonTimeline->childPtr;
    skeletonTimeline->childPtr           = scaleTimeline->translateTimeline;
    TranslateRelease(scaleTimeline->translateTimeline->skeletonTimeline);
}


static SkeletonScaleTimeline* CreateScale(int frameCount, int boneIndex)
{
    SkeletonScaleTimeline* scaleTimeline = (SkeletonScaleTimeline*) malloc(sizeof(SkeletonScaleTimeline));
    TranslateTimelineInit(scaleTimeline->translateTimeline, frameCount, boneIndex);

    scaleTimeline->translateTimeline->skeletonTimeline->Apply    = ScaleApply;
    scaleTimeline->translateTimeline->skeletonTimeline->Release  = ScaleRelease;
    scaleTimeline->translateTimeline->skeletonTimeline->childPtr = scaleTimeline;

    return scaleTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


#define COLOR_FRAME_R     1
#define COLOR_FRAME_G     2
#define COLOR_FRAME_B     3
#define COLOR_FRAME_A     4
#define COLOR_FRAME_TIMES 5


static void SetColorFrame(SkeletonColorTimeline* colorTimeline, int frameIndex, float time,  Color* color)
{
    float* frames                      = AArray_GetData(colorTimeline->frameArr, float);

    frameIndex                        *= COLOR_FRAME_TIMES;
    frames[frameIndex]                 = time;
    frames[frameIndex + COLOR_FRAME_R] = color->r;
    frames[frameIndex + COLOR_FRAME_G] = color->g;
    frames[frameIndex + COLOR_FRAME_B] = color->b;
    frames[frameIndex + COLOR_FRAME_A] = color->a;
}


static void ColorApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonColorTimeline* colorTimeline = (SkeletonColorTimeline*) skeletonTimeline->childPtr;
    float*                 frames        = AArray_GetData(colorTimeline->frameArr, float);

    if (time < frames[0])
    {
        return; // time is before first frame
    }

    SkeletonSlot* slot = AArray_GetPtr(skeleton->slotArr, colorTimeline->slotIndex, SkeletonSlot);

    if (slot->attachmentData == NULL)
    {
        return;
    }

    int    frameLength = colorTimeline->frameArr->length;
    Color* color       = slot->color;

    if (time >= frames[frameLength - COLOR_FRAME_TIMES])
    {
        // time is after last frame.
        color->r = frames[frameLength - COLOR_FRAME_A];
        color->g = frames[frameLength - COLOR_FRAME_B];
        color->b = frames[frameLength - COLOR_FRAME_G];
        color->a = frames[frameLength - COLOR_FRAME_R];
    }
    else
    {
        // interpolate between the last frame and the current frame.
        int   frameIndex = BinarySearchByStep(frames, frameLength, time, COLOR_FRAME_TIMES);
        float frameTime  = frames[frameIndex];
        float r          = frames[frameIndex - COLOR_FRAME_A];
        float g          = frames[frameIndex - COLOR_FRAME_B];
        float b          = frames[frameIndex - COLOR_FRAME_G];
        float a          = frames[frameIndex - COLOR_FRAME_R];

        float percent    = GetCurvePercent
                           (
                                colorTimeline->curveTimeline,
                                frameIndex / COLOR_FRAME_TIMES - 1,
                                1.0f - (time - frameTime) / (frames[frameIndex - COLOR_FRAME_TIMES] - frameTime)
                           );

        r               += (frames[frameIndex + COLOR_FRAME_R] - r) * percent;
        g               += (frames[frameIndex + COLOR_FRAME_G] - g) * percent;
        b               += (frames[frameIndex + COLOR_FRAME_B] - b) * percent;
        a               += (frames[frameIndex + COLOR_FRAME_A] - a) * percent;

        if (mixPercent < 1.0f)
        {
            color->r = (r - color->r) * mixPercent;
            color->g = (g - color->g) * mixPercent;
            color->b = (b - color->b) * mixPercent;
            color->a = (a - color->a) * mixPercent;
        }
        else
        {
            color->r = r;
            color->g = g;
            color->b = b;
            color->a = a;
        }
    }

    SubMesh* subMesh = ASkeleton->GetAttachmentSubMesh(skeleton, slot->attachmentData);

    if (memcmp(subMesh->drawable->color, color, sizeof(Color)) != 0)
    {
        ADrawable_SetColor(subMesh->drawable, color);
    }
}


static void ColorRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonColorTimeline* colorTimeline = (SkeletonColorTimeline*) skeletonTimeline->childPtr;

    free(colorTimeline->frameArr);
    colorTimeline->frameArr = NULL;

    CurveRelease(colorTimeline->curveTimeline);
}


static SkeletonColorTimeline* CreateColor(int frameCount)
{
    SkeletonColorTimeline* colorTimeline      = (SkeletonColorTimeline*) malloc(sizeof(SkeletonColorTimeline));
    CurveTimelineInit(colorTimeline->curveTimeline, frameCount);

    colorTimeline->skeletonTimeline->Apply    = ColorApply;
    colorTimeline->skeletonTimeline->Release  = ColorRelease;
    colorTimeline->skeletonTimeline->childPtr = colorTimeline;

    colorTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * COLOR_FRAME_TIMES);

    return colorTimeline;
}


#undef COLOR_FRAME_TIMES
#undef COLOR_FRAME_R
#undef COLOR_FRAME_G
#undef COLOR_FRAME_B
#undef COLOR_FRAME_A


//----------------------------------------------------------------------------------------------------------------------


static void SetAttachmentFrame(SkeletonAttachmentTimeline* attachmentTimeline, int frameIndex, float time, char* attachmentName)
{
    char* copyName = (char*) malloc(strlen(attachmentName) + 1);
    strcpy(copyName, attachmentName);

    AArray_Set(attachmentTimeline->frameArr,          frameIndex, time,     float);
    AArray_Set(attachmentTimeline->attachmentNameArr, frameIndex, copyName, char*);
}


static void AttachmentApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonAttachmentTimeline* attachmentTimeline = (SkeletonAttachmentTimeline*) skeletonTimeline->childPtr;
    float*                      frames             = AArray_GetData(attachmentTimeline->frameArr, float);

    // time is before first frame
    if (time < frames[0])
    {
        attachmentTimeline->preFrameIndex = -1;
        return;
    }

    int frameLength = attachmentTimeline->frameArr->length;
    int frameIndex;

    if (time >= frames[frameLength - 1])
    {
        // time is after last frame
        frameIndex = frameLength - 1;
    }
    else
    {
        frameIndex = BinarySearch(frames, frameLength, time) - 1;
    }

    if (attachmentTimeline->preFrameIndex == frameIndex)
    {
        return;
    }
    else
    {
        attachmentTimeline->preFrameIndex = frameIndex;
    }

    SkeletonSlot*           slot           = AArray_GetPtr(skeleton->slotArr,                  attachmentTimeline->slotIndex, SkeletonSlot);
    char*                   attachmentName = AArray_Get(attachmentTimeline->attachmentNameArr, frameIndex,                    char*);
    SkeletonAttachmentData* attachmentData = ASkeleton->GetAttachmentData(slot->skeleton,      slot->slotData->name,         attachmentName);

    ASkeletonSlot->SetAttachmentData(slot, attachmentData);
}


static void AttachmentRelease(SkeletonTimeline* skeletonTimeline )
{
    SkeletonAttachmentTimeline* attachmentTimeline = (SkeletonAttachmentTimeline*) skeletonTimeline->childPtr;

    for (int i = 0; i < attachmentTimeline->attachmentNameArr->length; i++)
    {
        free(AArray_Get(attachmentTimeline->attachmentNameArr, i, char*));
    }

    free(attachmentTimeline->frameArr);
    attachmentTimeline->frameArr = NULL;

    free(attachmentTimeline->attachmentNameArr);
    attachmentTimeline->attachmentNameArr = NULL;
}


static SkeletonAttachmentTimeline* CreateAttachment(int frameCount)
{
    SkeletonAttachmentTimeline* attachmentTimeline = (SkeletonAttachmentTimeline*) malloc(sizeof(SkeletonAttachmentTimeline));

    attachmentTimeline->skeletonTimeline->Apply    = AttachmentApply;
    attachmentTimeline->skeletonTimeline->Release  = AttachmentRelease;
    attachmentTimeline->skeletonTimeline->childPtr = attachmentTimeline;

    attachmentTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount);
    attachmentTimeline->attachmentNameArr          = AArray->Create(sizeof(char*), frameCount);
    attachmentTimeline->preFrameIndex              = -1;


    return attachmentTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetEventFrame(SkeletonEventTimeline* eventTimeline, int frameIndex, float time, SkeletonEventData* eventData)
{
    AArray_Set(eventTimeline->frameArr, frameIndex, time,      float);
    AArray_Set(eventTimeline->eventArr, frameIndex, eventData, SkeletonEventData*);
}


static void EventApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    if (skeleton->FireSkeletonEvent == NULL)
    {
        return;
    }

    SkeletonEventTimeline* eventTimeline = (SkeletonEventTimeline*) skeletonTimeline->childPtr;
    float*                 frames        = AArray_GetData(eventTimeline->frameArr, float);

    // time is before first frame
    if (time < frames[0])
    {
        eventTimeline->preFrameIndex = -1;
        return;
    }

    int frameLength = eventTimeline->frameArr->length;
    int frameIndex;

    if (time >= frames[frameLength - 1])
    {
        // time is after last frame
        frameIndex = frameLength - 1;
    }
    else
    {
        frameIndex = BinarySearch(frames, frameLength, time) - 1;
    }


    if (eventTimeline->preFrameIndex == frameIndex)
    {
        return;
    }
    else
    {
        eventTimeline->preFrameIndex = frameIndex;
    }

    skeleton->FireSkeletonEvent
    (
        skeleton,
        AArray_Get(eventTimeline->eventArr, frameIndex, SkeletonEventData*),
        mixPercent
    );
}


static void EventRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonEventTimeline* eventTimeline = (SkeletonEventTimeline*) skeletonTimeline->childPtr;

    free(eventTimeline->frameArr);
    eventTimeline->frameArr = NULL;

    for (int i = 0; i < eventTimeline->eventArr->length; i++)
    {
        free(AArray_Get(eventTimeline->eventArr, i, SkeletonEventData*));
    }

    free(eventTimeline->eventArr);
    eventTimeline->eventArr = NULL;
}


static SkeletonEventTimeline* CreateEvent(int frameCount)
{
    SkeletonEventTimeline* eventTimeline      = (SkeletonEventTimeline*) malloc(sizeof(SkeletonEventTimeline));

    eventTimeline->skeletonTimeline->Apply    = EventApply;
    eventTimeline->skeletonTimeline->Release  = EventRelease;
    eventTimeline->skeletonTimeline->childPtr = eventTimeline;

    eventTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount);
    eventTimeline->eventArr                   = AArray->Create(sizeof(SkeletonEventData*), frameCount);
    eventTimeline->preFrameIndex              = -1;

    return eventTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetDrawOrderFrame(SkeletonDrawOrderTimeline*  drawOrderTimeline,  int frameIndex, float time, Array(int)* drawOrder)
{
    AArray_Set(drawOrderTimeline->frameArr,     frameIndex, time,      float);
    AArray_Set(drawOrderTimeline->drawOrderArr, frameIndex, drawOrder, Array(int)*);
}


static void DrawOrderRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline = (SkeletonDrawOrderTimeline*) skeletonTimeline->childPtr;

    free(drawOrderTimeline->frameArr);
    drawOrderTimeline->frameArr = NULL;

    for (int i = 0; i < drawOrderTimeline->drawOrderArr->length; i++)
    {
        free(AArray_Get(drawOrderTimeline->drawOrderArr, i, Array(int)*));
    }

    free(drawOrderTimeline->drawOrderArr);
    drawOrderTimeline->drawOrderArr = NULL;
}


static void DrawOrderApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline = (SkeletonDrawOrderTimeline*) skeletonTimeline->childPtr;

    float* frames = AArray_GetData(drawOrderTimeline->frameArr, float);

    // time is before first frame.
    if (time < frames[0])
    {
        drawOrderTimeline->preFrameIndex = -1;
        return;
    }

    int frameLength = drawOrderTimeline->frameArr->length;
    int frameIndex;

    if (time >= frames[frameLength - 1])
    {
        // time is after last frame.
        frameIndex = frameLength - 1;
    }
    else
    {
        frameIndex = BinarySearch(frames, frameLength, time) - 1;
    }

    if (drawOrderTimeline->preFrameIndex == frameIndex)
    {
        return;
    }
    else
    {
        drawOrderTimeline->preFrameIndex = frameIndex;
    }

    ArrayList* meshList = skeleton->meshList;
    int        subMeshIndexCount[meshList->size];

    memset(subMeshIndexCount, 0, meshList->size * sizeof(int));

    Array(int)*    drawOrderArr = AArray_Get(drawOrderTimeline->drawOrderArr, frameIndex, Array(int)*);
    SkeletonSlot*  slots        = AArray_GetData(skeleton->slotArr,      SkeletonSlot);
    SkeletonSlot** slotOrders   = AArray_GetData(skeleton->slotOrderArr, SkeletonSlot*);
    int*           drawOrder    = NULL;


    if (drawOrderArr != NULL)
    {
        drawOrder = AArray_GetData(drawOrderArr, int);
    }

    for (int i = 0; i < skeleton->slotArr->length; i++)
    {
        SkeletonSlot* slot;

        if (drawOrder == NULL)
        {
            slot = slots + i;
        }
        else
        {
            slot = slots + drawOrder[i];
        }

        slotOrders[i] = slot;

        if (slot->slotData->attachmentDataList != NULL)
        {
            ArrayList* attachmentDataList = slot->slotData->attachmentDataList;

            for(int j = 0; j < attachmentDataList->size; j++)
            {
                SubMesh* subMesh = ASkeleton->GetAttachmentSubMesh
                                   (
                                       skeleton,
                                       AArrayList_Get(attachmentDataList, j, SkeletonAttachmentData*)
                                   );

                for (int k = 0; k < meshList->size; k++)
                {
                    Mesh* mesh = AArrayList_GetPtr(meshList, k, Mesh);

                    if (subMesh->parent == mesh)
                    {
                        subMesh->index = subMeshIndexCount[k]++;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < meshList->size; i++)
    {
        AMesh->ReorderAllChildren
        (
            AArrayList_GetPtr(meshList, i, Mesh)
        );
    }
}


static SkeletonDrawOrderTimeline* CreateDrawOrder(int frameCount)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline  = (SkeletonDrawOrderTimeline*) malloc(sizeof(SkeletonDrawOrderTimeline));

    drawOrderTimeline->skeletonTimeline->Release  = DrawOrderRelease;
    drawOrderTimeline->skeletonTimeline->Apply    = DrawOrderApply;
    drawOrderTimeline->skeletonTimeline->childPtr = drawOrderTimeline;

    drawOrderTimeline->frameArr                   = AArray->Create(sizeof(float),       frameCount);
    drawOrderTimeline->drawOrderArr               = AArray->Create(sizeof(Array(int)*), frameCount);
    drawOrderTimeline->preFrameIndex              = -1;

    return drawOrderTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetDeformFrame(SkeletonDeformTimeline* deformTimeline, int frameIndex, float time, Array(float)* vertices)
{
    AArray_Set(deformTimeline->frameArr,  frameIndex, time,     float);
    AArray_Set(deformTimeline->vertexArr, frameIndex, vertices, Array(float)*);
}


static void DeformRelease(SkeletonTimeline *skeletonTimeline)
{
    SkeletonDeformTimeline* deformTimeline = (SkeletonDeformTimeline*) skeletonTimeline->childPtr;

    free(deformTimeline->frameArr);
    deformTimeline->frameArr = NULL;

    for (int i = 0; i < deformTimeline->vertexArr->length; i++)
    {
        free(AArray_Get(deformTimeline->vertexArr, i, Array(float)*));
    }

    free(deformTimeline->vertexArr);
    deformTimeline->vertexArr = NULL;

    CurveRelease(deformTimeline->curveTimeline);
}


static void DeformApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonDeformTimeline* deformTimeline = (SkeletonDeformTimeline*) skeletonTimeline->childPtr;
    SkeletonSlot*           slot           = AArray_GetPtr(skeleton->slotArr, deformTimeline->slotIndex, SkeletonSlot);

    if (slot->attachmentData != deformTimeline->attachmentData)
    {
        return;
    }

    float* frames = AArray_GetData(deformTimeline->frameArr, float);

    // time is before first frame.
    if (time < frames[0])
    {
        return;
    }

    Array* positionArr;
    float* position;

    switch (deformTimeline->attachmentData->type)
    {
        case SkeletonAttachmentDataType_Region:
        case SkeletonAttachmentDataType_BoundingBox:
            ALog_A
            (
                false,
                "ASkeletonTimeline DeformApply deformTimeline->attachmentData->type only be"
                "SkeletonAttachmentDataType_Mesh | SkeletonAttachmentDataType_SkinnedMesh"
            );
            break;

        case SkeletonAttachmentDataType_Mesh:
        {
            SkeletonMeshAttachmentData* meshAttachment = (SkeletonMeshAttachmentData*) deformTimeline->attachmentData->childPtr;
            SubMesh*                    subMesh        = AArrayList_Get
                                                         (
                                                             AArrayList_GetPtr(skeleton->meshList, meshAttachment->meshIndex, Mesh)->childList,
                                                             meshAttachment->subMeshIndex,
                                                             SubMesh*
                                                         );

            positionArr                                = subMesh->positionArr;
            position                                   = AArray_GetData(positionArr, float);

            break;
        }

        case SkeletonAttachmentDataType_SkinnedMesh:
        {
            SkeletonSkinnedMeshAttachmentData* skinnedMeshAttachment =
                (SkeletonSkinnedMeshAttachmentData*) deformTimeline->attachmentData->childPtr;

            positionArr = skinnedMeshAttachment->weightVertexArr;
            position    = AArray_GetData(positionArr, float);

            break;
        }
    }

    int frameLength = deformTimeline->frameArr->length;

    if (time >= frames[frameLength - 1])
    {
        // time is after last frame.
        float* lastVertex = AArray_GetData
                            (
                                AArray_Get(deformTimeline->vertexArr, frameLength - 1, Array(float)*),
                                float
                            );

        if (mixPercent < 1.0f)
        {
            for (int i = 0; i < positionArr->length; i += 3)
            {
                position[i]  += (lastVertex[i]  - position[i])  * mixPercent;
                int i1        = i + 1;
                position[i1] += (lastVertex[i1] - position[i1]) * mixPercent;
            }
        }
        else
        {
            memcpy(position, lastVertex, positionArr->length * sizeof(float));
        }

        return;
    }

    // interpolate between the previous frame and the current frame
    int    frameIndex = BinarySearch(frames, frameLength, time);
    float  frameTime  = frames[frameIndex];

    float  percent    = GetCurvePercent
                        (
                            deformTimeline->curveTimeline,
                            frameIndex - 1,
                            1.0f - (time - frameTime) / (frames[frameIndex - 1] - frameTime)
                        );

    float* pre        = AArray_GetData
                        (
                            AArray_Get(deformTimeline->vertexArr, frameIndex - 1, Array(float)*),
                            float
                        );

    float* next       = AArray_GetData
                        (
                            AArray_Get(deformTimeline->vertexArr, frameIndex, Array(float)*),
                            float
                        );

    if (mixPercent < 1.0f)
    {
        for (int i = 0; i < positionArr->length; i += 3)
        {
            float p       = pre [i];
            float n       = next[i];

            position[i]  += (p + (n - p) * percent - position[i]) * mixPercent;

            int   i1      = i + 1;
            p             = pre [i1];
            n             = next[i1];

            position[i1] += (p + (n - p) * percent - position[i1]) * mixPercent;
        }

    }
    else
    {
        for (int i = 0; i < positionArr->length; i += 3)
        {
            float p       = pre [i];
            float n       = next[i];

            position[i]   = p + (n - p) * percent;

            int   i1      = i + 1;

            p             = pre [i1];
            n             = next[i1];

            position[i1]  = p + (n - p) * percent;
        }
    }
}


static SkeletonDeformTimeline* CreateDeform(int frameCount)
{
    SkeletonDeformTimeline* deformTimeline     = (SkeletonDeformTimeline*) malloc(sizeof(SkeletonDeformTimeline));

    CurveTimelineInit(deformTimeline->curveTimeline, frameCount);

    deformTimeline->skeletonTimeline->Release  = DeformRelease;
    deformTimeline->skeletonTimeline->Apply    = DeformApply;
    deformTimeline->skeletonTimeline->childPtr = deformTimeline;

    deformTimeline->frameArr                   = AArray->Create(sizeof(float),         frameCount);
    deformTimeline->vertexArr                  = AArray->Create(sizeof(Array(float)*), frameCount);

    return deformTimeline;
}


struct ASkeletonTimeline ASkeletonTimeline[1] =
{
    CreateRotate,
    CreateTranslate,
    CreateScale,
    CreateColor,
    CreateAttachment,
    CreateEvent,
    CreateDrawOrder,
    CreateDeform,

    SetLinear,
    SetStepped,
    SetCurve,

    SetRotateFrame,
    SetTranslateFrame,
    SetColorFrame,
    SetAttachmentFrame,
    SetEventFrame,
    SetDrawOrderFrame,
    SetDeformFrame,
};
