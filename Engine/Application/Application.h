/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2013-09-04
 * Update : 2019-1-25
 * Author : scott.cgi
 */


#ifndef APPLICATION_H
#define APPLICATION_H


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
     * extraData: Array(InputTouch*)*, the index of array not corresponding to fingerId.
     */
    ComponentMsg_OnTouch,
};


//----------------------------------------------------------------------------------------------------------------------


/**
 * Application lifecycle callbacks, not running in rendering thread.
 */
typedef struct
{
    /**
     * Callback when application window ready that can rendering.
     */
    void (*OnReady)            (void);

    /**
     * Callback when application going into the background.
     */
    void (*OnPause)            (void);

    /**
     * Callback when application going into the foreground.
     */
    void (*OnResume)           (void);

    /**
     * Callback when application is being destroyed.
     */
    void (*OnDestroy)          (void);

    /**
     * Callback when application surface size changed.
     */
    void (*OnResized)          (int width, int height);

    /**
     * Callback when application request save persistent data,
     * or AApplication->SaveData(param) called manually，
     * and this method running in a detached thread.
     *
     * param: pass by AApplication->SaveData,
     *       if application request save data then it will be NULL
     */
    void (*OnSaveData)         (void* param);
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
    void (*Init)    (void);

    /**
     * Loop called every frame.
     */
    void (*Loop)    (void);

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
    void (*Pause)   (void);

    /**
     * Called when application resume from pause.
     */
    void (*Resume)  (void);

    /**
     * Called when application destroyed.
     */
    void (*Destroy) (void);

    /**
     * Called when received one touch event.
     * touch event will convert to Array(InputTouch*)*,
     * and send to rootComponent with ComponentMsg_OnTouch.
     *
     * fingerId      : identify touch finger
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
    void (*Touches) (int fingerIds[], float pixelXs[], float pixelYs[], int touchesCount, InputTouchType inputTouchType);

    /**
     * Callback when application request save persistent data,
     * or can called manually if need to save persistent data,
     * and this method will start a detached thread to execute ApplicationCallbacks->OnSaveData.
     *
     * param: if application request call this method then pass NULL
     */
    void (*SaveData)(void* param);
};


extern struct AApplication AApplication[1];


/**
 * This function must implement ApplicationCallbacks method,
 * and called on very first entry.
 */
extern void Application_MainImpl(void);


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
