/*
 *
 *
 *  Created on: 2013-09-04
 *      Author: scott.cgi
 */

#ifndef application_h
#define application_h

#include "Engine/Application/Component.h"

/**
 * AApplication receive this type of subject message
 * then 'OnMessage' called for handle
 * the sender is AApplication
 *
 * these messages callback from target platform
 */
enum
{
	/**
	 * When touch event happened
	 * the extraData is Array(EventTouchPoint)
     */
	application_msg_on_touch,
};

//--------------------------------------------------------------------------------------------------

typedef struct
{
	/** Called when application window ready, openGL ready */
	void (*OnReady)      ();

	/** Called when application going into the background */
	void (*OnPause)      ();

	/** Called when application going into the foreground */
	void (*OnResume)     ();

	/** Called when application surface size changed */
	void (*OnResized)    (int width, int height);

    /** Called when application request save persistent data */
    void (*OnGetSaveData)(void** outSaveData, int* outLength);

    /** Called when application start then set saved persistent data */
    void (*OnSetSaveData)(void*  savedData,   int  length);
}
ApplicationCallbacks;

//--------------------------------------------------------------------------------------------------

struct AApplication
{
	/**
	 * Root Component when OnCreated can add child on
	 */
	Component             component[1];

	/**
	 * Application life cycle callback
	 */
	ApplicationCallbacks  callbacks[1];

//--------------------------------------------------------------------------------------------------
// These functions called in target platform
//--------------------------------------------------------------------------------------------------

	/**
	 * Initialize each modules
	 */
	void (*Init)   ();

	/**
	 * Called every frame
	 */
	void (*Loop)   ();

    /**
     * Called when OpenGL ready to rendering
     */
    void (*GLReady)(int width, int height);

    /**
     * Called when window resized
     */
    void (*Resized)(int width, int height);

    /**
     * Called when application pause
     */
    void (*Pause)  ();

    /**
     * Called when application resume from pause
     */
    void (*Resume) ();

    /**
     * Called when receive touch event
     */
    void (*Touch)  (Array(EventTouchPoint)* touchData);
};

extern struct AApplication AApplication[1];


/**
 * This function must implement ApplicationCallbacks method
 * called on very first entry
 */
extern void ApplicationMain();


static inline void AApplicationAppendChild(Component* child)
{
    AComponent->AppendChild(AApplication->component, child);
}


static inline void AApplicationAddChild(Component* child, int order)
{
	AComponent->AddChild(AApplication->component, child, order);
}


#endif
