/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2013-1-10
 * Author : scott.cgi
 * Version: 0.1
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

    // Camera position
    0, 0, 0,

    // We are looking toward the distance
    0, 0, 0,

    // up vector control camera direction, perpendicular to the camera plane
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

