/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2013-09-04
 * Update: 2019-1-25
 * Author: scott.cgi
 */


#ifndef APPLICATION_H
#define  APPLICATION_H


#include "Engine/Application/Component.h"
#include "Engine/Application/Input.h"


/**
 * ComponentState subject.
 *
 * AApplication receive this type of subject message,
 * then the OnMessage called for handle,
 * and these messages callback from target platform.
 */
enum
{
    /**
     * When touch event happened.
     *
     * sender   : AApplication
     * extraData: Array(InputTouch*)*
     */
    ComponentMsg_OnTouch,
};


//----------------------------------------------------------------------------------------------------------------------


/**
 * Application lifecycle callbacks.
 */
typedef struct
{
    /**
     * Callback when application window ready that can rendering.
     */
    void (*OnReady)            ();

    /**
     * Callback when application going into the background.
     */
    void (*OnPause)            ();

    /**
     * Callback when application going into the foreground.
     */
    void (*OnResume)           ();

    /**
     * Callback when application is being destroyed.
     */
    void (*OnDestroy)          ();

    /**
     * Callback when application surface size changed.
     */
    void (*OnResized)          (int width, int height);

    /**
     * Callback when application request save persistent data.
     *
     * outSaveData: the persistent data ptr will be saved
     * outSize    : the persistent data bytes count
     */
    void (*OnSaveData)         (void** outSaveData, size_t* outSize);

    /**
     * Callback when application create.
     *
     * savedData: the data saved when OnSaveData
     * size     : savedData size
     */
    void (*OnInitWithSavedData)(void* savedData, size_t size);
}
ApplicationCallbacks;


//----------------------------------------------------------------------------------------------------------------------


/**
 * Control Application and interact with the native platform.
 */
struct AApplication
{
    /**
     * Root Component after application Ready can add child,
     * and everything from here.
     */
    Component             rootComponent[1];

    /**
     * Application life cycle callbacks.
     */
    ApplicationCallbacks  callbacks[1];

    /* These functions called in target platform. */

    /**
     * Init each modules when application create.
     */
    void (*Init)    ();

    /**
     * Loop called every frame.
     */
    void (*Loop)    ();

    /**
     * Called when ready to rendering.
     */
    void (*Ready)   (int width, int height);

    /**
     * Called when window resized.
     */
    void (*Resized) (int width, int height);

    /**
     * Called when application pause.
     */
    void (*Pause)   ();

    /**
     * Called when application resume from pause.
     */
    void (*Resume)  ();

    /**
     * Called when application destroyed.
     */
    void (*Destroy) ();

    /**
     * Called when received one touch event.
     * touch event will convert to Array(InputTouch*)*,
     * and send to rootComponent with ComponentMsg_OnTouch.
     *
     * fingerId       : identify touch finger
     * pixelX        : screen pixel x
     * pixelY        : screen pixel y
     * inputTouchType: touch event type
     */
    void (*Touch)   (int fingerId, float pixelX, float pixelY, InputTouchType inputTouchType);

    /**
     * Called when received multiple touches event.
     * touches event will convert to Array(InputTouch*)*,
     * and send to rootComponent with ComponentMsg_OnTouch.
     */
    void (*Touches) (int fingerIds[], float pixelXs[], float pixelYs[], int touchCount, InputTouchType inputTouchType);

    /**
     * Called when application need to save data.
     */
    void (*SaveData)();
};


extern struct AApplication AApplication[1];


/**
 * This function must implement ApplicationCallbacks method,
 * and called on very first entry.
 */
extern void Application_MainImpl();


/**
 * Append child to rootComponent.
 */
static inline void AApplication_AppendChild(Component* child)
{
    AComponent->AppendChild(AApplication->rootComponent, child);
}


/**
 * Add child to rootComponent.
 */
static inline void AApplication_AddChild(Component* child, int order)
{
    AComponent->AddChild(AApplication->rootComponent, child, order);
}


#endif
