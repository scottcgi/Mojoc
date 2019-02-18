/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2016-7-27
 * Update: 2019-1-31
 * Author: scott.cgi
 */


#ifndef FONT_H
#define  FONT_H


#include "Engine/Toolkit/Utils/ArrayIntSet.h"
#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


/**
 * Font implementation by TextureAtlas.
 */
typedef struct
{
    /**
     * the TextureAtlas that Font used.
     */
    TextureAtlas*          textureAtlas;

    /**
     * The Mesh that textureAtlas used.
     */
    Mesh                   mesh[1];

    /**
     * Cache all FontTexts.
     */
    ArrayIntSet(FontText*) fontTextSet[1];

    /**
     * Font Mesh unused SubMesh list.
     * released FontText's SubMesh will add into this.
     */
    ArrayList  (SubMesh*)  unusedSubMeshList[1];
}
Font;


/**
 * FontText alignment type.
 */
typedef enum
{
    FontTextAlignment_HorizontalLeft,
    FontTextAlignment_HorizontalRight,
    FontTextAlignment_VerticalTop,
    FontTextAlignment_VerticalBottom,
}
FontTextAlignment;


/**
 * The text in font textureAtlas.
 */
typedef struct
{
    /**
     * The base class for provide draw functions.
     */
    Drawable            drawable[1];

    /**
     * Default FontTextAlignment_HorizontalLeft.
     */
    FontTextAlignment   alignment;

    /**
     * The spacing between chars, default 0.
     */
    float                charSpacing;

    /**
     * The Font that FontText belongs to.
     */
    Font*               font;

    /**
     * The SubMesh list that FontText used.
     */
    ArrayList(SubMesh*) usedSubMeshList[1];
}
FontText;


/**
 * Control Font and FontText.
 */
struct AFont
{
    /**
     * Get Font by filePath.
     *
     * filePath:
     *     Android: assets
     *     IOS    : NSBundle
     */
    Font*     (*Get)           (const char* filePath);
    FontText* (*GetText)       (Font* font);
    void      (*Draw)          (Font* font);

    void      (*SetString)     (FontText* text, const char* str);
    void      (*SetInt)        (FontText* text, int         num);
    void      (*SetFloat)      (FontText* text, float        num);

    /**
     * Make Font can reuse in Get method,
     * and release all FontTexts.
     */
    void      (*Release)       (Font* font);

    /**
     * Make FontText can reuse in GetText method.
     * and add FontText's SubMesh into Font->unusedSubMeshList.
     */
    void      (*ReleaseText)   (FontText* text);
};


extern struct AFont AFont[1];


#endif
