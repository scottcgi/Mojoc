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
 * Since    : 2014-6-23
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#ifndef GAME_ACTOR_H
#define GAME_ACTOR_H


#include "Engine/Extension/Font.h"
#include "Engine/Application/Component.h"


struct AGameActor
{
   Component component[1];
   Font*     hpFont;
   Font*     talkFont;

   void (*Init)(void);
   void (*Run) (void);
};


extern struct AGameActor AGameActor[1];


#endif 
