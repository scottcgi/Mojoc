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
 * Since    : 2017-4-1
 * Update   : 2019-7-30
 * Author   : scott.cgi
 */


#ifndef JNI_TOOL_H
#define JNI_TOOL_H


#include "Engine/Toolkit/Platform/Platform.h"


//------------------------
#ifdef IS_PLATFORM_ANDROID
//------------------------


#include <jni.h>
#include "Engine/Toolkit/HeaderUtils/Define.h"


/**
 * The AJniTool must Called in game thread, not main thread.
 * the main thread Env is nativeActivity->env, and work in ANativeActivityCallbacks.
 *
 * className : pass to FindClass which is java class name with package such as: "java/lang/ClassLoader"
 * methodName: java class/object method name such as: "getClassLoader"
 * paramCode : java class/object method arguments such as: "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;"
 * ...       : varargs parameter which are method parameters.
 *
 * typeCode  : java class/object field type such as: "Landroid/content/pm/PackageInfo;", "I"
 * typeChar  : java class/object field type such as: 'L', 'B', 'I', 'V' (void)
 *
 * jvalue    : '[' - l - Array (Object)
 *             'L' - l - Object
 *             'Z' - z - Boolean
 *             'B' - b - Byte
 *             'C' - c - Char
 *             'S' - s - Short
 *             'I' - i - Int
 *             'J' - j - Long (in C is long long type)
 *             'F' - f - Float
 *             'D' - d - Double
 *
 * 
 * Important tips:
 * - if the param of java method is String, then you must pass the jstring (not char*) to "paramCode".
 * - if JNI calls cannot find a method or field, may consider "code proguard" settings.
 */
struct AJniTool
{
    /**
     * Called by NativeGlue for init a thread env, and AJniTool work with this env.
     */
    void    (*Init)                     ();

    /**
     * Get the thread evn of AJniTool.
     */
    JNIEnv* (*GetEnvPtr)                ();

    /**
     * Call java class static method.
     */
    jvalue  (*CallStaticMethod)        (const char* className, const char* methodName, const char* paramCode, ...);

    /**
     * Call java object method.
     * the object is any instance of a java class.
     */
    jvalue  (*CallMethod)              (jobject     object,    const char* methodName, const char* paramCode, ...);

    /**
     * Call the method of NativeActivity (or the Activity inherits NativeActivity) instance.
     */
    jvalue  (*CallNativeActivityMethod)(const char* methodName, const char* paramCode, ...);

    /**
     * Get class static field.
     */
    jvalue  (*GetStaticField)          (const char* className,  const char* fieldName,  const char* typeCode);

    /**
     * Get java object field.
     */
    jvalue  (*GetField)                (jobject object, const char* fieldName, const char* typeCode);

    /**
     * Get java object Array field length.
     * the array is jobject which GetStaticField/GetField returned.
     */
    jsize   (*GetArrayLength)          (jarray array);

    /**
     * Get java object Array field element at index.
     * the array is jobject which GetStaticField/GetField returned.
     */
    jvalue  (*GetArrayAt)              (jarray array, jint index, char typeChar);

    /**
     * Get the field of NativeActivity (or the Activity inherits NativeActivity) instance.
     */
    jvalue  (*GetNativeActivityField)  (const char* fieldName, const char* typeCode);

    /**
     * Get hash code from apk signature (work only in Android).
     */
    int     (*GetSignHashCode)         ();
};


extern struct AJniTool AJniTool[1];


//---------------------------
#endif // IS_PLATFORM_ANDROID
//---------------------------


#endif