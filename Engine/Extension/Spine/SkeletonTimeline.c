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
 * Since : 2013-7-3
 * Update: 2019-2-2
 * Author: scott.cgi
 */


#include <stdbool.h>
#include <string.h>

#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Extension/Spine/SkeletonTimeline.h"
#include "Engine/Toolkit/Platform/Log.h"


/**
 * The target After the first and before the last entry.
 */
static inline int BinarySearchByStep(const float values[], int valuesLength, float target, int step)
{
    int low  = 0;
    int high = valuesLength / step - 2;

    if (high <= 0)
    {
        return step;
    }

    int current = (unsigned int) high >> 1; // NOLINT(hicpp-signed-bitwise)

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

        current = (unsigned int) (low + high) >> 1; // NOLINT(hicpp-signed-bitwise)
    }
}


/**
 * The target After the first and before the last entry.
 */
static inline int BinarySearch(const float values[], int valuesLength, float target)
{
    int low  = 0;
    int high = valuesLength - 2;

    if (high <= 0)
    {
        return 1;
    }

    int current = (unsigned int) high >> 1; // NOLINT(hicpp-signed-bitwise)

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

        current = (unsigned int) (low + high) >> 1; // NOLINT(hicpp-signed-bitwise)
    }
}


enum
{
    Bezier_Segments = 12,
    Bezier_Size     = ((Bezier_Segments + 1) << 1) - 1, // NOLINT(hicpp-signed-bitwise)
};


static inline void SetLinear(SkeletonCurveTimeline* curveTimeline, int frameIndex)
{
    AArray_Set(curveTimeline->curveArr, frameIndex * Bezier_Size, SkeletonCurveType_Linear, float);
}


static inline void SetStepped(SkeletonCurveTimeline* curveTimeline, int frameIndex)
{
    AArray_Set(curveTimeline->curveArr, frameIndex * Bezier_Size, SkeletonCurveType_Stepped, float);
}


