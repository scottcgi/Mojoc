/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2015-4-26
 * Update   : 2019-2-17
 * Author   : scott.cgi
 */


#include <stdio.h>
#include <string.h>

#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"

#include "Tool.h"
#include "GameMap.h"
#include "HUD.h"


static void UpdateBox(PhysicsBody* body, Drawable* drawable)
{
    float* vertexData = body->vertexArr->data;
    float* posData    = body->transformedVertexArr->data;

    for (int i = 0; i < body->vertexArr->length; i += 2)
    {
        Vector2 world[1];

        ADrawable->ConvertBetweenLocalPositionV2
        (
            drawable,
            (Vector2*) (vertexData + i),
            AGameMap->beforeDrawable,
            world
        );

        posData[i]     = world->x;
        posData[i + 1] = world->y;
    }
}


static void InitBox(PhysicsBody* body, float x, float y)
{
    float* posData = body->transformedVertexArr->data;

    for (int i = 0; i < body->transformedVertexArr->length; i += 2)
    {
        posData[i]     += x;
        posData[i + 1] += y;
    }
}


static void ShakeX(Drawable* drawable)
{
    ATweenTool->AddMoveX   (-0.005f, 0.05f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveX   (0.01f, 0.08f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveX   (-0.008f, 0.12f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveX   (0.005f, 0.18f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveX   (-0.002f, 0.25f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->RunActions (drawable);
}


static void ShakeZActionOnComplete(TweenAction* action)
{
    ATool->ShakeZ(AUserData_GetSlotPtrWithType(action->userData, 0, Drawable*));
}


static void ShakeZ(Drawable* drawable)
{
    ATweenTool->AddRotateZ    (90.0f, 0.8f)
              ->SetRelative   (false)
              ->SetEaseType   (TweenEaseType_SineInOut)

              ->AddRotateZ    (0.0f,  0.8f)
              ->SetRelative   (false)
              ->SetUserDataPtr(0, drawable)
              ->SetOnComplete (ShakeZActionOnComplete)
              ->SetEaseType   (TweenEaseType_SineInOut)

              ->RunActions    (drawable);
}


static void ScaleActionOnComplete(TweenAction* action)
{
    ATool->Scale2(AUserData_GetSlotPtrWithType(action->userData, 0, Drawable*));
}


static void Scale2(Drawable* drawable)
{
    ATweenTool->AddScaleSame2 (0.8f, 0.5f, false, TweenEaseType_SineInOut)
              ->AddScaleSame2 (1.0f, 0.5f, false, TweenEaseType_SineInOut)
              ->SetOnComplete (ScaleActionOnComplete)
              ->SetUserDataPtr(0, drawable)
              ->RunActions    (drawable);
}


static void SetTimeToBuff(char buff[], int seconds)
{
    int min = seconds / 60;
    int sec = seconds % 60;
    sprintf(buff, "%02d:%02d", min, sec);
}


// the number bigger the tail longer
static const float arrowTailPosTime = 0.018f;


static void UpdateArrowTail(Arrow* arrow, float deltaSeconds)
{
    // add tail pos
    if (arrow->tailPosCurTime >= arrowTailPosTime)
    {
        arrow->tailPosCurTime = 0;

        if (arrow->tailPosCurCount == ArrowTail_PosCount)
        {
            // from and to overlap so cannot use memcpy
            memmove(arrow->tailPos + 2, arrow->tailPos, (ArrowTail_PosLen - 2) * sizeof(float));
        }
        else
        {
            // from and to overlap so cannot use memcpy
            memmove
            (
                arrow->tailPos + 2,
                arrow->tailPos,
                (arrow->tailPosCurCount << 1) * sizeof(float)
            );
            ++arrow->tailPosCurCount;
        }

        arrow->tailPos[0] = arrow->body->positionX - arrow->halfWidth * AMath_Cos(arrow->body->rotationZ);
        arrow->tailPos[1] = arrow->body->positionY - arrow->halfWidth * AMath_Sin(arrow->body->rotationZ);
    }

    arrow->tailPosCurTime += deltaSeconds;
}


static void ShakeWorld()
{
    ATweenTool->AddMoveY   (0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->AddMoveY   (-0.01f, 0.2f)
              ->SetEaseType(TweenEaseType_BounceOut)

              ->RunActions (AHero_GetDrawable());


    ATweenTool->AddMoveY   (-0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->AddMoveY   (0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_SineInOut)
              ->RunActions (AGameMap->beforeDrawable);


    ATweenTool->AddInterval(0.04f)

              ->AddMoveY   (-0.01f, 0.15f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->AddMoveY   (0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->RunActions (AGameMap->middleDrawable);


    ATweenTool->AddInterval(0.08f)

              ->AddMoveY   (-0.01f, 0.2f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->AddMoveY   (0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_BounceOut)

              ->RunActions (AGameMap->backDrawable);


    ATweenTool->AddInterval(0.1f)

              ->AddMoveY   (-0.01f, 0.25f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->AddMoveY   (0.01f, 0.1f)
              ->SetEaseType(TweenEaseType_SineInOut)

              ->RunActions (AGameMap->skyDrawable);


    ATweenTool->AddMoveY   (0.0015f, 0.2f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveY   (-0.0015f, 0.2f)
              ->SetEaseType(TweenEaseType_SineIn)

              ->RunActions (AHUD->upDrawable);


    ATweenTool->AddMoveY   (-0.002f, 0.2f)
              ->SetEaseType(TweenEaseType_SineOut)

              ->AddMoveY   (0.002f, 0.2f)
              ->SetEaseType(TweenEaseType_SineIn)

              ->RunActions (AHUD->downDrawable);
}


static void TalkTextTweenActionOnComplete(TweenAction* tweenAction)
{
    AFont->ReleaseText(AUserData_GetSlotPtrWithType(tweenAction->userData, 0, FontText*));
}


static void FlyTextOnHero(FontText* text, float y, float duration)
{
    ADrawable_SetParent(text->drawable, AGameMap->beforeDrawable);

    ADrawable_SetPosition2
    (
        text->drawable,
        ADrawable->ConvertBetweenLocalPositionX
        (
            AGameMap->groundPosDrawable,
            AHero_GetDrawable()->positionX - text->drawable->width / 2,
            AGameMap->beforeDrawable
        ),
        AHero_GetDrawable()->positionY + ASkeletonAnimationPlayer_GetHeight(AHero->player)
    );

    ATweenTool->AddAction     ()
              ->SetFadeTo     (0.0f)
              ->SetRelative   (false)
              ->SetEaseType   (TweenEaseType_ExponentialOut)
              ->SetMoveY      (y)
              ->SetEaseType   (TweenEaseType_ExponentialOut)
              ->SetDuration   (duration)
              ->SetOnComplete (TalkTextTweenActionOnComplete)
              ->SetUserDataPtr(0, text)
              ->RunActions    (text->drawable);
}


struct ATool ATool[1] =
{{
    1.0f,
    1.0f,
    LanguageCode_en,

    UpdateBox,
    InitBox,

    ShakeX,
    ShakeZ,
    Scale2,
    ShakeWorld,
    
    SetTimeToBuff,
    UpdateArrowTail,
    FlyTextOnHero,
}};
