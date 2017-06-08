/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-6-23
 * Author : scott.cgi
 * Version: 0.1
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

   void (*Init)();
   void (*Run) ();
};


extern struct AGameActor AGameActor[1];


#endif 
