/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-4-26
 * Author : scott.cgi
 * Version: 0.0.0
 */


#include <stdio.h>

#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"

#include "Tool.h"
#include "GameMap.h"


static void UpdateBox(PhysicsBody* body, Drawable* drawable)
{
    float* vertexData = AArray_GetData(body->vertexArr,   float);
    float* posData    = AArray_GetData(body->positionArr, float);

    for (int i = 0; i < body->vertexArr->length; i += 2)
    {
        Vector2 world[1];

        ADrawable->ConvertBetweenLocal
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
    float* posData = AArray_GetData(body->positionArr, float);

    for (int i = 0; i < body->positionArr->length; i += 2)
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
    ATool->ShakeZ((Drawable*) action->userData->slot0->ptrValue);
}


static void ShakeZ(Drawable* drawable)
{
    ATweenTool->AddRotateZ      (90.0f, 0.8f)
              ->SetRelative     (false)
              ->SetEaseType     (TweenEaseType_SineInOut)

              ->AddRotateZ      (0.0f,  0.8f)
              ->SetRelative     (false)
              ->SetUserData0Ptr (drawable)
              ->SetOnComplete   (ShakeZActionOnComplete)
              ->SetEaseType     (TweenEaseType_SineInOut)

              ->RunActions      (drawable);
}


static void ScaleActionOnComplete(TweenAction* action)
{
    ATool->Scale2((Drawable*) action->userData->slot0->ptrValue);
}


static void Scale2(Drawable* drawable)
{
    ATweenTool->AddScaleSame2  (0.8f, 0.5f, false, TweenEaseType_SineInOut)
              ->AddScaleSame2  (1.0f, 0.5f, false, TweenEaseType_SineInOut)
              ->SetOnComplete  (ScaleActionOnComplete)
              ->SetUserData0Ptr(drawable)
              ->RunActions     (drawable);
}


static void SetTimeToBuff(char buff[], int seconds)
{
    int min = seconds / 60;
    int sec = seconds % 60;

    sprintf
    (
        buff,
        "%02d:%02d",
        min,
        sec
    );
}


struct ATool ATool[1] =
{
    1.0f,
    1.0f,

    UpdateBox,
    InitBox,

    ShakeX,
    ShakeZ,
    Scale2,
    SetTimeToBuff,
};
