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
 * Since    : 2017-4-1
 * Update   : 2019-7-30
 * Author   : scott.cgi
 */


#ifndef JNI_TOOL_H
#define JNI_TOOL_H


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID


#include <jni.h>
#include "Engine/Toolkit/HeaderUtils/Define.h"


typedef struct
{
    JNIEnv*   envPtr;
    jclass    cls;
    jmethodID methodID;
}
JniMethodInfo;


/**
 * Call java method, and must Called in same thread.
 *
 * className : pass to FindClass which is java class name with package like "java/lang/ClassLoader"
 * methodName: java class/object method name like "getClassLoader"
 * paramCode : java class/object method arguments like "()Ljava/lang/ClassLoader;"
 * ...       : varargs parameter which are method parameters.
 */
struct AJniTool
{
    void   (*GetMethodInfo)           (
                                          const char*    className,
                                          const char*    methodName,
                                          const char*    paramCode,
                                          JniMethodInfo* outJniMethodInfo
                                      );

    void   (*GetStaticMethodInfo)     (
                                          const char*    className,
                                          const char*    methodName,
                                          const char*    paramCode,
                                          JniMethodInfo* outJniMethodInfo
                                      );

    jvalue (*CallStaticMethod)        (
                                          const char*    className,
                                          const char*    methodName,
                                          const char*    paramCode,
                                          ...
                                      );

    /**
     * The object is any instance of a java class.
     */
    jvalue (*CallMethod)              (jobject object, const char* methodName, const char* paramCode, ...);

    /**
     * Call the method of NativeActivity instance which is created by NDK.
     */
    jvalue (*CallNativeActivityMethod)(const char* methodName, const char* paramCode,  ...);

    /**
     * Call the method of Activity instance which is inherited NativeActivity,
     * and the className is the Activity class.
     */
    jvalue (*CallActivityMethod)      (const char* className, const char* methodName, const char* paramCode,  ...);

    /**
     * Get hash code from apk signature.
     */
    int    (*GetSignHashCode)         ();
};


extern struct AJniTool AJniTool[1];


#endif


#endif