static const float subDivStep  = 1.0f        / Bezier_Segments;
static const float subDivStep2 = subDivStep  * subDivStep;
static const float subDivStep3 = subDivStep2 * subDivStep;
static const float pre1        = 3           * subDivStep;
static const float pre2        = 3           * subDivStep2;
static const float pre4        = 6           * subDivStep2;
static const float pre5        = 6           * subDivStep3;
static const float subDivPre   = subDivStep3 / pre5;


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

    float* curves = curveTimeline->curveArr->data;
    int    i      = frameIndex * Bezier_Size;
    curves[i++]   = SkeletonCurveType_Bezier;

    float  x      = dfx;
    float  y      = dfy;

    for (int n = i + Bezier_Size - 1; i < n; i += 2)
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
    int    i      = frameIndex * Bezier_Size;
    float* curves = curveTimeline->curveArr->data;
    float  dfx    = curves[i];

    switch ((int) dfx)
    {
        case SkeletonCurveType_Linear:
            return AMath_Clamp(percent, 0.0f, 1.0f);

        case SkeletonCurveType_Bezier:
        {
            ++i;
            percent = AMath_Clamp(percent, 0.0f, 1.0f);
            float x  = 0.0f;

            for (int start = i, n = i + Bezier_Size - 1; i < n; i += 2)
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
        default:
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
    curveTimeline->curveArr = AArray->Create(sizeof(float), frameCount * Bezier_Size);
}


//----------------------------------------------------------------------------------------------------------------------


enum 
{
    RotateFrame_Value = 1,
    RotateFrame_Times = 2,
};


static void SetRotateFrame(SkeletonRotateTimeline* rotateTimeline, int frameIndex, float time, float angle)
{
    float* frames          = rotateTimeline->frameArr->data;
    frameIndex            *= RotateFrame_Times;
    frames[frameIndex]     = time;
    frames[frameIndex + 1] = angle;
}


static void RotateApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonRotateTimeline* rotateTimeline = skeletonTimeline->childPtr;

    float* frames      = rotateTimeline->frameArr->data;
    int    frameLength = rotateTimeline->frameArr->length;

    if (time < frames[0])
    {
        // time is before first frame
        return;
    }

    SkeletonBone*     bone     = AArray_GetPtr(skeleton->boneArr, rotateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData = bone->boneData;
    Drawable*         drawable = bone->drawable;

    if (time >= frames[frameLength - RotateFrame_Times])
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
    int frameIndex       = BinarySearchByStep(frames, frameLength, time, RotateFrame_Times);

    float lastFrameValue = frames[frameIndex - 1];
    float frameTime      = frames[frameIndex];
    float percent        = GetCurvePercent
                          (
                              rotateTimeline->curveTimeline, (frameIndex >> 1) - 1,  // NOLINT(hicpp-signed-bitwise)
                              1.0f - (time - frameTime) / (frames[frameIndex - RotateFrame_Times] - frameTime)
                          );
    float amount         = frames[frameIndex + RotateFrame_Value] - lastFrameValue;

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
    SkeletonRotateTimeline* rotateTimeline = skeletonTimeline->childPtr;

    CurveRelease(rotateTimeline->curveTimeline);
    free(rotateTimeline->frameArr);
    rotateTimeline->frameArr = NULL;
}


static SkeletonRotateTimeline* CreateRotate(int frameCount, int boneIndex)
{
    SkeletonRotateTimeline* rotateTimeline     = malloc(sizeof(SkeletonRotateTimeline));

    CurveTimelineInit(rotateTimeline->curveTimeline, frameCount);

    rotateTimeline->skeletonTimeline->Apply    = RotateApply;
    rotateTimeline->skeletonTimeline->Release  = RotateRelease;
    rotateTimeline->skeletonTimeline->childPtr = rotateTimeline;

    rotateTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * RotateFrame_Times);
    rotateTimeline->boneIndex                  = boneIndex;

    return rotateTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


enum
{
    TranslateFrame_X     = 1,
    TranslateFrame_Y     = 2,
    TranslateFrame_Times = 3,
};


static void SetTranslateFrame(SkeletonTranslateTimeline* translateTimeline, int frameIndex, float time, float x, float y)
{
    float* frames          = translateTimeline->frameArr->data;
    frameIndex            *= TranslateFrame_Times;
    frames[frameIndex]     = time;
    frames[frameIndex + 1] = x;
    frames[frameIndex + 2] = y;
}


static void TranslateApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonTranslateTimeline* translateTimeline = skeletonTimeline->childPtr;
    float*                     frames            = translateTimeline->frameArr->data;

    if (time < frames[0])
    {
        return; // time is before first frame
    }

    int               frameLength = translateTimeline->frameArr->length;
    SkeletonBone*     bone        = AArray_GetPtr(skeleton->boneArr, translateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData    = bone->boneData;
    Drawable*         drawable    = bone->drawable;

    if (time >= frames[frameLength - TranslateFrame_Times])
    {
        // time is after last frame
        // each frame value is relative boneData x or y
        ADrawable_SetPosition2
        (
            drawable,
            drawable->positionX +
            (boneData->x + frames[frameLength - TranslateFrame_Y] - drawable->positionX) * mixPercent,
            drawable->positionY +
            (boneData->y + frames[frameLength - TranslateFrame_X] - drawable->positionY) * mixPercent
        );

        return;
    }

    // interpolate between the last frame and the current frame.
    int   frameIndex  = BinarySearchByStep(frames, frameLength, time, TranslateFrame_Times);
    float lastFrameX  = frames[frameIndex - TranslateFrame_Y];
    float lastFrameY  = frames[frameIndex - TranslateFrame_X];
    float frameTime   = frames[frameIndex];
    float percent     = GetCurvePercent
                        (
                            translateTimeline->curveTimeline, frameIndex / TranslateFrame_Times - 1,
                            1.0f - (time - frameTime) / (frames[frameIndex - TranslateFrame_Times] - frameTime)
                        );

    ADrawable_SetPosition2
    (
        drawable,
        
        drawable->positionX +
        (
            boneData->x + lastFrameX +
            (frames[frameIndex + TranslateFrame_X] - lastFrameX) * percent - drawable->positionX
        )
        * mixPercent,

        drawable->positionY +
        (
            boneData->y + lastFrameY +
            (frames[frameIndex + TranslateFrame_Y] - lastFrameY) * percent - drawable->positionY
        )
        * mixPercent
    );
}


static void TranslateRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonTranslateTimeline* translateTimeline = skeletonTimeline->childPtr;

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

    translateTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * TranslateFrame_Times);
    translateTimeline->boneIndex                  = boneIndex;
}


