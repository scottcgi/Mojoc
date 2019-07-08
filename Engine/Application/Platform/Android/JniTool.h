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
 * Since  : 2017-4-1
 * Update : 2019-1-29
 * Author : scott.cgi
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