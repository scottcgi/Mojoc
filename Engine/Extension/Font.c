/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since : 2016-7-27
 * Author: scott.cgi
 */

#include <stdio.h>
#include <string.h>
#include <Engine/Toolkit/Utils/ArrayIntSet.h>

#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Extension/Font.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"


static ArrayList(Font*)     fontList[1] = AArrayListInit(sizeof(Font*),     5);
static ArrayList(FontText*) textList[1] = AArrayListInit(sizeof(FontText*), 30);


static Font* Get(char* filePath)
{
    TextureAtlas* textureAtlas = ATextureAtlas->Get(filePath);

    ALog_A
    (
        textureAtlas->textureList->size == 1,
        "Font not support TextureAtlas has multiple texture"
    );

	Font* font = AArrayListPop(fontList, Font*);

    if (font == NULL)
    {
        font = (Font*) malloc(sizeof(Font));

        AMesh->InitWithCapacity
        (
            AArrayListGet(textureAtlas->textureList, 0, Texture*),
            50,
            font->mesh
        );

        AArrayIntSet->Init(font->fontTextSet);
		AArrayList  ->Init(sizeof(SubMesh*),  font->unusedSubMeshList);
    }
    else
    {
        AArrayIntSet->Clear(font->fontTextSet);
        AArrayList  ->Clear(font->unusedSubMeshList);
        AMesh       ->Clear(font->mesh);
    }

    font->textureAtlas = textureAtlas;

	return font;
}


static FontText* GetText(Font* font)
{
    FontText* text = AArrayListPop(textList, FontText*);

    if (text == NULL)
    {
        text = (FontText*) malloc(sizeof(FontText));
        AArrayList->Init(sizeof(SubMesh*), text->usedSubMeshList);
        text->usedSubMeshList->increase = 10;
    }
    else
    {
        AArrayList->Clear(text->usedSubMeshList);
    }

    // reset drawable
    ADrawable->Init(text->drawable);

    text->alignment   = font_text_alignment_horizontal_left;
    text->charSpacing = 0.0f;
    text->font        = font;
    AArrayIntSet->Add(font->fontTextSet, (intptr_t) text);

    return text;
}


static void Draw(Font* font)
{
	for (int i = 0; i < font->fontTextSet->elementList->size; i++)
	{
        ADrawable->Draw
        (
            AArrayListGet(font->fontTextSet->elementList, i, FontText*)->drawable
        );
	}

	AMeshDraw(font->mesh);
}


static inline TextureAtlasQuad* GetAtlasQuad(FontText* text, char* str, int index)
{
    TextureAtlasQuad* atlasQuad = ATextureAtlasGetQuad
                                  (
                                    text->font->textureAtlas,
                                    (char[]) {str[index], '\0'}
                                  );

    ALog_A(atlasQuad != NULL, "Font SetString not found char = %c in TextureAtlas quads", str[index]);

    return atlasQuad;
}


static inline void SetNewChar(FontText* text, char* str, int len)
{
    // set text new char
    for (int i = 0; i < len; i++)
    {
        TextureAtlasQuad* atlasQuad = GetAtlasQuad(text, str, i);
        SubMesh*          subMesh   = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

        ASubMesh->SetWithQuad
        (
            subMesh,
            text->font->mesh->texture,
            atlasQuad->quad
        );
    }
}


