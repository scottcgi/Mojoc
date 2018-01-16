/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-7-27
 * Author: scott.cgi
 */

#ifndef FONT_H
#define FONT_H


#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


typedef struct
{
    /**
     * Font used TextureAtlas
     */
    TextureAtlas*                       textureAtlas;

    /**
     * Font TextureAtlas used Mesh
     */
    Mesh                                mesh             [1];

    /**
     * Cahce FontText
     */
    ArrayIntSet(FontText*)              fontTextSet      [1];

    /**
     * Font Mesh unused SubMesh list
     */
    ArrayList  (SubMesh*)               unusedSubMeshList[1];
}
Font;


typedef enum
{
    FontTextAlignment_HorizontalLeft,
    FontTextAlignment_HorizontalRight,
    FontTextAlignment_VerticalTop,
    FontTextAlignment_VerticalBottom,
}
FontTextAlignment;


typedef struct
{
    Drawable             drawable       [1];

    /**
     * Default FontTextAlignment_HorizontalLeft
     */
    FontTextAlignment    alignment;

    /**
     * Default 0
     */
    float                charSpacing;

    /**
     * FontText belongs Font
     */
    Font*                font;

    /**
     * FontText unused SubMesh list
     */
    ArrayList(SubMesh*)  usedSubMeshList[1];
}
FontText;


struct AFont
{
    Font*     (*Get)           (char* filePath);
    FontText* (*GetText)       (Font* font);
    void      (*Draw)          (Font* font);

    void      (*SetString)     (FontText* text, char* str);
    void      (*SetInt)        (FontText* text, int   num);
    void      (*SetFloat)      (FontText* text, float num);

    /**
     * Make Font can reuse in Get method
     * and release all FontText contained
     */
    void      (*Reuse)         (Font* font);

    /**
     * Make FontText can reuse in GetText method
     */
    void      (*ReuseText)     (FontText* text);
};


extern struct AFont AFont[1];


#endif
