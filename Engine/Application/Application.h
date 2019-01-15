/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2013-09-04
 * Author: scott.cgi
 */

#ifndef APPLICATION_H
#define APPLICATION_H


#include "Engine/Application/Component.h"


/**
 * AApplication receive this type of subject message
 * then 'OnMessage' called for handle and the sender is AApplication
 *
 * these messages callback from target platform
 */
enum
{
    /**
     * When touch event happened
     * the extraData is Array(InputTouch*)
     */
    ComponentMsg_OnTouch,
};


//----------------------------------------------------------------------------------------------------------------------


typedef struct
{
    /**
     * Called when application window ready, openGL ready
     */
    void (*OnReady)           ();

    /**
     * Called when application going into the background
     */
    void (*OnPause)           ();

    /**
     * Called when application going into the foreground
     */
    void (*OnResume)          ();

    /**
     * Called when application is being destroyed
     */
    void (*OnDestroy)         ();

    /**
     * Called when application surface size changed
     */
    void (*OnResized)         (int width, int height);

    /**
     * Called when application request save persistent data
     *
     * outSaveData: the persistent data ptr will be saved
     * outLength  : the persistent data bytes count
     */
    void (*OnSaveData)        (void** outSaveData, size_t* outLength);

    /**
     * Called when application init and have saved data
     */
    void (*OnInitWithSavedData)(void* savedData, size_t length);
}
ApplicationCallbacks;


//----------------------------------------------------------------------------------------------------------------------


struct AApplication
{
    /**
     * Root Component when OnReady can add child on
     */
    Component             component[1];

    /**
     * Application life cycle callback
     */
    ApplicationCallbacks  callbacks[1];

/*
------------------------------------------------------------------------------------------------------------------------
    These functions called in target platform
------------------------------------------------------------------------------------------------------------------------
*/

    /**
     * Initialize each modules
     */
    void (*Init)            ();

    /**
     * Called every frame
     */
    void (*Loop)            ();

    /**
     * Called when OpenGL ready to rendering
     */
    void (*GLReady)         (int width, int height);

    /**
     * Called when window resized
     */
    void (*Resized)         (int width, int height);

    /**
     * Called when application pause
     */
    void (*Pause)           ();

    /**
     * Called when application resume from pause
     */
    void (*Resume)          ();

    /**
     * Called when application destroyed
     */
    void (*Destroy)         ();

    /**
     * Called when receive touch event
     */
    void (*Touch)           (Array(InputTouch*)* touchData);

    /**
     * Called when application need to save data
     */
    void (*SaveData)        ();
};


extern struct AApplication AApplication[1];


/**
 * This function must implement ApplicationCallbacks method
 * called on very first entry
 */
extern void Application_Main();


static inline void AApplication_AppendChild(Component* child)
{
    AComponent->AppendChild(AApplication->component, child);
}


static inline void AApplication_AddChild(Component* child, int order)
{
    AComponent->AddChild(AApplication->component, child, order);
}


#endif
