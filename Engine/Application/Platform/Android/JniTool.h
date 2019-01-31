/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2017-4-1
 * Update: 2019-1-29
 * Author: scott.cgi
 */


#ifndef JNI_TOOL_H
#define  JNI_TOOL_H


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
 * Call java method, and Must Called in same thread.
 *
 * className : pass to FindClass which is java class name with package like "java/lang/ClassLoader"
 * methodName: java class method name like "getClassLoader"
 * paramCode : java class method arguments like "()Ljava/lang/ClassLoader;"
 * ...       : varargs parameter which are method parameters.
 */
struct AJniTool
{
    void   (*GetMethodInfo)      (
                                     const char*    className,
                                     const char*    methodName,
                                     const char*    paramCode,
                                     JniMethodInfo* outJniMethodInfo
                                 );
    void   (*GetStaticMethodInfo)(
                                     const char*    className,
                                     const char*    methodName,
                                     const char*    paramCode,
                                     JniMethodInfo* outJniMethodInfo
                                 );

    jvalue (*CallStaticMethod)   (const char* className, const char* methodName, const char* paramCode, ...);
    jvalue (*CallObjectMethod)   (jobject     object,    const char* methodName, const char* paramCode, ...);
    jvalue (*CallClassMethod)    (jclass      cls,       const char* methodName, const char* paramCode, ...);

    /**
     * Get hash code from apk signature.
     */
    int    (*GetSignHashCode)    ();
};


extern struct AJniTool AJniTool[1];


#endif


#endif