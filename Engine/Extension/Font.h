/*
 * Font.h
 *
 *  Created on: 2016-7-27
 *      Author: scott.cgi
 */

#ifndef font_h
#define font_h

#include "Engine/Toolkit/Utils/ArrayIntMap.h"
#include "Engine/Extension/TextureAtlas.h"
#include "Engine/Graphics/OpenGL/Mesh.h"


typedef struct
{
	TextureAtlas*                       get_only textureAtlas;
	Mesh                                get_only mesh             [1];
    ArrayIntMap(fontTextPtr, FontText*) get_only fontTextMap      [1];
	ArrayList  (SubMesh*)               get_only unusedSubMeshList[1];
}
Font;


typedef enum
{
    font_text_alignment_horizontal_left,
    font_text_alignment_horizontal_right,
    font_text_alignment_vertical_top,
    font_text_alignment_vertical_bottom,
}
FontTextAlignment;


typedef struct
{
	Font*               get_only font;

    /**
     * Default font_text_alignment_horizontal_left
     */
    FontTextAlignment            alignment;

	/**
	 * Default 0
	 */
    float                        charSpacing;

	Drawable                     drawable       [1];
	ArrayList(SubMesh*) get_only usedSubMeshList[1];
}
FontText;


struct AFont
{
	Font*     (*Get)           (const char* filePath);
    FontText* (*GetText)       (Font* font);
	void      (*Draw)          (Font* font);

	void      (*SetString)     (FontText* text, const char* str);
	void      (*SetInt)        (FontText* text, int   num);
	void      (*SetFloat)      (FontText* text, float num);

    /**
     * Make Font can reuse in Get
     * and release all FontText contained
     */
    void      (*Reuse)         (Font* font);

	/**
	 * make FontText can reuse in GetText
	 */
	void      (*ReuseText)     (FontText* text);
};


extern struct AFont AFont[1];


#endif