static SkeletonTranslateTimeline* CreateTranslate(int frameCount, int boneIndex)
{
    SkeletonTranslateTimeline* translateTimeline = malloc(sizeof(SkeletonTranslateTimeline));
    TranslateTimelineInit(translateTimeline, frameCount, boneIndex);

    return translateTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void ScaleApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonScaleTimeline*     scaleTimeline     = skeletonTimeline->childPtr;
    SkeletonTranslateTimeline* translateTimeline = scaleTimeline->translateTimeline;

    float* frames = translateTimeline->frameArr->data;

    if (time < frames[0])
    {
        return; // time is before first frame
    }

    int               frameLength = translateTimeline->frameArr->length;
    SkeletonBone*     bone        = AArray_GetPtr(skeleton->boneArr, translateTimeline->boneIndex, SkeletonBone);
    SkeletonBoneData* boneData    = bone->boneData;
    Drawable*         drawable    = bone->drawable;

    if (time >= frames[frameLength - TranslateFrame_Times])
    {
        // time is after last frame
        // each frame scale value is relative 1.0 not boneData scale
        // so we use boneData scale - 1.0f
        ADrawable_SetScale2
        (
            drawable,

            drawable->scaleX +
            (boneData->scaleX - 1.0f + frames[frameLength - TranslateFrame_Y] - drawable->scaleX) * mixPercent,

            drawable->scaleY +
            (boneData->scaleY - 1.0f + frames[frameLength - TranslateFrame_X] - drawable->scaleY) * mixPercent
        );

        return;
    }

    // interpolate between the last frame and the current frame.
    int   frameIndex = BinarySearchByStep(frames, frameLength, time, TranslateFrame_Times);
    float lastFrameX = frames[frameIndex - TranslateFrame_Y];
    float lastFrameY = frames[frameIndex - TranslateFrame_X];
    float frameTime  = frames[frameIndex];

    float percent    = 1.0f - (time - frameTime) / (frames[frameIndex - TranslateFrame_Times] - frameTime);
    percent          = GetCurvePercent(translateTimeline->curveTimeline, frameIndex / TranslateFrame_Times - 1, percent);

    ADrawable_SetScale2
    (
        drawable,

        drawable->scaleX +
        (
            boneData->scaleX - 1.0f + lastFrameX +
            (frames[frameIndex + TranslateFrame_X] - lastFrameX) * percent - drawable->scaleX
        )
        * mixPercent,

        drawable->scaleY +
        (
            boneData->scaleY - 1.0f + lastFrameY +
            (frames[frameIndex + TranslateFrame_Y] - lastFrameY) * percent - drawable->scaleY
        )
        * mixPercent
    );
}


static void ScaleRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonScaleTimeline* scaleTimeline = skeletonTimeline->childPtr;
    skeletonTimeline->childPtr           = scaleTimeline->translateTimeline;
    TranslateRelease(scaleTimeline->translateTimeline->skeletonTimeline);
}


