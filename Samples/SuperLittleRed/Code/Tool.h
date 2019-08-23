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
 * Since    : 2015-4-26
 * Update   : 2019-2-17
 * Author   : scott.cgi
 */


#ifndef TOOL_H
#define TOOL_H


#include "Engine/Extension/Font.h"
#include "Engine/Graphics/Draw/Drawable.h"
#include "Engine/Physics/PhysicsBody.h"
#include "Hero.h"


typedef enum
{
    LanguageCode_zh,
    LanguageCode_en
}
LanguageCode;


struct ATool
{
    float        globalScaleX;
    float        globalScaleY;
    LanguageCode languageCode;

    void (*UpdateBox)      (PhysicsBody* body, Drawable* drawable);
    void (*InitBox)        (PhysicsBody* body, float x,  float y);

    void (*ShakeX)         (Drawable*    drawable);
    void (*ShakeZ)         (Drawable*    drawable);
    void (*Scale2)         (Drawable*    drawable);
    void (*ShakeWorld)     (void);

    void (*SetTimeToBuff)  (char      buff[], int   seconds);
    void (*UpdateArrowTail)(Arrow*    arrow,  float deltaSeconds);
    void (*FlyTextOnHero)  (FontText* text,   float y, float duration);
};


extern struct ATool ATool[1];


#endif
