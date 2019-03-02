/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-1-10
 * Update: 2019-1-24
 * Author: scott.cgi
 */


#include "Engine/Graphics/OpenGL/Camera.h"


static void SetOrtho()
{
    // near and far is distance of eye position to look up direction
    AMatrix->Ortho
    (
        ACamera->left,
        ACamera->right,
        ACamera->bottom,
        ACamera->top,
        ACamera->near,
        ACamera->far,
        ACamera->projection
    );

    AMatrix->MultiplyMM(ACamera->projection, ACamera->view, ACamera->vp);
}


static void SetFrustum()
{
    // near and far is distance of eye position to look up direction
    AMatrix->Frustum
    (
        ACamera->left,
        ACamera->right,
        ACamera->bottom,
        ACamera->top,
        ACamera->near,
        ACamera->far,
        ACamera->projection
    );

    AMatrix->MultiplyMM(ACamera->projection, ACamera->view, ACamera->vp);
}


static void SetLookAt()
{
    AMatrix->LookAt
    (
        ACamera->eyeX,
        ACamera->eyeY,
        ACamera->eyeZ,
        ACamera->lookX,
        ACamera->lookY,
        ACamera->lookZ,
        ACamera->upX,
        ACamera->upY,
        ACamera->upZ,
        ACamera->view
    );

    AMatrix->MultiplyMM(ACamera->projection, ACamera->view, ACamera->vp);
}


static float GetNearZOrder()
{
    return ACamera->eyeZ - ACamera->near;
}


static float GetFarZOrder()
{
    return GetNearZOrder() - ACamera->far;
}


struct ACamera ACamera[1] =
{
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,

    MATRIX4_IDENTITY,
    MATRIX4_IDENTITY,
    MATRIX4_IDENTITY,

    SetOrtho,
    SetFrustum,
    SetLookAt,
    GetNearZOrder,
    GetFarZOrder,
};