static SkeletonScaleTimeline* CreateScale(int frameCount, int boneIndex)
{
    SkeletonScaleTimeline* scaleTimeline = malloc(sizeof(SkeletonScaleTimeline));
    TranslateTimelineInit(scaleTimeline->translateTimeline, frameCount, boneIndex);

    scaleTimeline->translateTimeline->skeletonTimeline->Apply    = ScaleApply;
    scaleTimeline->translateTimeline->skeletonTimeline->Release  = ScaleRelease;
    scaleTimeline->translateTimeline->skeletonTimeline->childPtr = scaleTimeline;

    return scaleTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


enum
{
    ColorFrame_R     = 1,
    ColorFrame_G     = 2,
    ColorFrame_B     = 3,
    ColorFrame_A     = 4,
    ColorFrame_Times = 5,
};


static void SetColorFrame(SkeletonColorTimeline* colorTimeline, int frameIndex, float time,  Color* color)
{
    float* frames                     = colorTimeline->frameArr->data;
    frameIndex                       *= ColorFrame_Times;
    frames[frameIndex]                = time;
    frames[frameIndex + ColorFrame_R] = color->r;
    frames[frameIndex + ColorFrame_G] = color->g;
    frames[frameIndex + ColorFrame_B] = color->b;
    frames[frameIndex + ColorFrame_A] = color->a;
}


static void ColorApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonColorTimeline* colorTimeline = skeletonTimeline->childPtr;
    float*                 frames        = colorTimeline->frameArr->data;

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

    if (time >= frames[frameLength - ColorFrame_Times])
    {
        // time is after last frame.
        color->r = frames[frameLength - ColorFrame_A];
        color->g = frames[frameLength - ColorFrame_B];
        color->b = frames[frameLength - ColorFrame_G];
        color->a = frames[frameLength - ColorFrame_R];
    }
    else
    {
        // interpolate between the last frame and the current frame.
        int   frameIndex = BinarySearchByStep(frames, frameLength, time, ColorFrame_Times);
        float frameTime  = frames[frameIndex];
        float r          = frames[frameIndex - ColorFrame_A];
        float g          = frames[frameIndex - ColorFrame_B];
        float b          = frames[frameIndex - ColorFrame_G];
        float a          = frames[frameIndex - ColorFrame_R];

        float percent    = GetCurvePercent
                           (
                               colorTimeline->curveTimeline,
                               frameIndex / ColorFrame_Times - 1,
                               1.0f - (time - frameTime) / (frames[frameIndex - ColorFrame_Times] - frameTime)
                           );

        r               += (frames[frameIndex + ColorFrame_R] - r) * percent;
        g               += (frames[frameIndex + ColorFrame_G] - g) * percent;
        b               += (frames[frameIndex + ColorFrame_B] - b) * percent;
        a               += (frames[frameIndex + ColorFrame_A] - a) * percent;

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
    SkeletonColorTimeline* colorTimeline = skeletonTimeline->childPtr;

    free(colorTimeline->frameArr);
    colorTimeline->frameArr = NULL;

    CurveRelease(colorTimeline->curveTimeline);
}


static SkeletonColorTimeline* CreateColor(int frameCount)
{
    SkeletonColorTimeline* colorTimeline      = malloc(sizeof(SkeletonColorTimeline));
    CurveTimelineInit(colorTimeline->curveTimeline, frameCount);

    colorTimeline->skeletonTimeline->Apply    = ColorApply;
    colorTimeline->skeletonTimeline->Release  = ColorRelease;
    colorTimeline->skeletonTimeline->childPtr = colorTimeline;
    colorTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount * ColorFrame_Times);

    return colorTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetAttachmentFrame
(
    SkeletonAttachmentTimeline* attachmentTimeline,
    int                         frameIndex,
    float                       time,
    const char*                 attachmentName
)
{
    char* copyName = malloc(strlen(attachmentName) + 1);
    strcpy(copyName, attachmentName);

    AArray_Set(attachmentTimeline->frameArr,          frameIndex, time,     float);
    AArray_Set(attachmentTimeline->attachmentNameArr, frameIndex, copyName, const char*);
}


static void AttachmentApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonAttachmentTimeline* attachmentTimeline = skeletonTimeline->childPtr;
    float*                      frames             = attachmentTimeline->frameArr->data;

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

    SkeletonSlot*           slot           = AArray_GetPtr
                                             (
                                                 skeleton->slotArr,
                                                 attachmentTimeline->slotIndex,
                                                 SkeletonSlot
                                             );
    const char*             attachmentName = AArray_Get
                                             (
                                                 attachmentTimeline->attachmentNameArr,
                                                 frameIndex,
                                                 const char*
                                             );
    SkeletonAttachmentData* attachmentData = ASkeleton->GetAttachmentData
                                             (
                                                 slot->skeleton,
                                                 slot->slotData->name,
                                                 attachmentName
                                             );

    ASkeletonSlot->SetAttachmentData(slot, attachmentData);
}


