/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-1-10
 * Update   : 2019-1-24
 * Author   : scott.cgi
 */


#ifndef CAMERA_H
#define CAMERA_H


#include "Engine/Toolkit/Math/Matrix.h"
#include "Engine/Toolkit/Math/Math.h"


/**
 * Control main camera.
 */
struct ACamera
{
    float left;
    float right;
    float bottom;

    float top;
    float near;
    float far;

    /**
     * Camera position.
     */
    float eyeX;
    float eyeY;
    float eyeZ;

    /**
     * We are looking toward the distance.
     */
    float lookX;
    float lookY;
    float lookZ;

    /**
     * The up vector control camera direction, perpendicular to the camera plane.
     */
    float upX;
    float upY;
    float upZ;
    
    /**
     * Store view projection matrix.
     */
    Matrix4 vp        [1];

    /**
     * Store projection matrix.
     */
    Matrix4 projection[1];

    /**
     * Store the view matrix.
     */
    Matrix4  view     [1];


    void  (*SetOrtho)    (void);
    void  (*SetFrustum)  (void);
    void  (*SetLookAt)   (void);

    /**
     * The ZOrder most near camera.
     */
    float (*GetNearZOrder)(void);

    /**
     * The ZOrder most far camera.
     */
    float (*GetFarZOrder) (void);
};


extern struct ACamera ACamera[1];


#endif