static void SetString(FontText* text, char* str)
{
	ArrayList* children    = text->font->mesh->childList;
	int        len         = (int) strlen(str);

	text->drawable->height = 0.0f;
	text->drawable->width  = 0.0f;

    if (len == text->usedSubMeshList->size)
    {
        SetNewChar(text, str, len);
    }
    else if (len > text->usedSubMeshList->size)
    {
        SetNewChar(text, str, text->usedSubMeshList->size);

        int originalSize = children->size;

        // add chars more than text has
        for (int i = text->usedSubMeshList->size; i < len; i++)
        {
            TextureAtlasQuad* atlasQuad = GetAtlasQuad(text, str, i);
            SubMesh*          subMesh;

            if (text->font->unusedSubMeshList->size > 0)
            {
                subMesh = AArrayListPop(text->font->unusedSubMeshList, SubMesh*);
                ADrawableSetVisible(subMesh->drawable);

                ASubMesh->SetWithQuad
                (
                    subMesh,
                    text->font->mesh->texture,
                    atlasQuad->quad
                );

                if (subMesh->drawable->parent != text->drawable)
                {
                    ADrawableSetParent(subMesh->drawable, text->drawable);
                }
            }
            else
            {
                // not enough SubMesh for text char so add SubMesh
                subMesh = AMesh->AddChildWithQuad(text->font->mesh, atlasQuad->quad);
                ADrawableSetParent(subMesh->drawable, text->drawable);
            }

            AArrayListAdd(text->usedSubMeshList, subMesh);
        }

        if (originalSize != children->size)
        {
            // if regenerate the SubMesh's drawable parent must visible
            // or parent property will lost
            AMesh->GenerateBuffer(text->font->mesh);
        }
    }
    else
    {
        SetNewChar(text, str, len);

        // text has more chars remove it
        for (int i = text->usedSubMeshList->size - 1; i >= len; i--)
        {
            SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

            if (ADrawableCheckVisible(subMesh->drawable))
            {
                ADrawableSetInVisible(subMesh->drawable);
            }

            AArrayList->Remove(text->usedSubMeshList, i);
            AArrayListAdd(text->font->unusedSubMeshList, subMesh);
        }
    }


    // set text size arrangement and alignment
    // set each char position
    switch (text->alignment)
    {
        case font_text_alignment_horizontal_left:
        {
            for (int i = 0; i < text->usedSubMeshList->size; i++)
            {
                SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

                if (subMesh->drawable->height > text->drawable->height)
                {
                    text->drawable->height = subMesh->drawable->height;
                }

                ADrawableSetPositionX(subMesh->drawable, text->drawable->width);
                text->drawable->width += subMesh->drawable->width + text->charSpacing;
            }

            break;
        }

        case font_text_alignment_horizontal_right:
        {
            for (int i = text->usedSubMeshList->size - 1; i > -1; i--)
            {
                SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

                if (subMesh->drawable->height > text->drawable->height)
                {
                    text->drawable->height = subMesh->drawable->height;
                }

                ADrawableSetPositionX(subMesh->drawable, -text->drawable->width);
                text->drawable->width += subMesh->drawable->width + text->charSpacing;
            }

            break;
        }

        case font_text_alignment_vertical_top:
        {
            for (int i = 0; i < text->usedSubMeshList->size; i++)
            {
                SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

                if (subMesh->drawable->width > text->drawable->width)
                {
                    text->drawable->width = subMesh->drawable->width;
                }

                ADrawableSetPositionY(subMesh->drawable, text->drawable->height);
                text->drawable->height -= subMesh->drawable->height + text->charSpacing;
            }

            break;
        }

        case font_text_alignment_vertical_bottom:
        {
            for (int i = text->usedSubMeshList->size - 1; i > -1; i--)
            {
                SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);

                if (subMesh->drawable->width > text->drawable->width)
                {
                    text->drawable->width = subMesh->drawable->width;
                }

                ADrawableSetPositionY(subMesh->drawable, -text->drawable->height);
                text->drawable->height -= subMesh->drawable->height + text->charSpacing;
            }

            break;
        }
    }
}


static void SetInt(FontText* text, int num)
{
	// max int digits count
	char buff[12];
    sprintf(buff,  "%d", num);
	AFont->SetString(text, buff);
}


static void SetFloat(FontText* text, float num)
{
	// max float digits count
	char buff[20];
    sprintf(buff,  "%.1f", num);
	AFont->SetString(text, buff);
}


static void Reuse(Font* font)
{
	ALog_A(font->textureAtlas != NULL, "Reuse font %p already reused", font);

    for (int i = 0; i < font->fontTextSet->elementList->size; i++)
    {
        FontText* text = AArrayListGet(font->fontTextSet->elementList, i, FontText*);
        text->font     = NULL;
        AArrayListAdd(textList, text);
    }

	font->textureAtlas = NULL;
    AArrayListAdd(fontList, font);
}


static void ReuseText(FontText* text)
{
    ALog_A(text->font != NULL, "ReuseText text %p already reused", text);

	for (int i = 0; i < text->usedSubMeshList->size; i++)
	{
        SubMesh* subMesh = AArrayListGet(text->usedSubMeshList, i, SubMesh*);
		ADrawableSetInVisible(subMesh->drawable);
        AArrayListAdd(text->font->unusedSubMeshList, subMesh);
	}

    AArrayIntSet->TryRemove(text->font->fontTextSet, (intptr_t) text);
    text->font = NULL;

    AArrayListAdd(textList, text);
}


struct AFont AFont[1] =
{
	Get,
    GetText,
    Draw,

	SetString,
	SetInt,
	SetFloat,

    Reuse,
    ReuseText,
};