static void AttachmentRelease(SkeletonTimeline* skeletonTimeline )
{
    SkeletonAttachmentTimeline* attachmentTimeline = skeletonTimeline->childPtr;

    for (int i = 0; i < attachmentTimeline->attachmentNameArr->length; ++i)
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
    SkeletonAttachmentTimeline* attachmentTimeline = malloc(sizeof(SkeletonAttachmentTimeline));

    attachmentTimeline->skeletonTimeline->Apply    = AttachmentApply;
    attachmentTimeline->skeletonTimeline->Release  = AttachmentRelease;
    attachmentTimeline->skeletonTimeline->childPtr = attachmentTimeline;

    attachmentTimeline->frameArr                   = AArray->Create(sizeof(float),  frameCount);
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

    SkeletonEventTimeline* eventTimeline = skeletonTimeline->childPtr;
    float*                 frames        = eventTimeline->frameArr->data;

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
    SkeletonEventTimeline* eventTimeline = skeletonTimeline->childPtr;

    free(eventTimeline->frameArr);
    eventTimeline->frameArr = NULL;

    for (int i = 0; i < eventTimeline->eventArr->length; ++i)
    {
        free(AArray_Get(eventTimeline->eventArr, i, SkeletonEventData*));
    }

    free(eventTimeline->eventArr);
    eventTimeline->eventArr = NULL;
}


static SkeletonEventTimeline* CreateEvent(int frameCount)
{
    SkeletonEventTimeline* eventTimeline      = malloc(sizeof(SkeletonEventTimeline));

    eventTimeline->skeletonTimeline->Apply    = EventApply;
    eventTimeline->skeletonTimeline->Release  = EventRelease;
    eventTimeline->skeletonTimeline->childPtr = eventTimeline;

    eventTimeline->frameArr                   = AArray->Create(sizeof(float), frameCount);
    eventTimeline->eventArr                   = AArray->Create(sizeof(SkeletonEventData*), frameCount);
    eventTimeline->preFrameIndex              = -1;

    return eventTimeline;
}


//----------------------------------------------------------------------------------------------------------------------


static void SetDrawOrderFrame
(
    SkeletonDrawOrderTimeline*  drawOrderTimeline,
    int                         frameIndex,
    float                       time,
    Array(int)*                 drawOrder
)
{
    AArray_Set(drawOrderTimeline->frameArr,     frameIndex, time,      float);
    AArray_Set(drawOrderTimeline->drawOrderArr, frameIndex, drawOrder, Array(int)*);
}


static void DrawOrderRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline = skeletonTimeline->childPtr;

    free(drawOrderTimeline->frameArr);
    drawOrderTimeline->frameArr = NULL;

    for (int i = 0; i < drawOrderTimeline->drawOrderArr->length; ++i)
    {
        free(AArray_Get(drawOrderTimeline->drawOrderArr, i, Array(int)*));
    }

    free(drawOrderTimeline->drawOrderArr);
    drawOrderTimeline->drawOrderArr = NULL;
}


static void DrawOrderApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline = skeletonTimeline->childPtr;

    float* frames = drawOrderTimeline->frameArr->data;

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
    SkeletonSlot*  slots        = skeleton->slotArr->data;
    SkeletonSlot** slotOrders   = skeleton->slotOrderArr->data;
    int*           drawOrder    = NULL;


    if (drawOrderArr != NULL)
    {
        drawOrder = drawOrderArr->data;
    }

    for (int i = 0; i < skeleton->slotArr->length; ++i)
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

            for(int j = 0; j < attachmentDataList->size; ++j)
            {
                SubMesh* subMesh = ASkeleton->GetAttachmentSubMesh
                                   (
                                       skeleton,
                                       AArrayList_Get(attachmentDataList, j, SkeletonAttachmentData*)
                                   );

                for (int k = 0; k < meshList->size; ++k)
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

    for (int i = 0; i < meshList->size; ++i)
    {
        AMesh->ReorderAllChildren(AArrayList_GetPtr(meshList, i, Mesh));
    }
}


static SkeletonDrawOrderTimeline* CreateDrawOrder(int frameCount)
{
    SkeletonDrawOrderTimeline* drawOrderTimeline  = malloc(sizeof(SkeletonDrawOrderTimeline));

    drawOrderTimeline->skeletonTimeline->Release  = DrawOrderRelease;
    drawOrderTimeline->skeletonTimeline->Apply    = DrawOrderApply;
    drawOrderTimeline->skeletonTimeline->childPtr = drawOrderTimeline;

    drawOrderTimeline->frameArr                   = AArray->Create(sizeof(float),        frameCount);
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


static void DeformRelease(SkeletonTimeline* skeletonTimeline)
{
    SkeletonDeformTimeline* deformTimeline = skeletonTimeline->childPtr;

    free(deformTimeline->frameArr);
    deformTimeline->frameArr = NULL;

    for (int i = 0; i < deformTimeline->vertexArr->length; ++i)
    {
        free(AArray_Get(deformTimeline->vertexArr, i, Array(float)*));
    }

    free(deformTimeline->vertexArr);
    deformTimeline->vertexArr = NULL;

    CurveRelease(deformTimeline->curveTimeline);
}


static void DeformApply(SkeletonTimeline* skeletonTimeline, Skeleton* skeleton, float time, float mixPercent)
{
    SkeletonDeformTimeline* deformTimeline = skeletonTimeline->childPtr;
    SkeletonSlot*           slot           = AArray_GetPtr(skeleton->slotArr, deformTimeline->slotIndex, SkeletonSlot);

    if (slot->attachmentData != deformTimeline->attachmentData)
    {
        return;
    }

    float* frames = deformTimeline->frameArr->data;

    // time is before first frame
    if (time < frames[0])
    {
        return;
    }

    Array(float*)* positionArr = NULL;
    float*         positions   = NULL;

    switch (deformTimeline->attachmentData->type)
    {
        case SkeletonAttachmentDataType_Region:
        case SkeletonAttachmentDataType_BoundingBox:
            ALog_A
            (
                false,
                "ASkeletonTimeline DeformApply deformTimeline->attachmentData->type only can be"
                "SkeletonAttachmentDataType_Mesh or SkeletonAttachmentDataType_SkinnedMesh"
            );
            return;

        case SkeletonAttachmentDataType_Mesh:
        {
            SkeletonMeshAttachmentData* meshAttachment = deformTimeline->attachmentData->childPtr;
            SubMesh*                    subMesh        = AArrayList_Get
                                                         (
                                                             AArrayList_GetPtr
                                                             (
                                                                 skeleton->meshList,
                                                                 meshAttachment->meshIndex,
                                                                 Mesh
                                                             )
                                                             ->childList,
                                                             
                                                             meshAttachment->subMeshIndex,
                                                             SubMesh*
                                                         );

            positionArr = subMesh->positionArr;
            positions   = positionArr->data;

            break;
        }

        case SkeletonAttachmentDataType_SkinnedMesh:
        {
            SkeletonSkinnedMeshAttachmentData* skinnedMeshAttachment = deformTimeline->attachmentData->childPtr;

            positionArr = skinnedMeshAttachment->weightVertexArr;
            positions   = positionArr->data;

            break;
        }
    }

    int frameLength = deformTimeline->frameArr->length;

    if (time >= frames[frameLength - 1])
    {
        // time is after last frame.
        float* lastVertex = AArray_Get(deformTimeline->vertexArr, frameLength - 1, Array(float)*)->data;

        if (mixPercent < 1.0f)
        {
            for (int i = 0; i < positionArr->length; i += 3)
            {
                positions[i]  += (lastVertex[i]  - positions[i])  * mixPercent;
                int       i1   = i + 1;
                positions[i1] += (lastVertex[i1] - positions[i1]) * mixPercent;
            }
        }
        else
        {
            memcpy(positions, lastVertex, positionArr->length * sizeof(float));
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

    float* pre        = AArray_Get(deformTimeline->vertexArr, frameIndex - 1, Array(float)*)->data;
    float* next       = AArray_Get(deformTimeline->vertexArr, frameIndex,     Array(float)*)->data;

    if (mixPercent < 1.0f)
    {
        for (int i = 0; i < positionArr->length; i += 3)
        {
            float p        = pre [i];
            float n        = next[i];

            positions[i]  += (p + (n - p) * percent - positions[i]) * mixPercent;

            int   i1       = i + 1;
            p              = pre [i1];
            n              = next[i1];

            positions[i1] += (p + (n - p) * percent - positions[i1]) * mixPercent;
        }

    }
    else
    {
        for (int i = 0; i < positionArr->length; i += 3)
        {
            float p       = pre [i];
            float n       = next[i];

            positions[i]  = p + (n - p) * percent;

            int  i1       = i + 1;
            p             = pre [i1];
            n             = next[i1];

            positions[i1] = p + (n - p) * percent;
        }
    }
}


static SkeletonDeformTimeline* CreateDeform(int frameCount)
{
    SkeletonDeformTimeline* deformTimeline     = malloc(sizeof(SkeletonDeformTimeline));

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
