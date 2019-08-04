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
 * Update   : 2019-1-29
 * Author   : scott.cgi
 */


#include "Engine/Application/Platform/Android/JniTool.h"


#ifdef IS_PLATFORM_ANDROID


#include <android/native_activity.h>
#include <stdbool.h>
#include "Engine/Toolkit/Platform/Log.h"


extern ANativeActivity* nativeActivity;


static inline JNIEnv* GetEnvPtr()
{
    static JNIEnv* envPtr = NULL;

    if (envPtr == NULL)
    {
        (*nativeActivity->vm)->AttachCurrentThread(nativeActivity->vm, &envPtr, NULL);
    }

    return envPtr;
}


static inline jclass GetClass(const char* className)
{
    JNIEnv* envPtr                   = GetEnvPtr();
    static  jobject   classLoaderObj = NULL;
    static  jmethodID loadClassId    = NULL;

    if (classLoaderObj == NULL)
    {
        jclass    activityCls      = (*envPtr)->FindClass(envPtr, "android/app/NativeActivity");
        jclass    loaderCls        = (*envPtr)->FindClass(envPtr, "java/lang/ClassLoader");

        jmethodID getClassLoaderId = (*envPtr)->GetMethodID
                                     (
                                        envPtr,
                                        activityCls,
                                        "getClassLoader",
                                        "()Ljava/lang/ClassLoader;"
                                     );

        classLoaderObj             = (*envPtr)->CallObjectMethod(envPtr, nativeActivity->clazz, getClassLoaderId);
        loadClassId                = (*envPtr)->GetMethodID
                                     (
                                         envPtr,
                                         loaderCls,
                                         "loadClass",
                                         "(Ljava/lang/String;)Ljava/lang/Class;"
                                     );
    }

    jstring classNameStr = (*envPtr)->NewStringUTF             (envPtr, className);
    jclass  cls          = (jclass) (*envPtr)->CallObjectMethod(envPtr, classLoaderObj, loadClassId, classNameStr);

    ALog_A(cls != NULL, "AJniTool GetClass cannot load class = %s", className);

    (*envPtr)->DeleteLocalRef(envPtr, classNameStr);

    return cls;
}


static inline void GetJniMethodInfo
(
    const char*    className,
    const char*    methodName,
    const char*    paramCode,
    bool           isStatic,
    JniMethodInfo* outJniMethodInfo
)
{
    JNIEnv*   envPtr = GetEnvPtr();
    jclass    cls    = GetClass(className);

    jmethodID methodID;

    if (isStatic)
    {
        methodID = (*envPtr)->GetStaticMethodID(envPtr, cls, methodName, paramCode);
    }
    else
    {
        methodID = (*envPtr)->GetMethodID(envPtr, cls, methodName, paramCode);
    }

    ALog_A
    (
        methodID != NULL,
        "AJniTool %s cannot get methodID, className = %s, methodName = %s, paramCode = %s",
        isStatic ? "GetStaticMethodID" : "GetMethodID",
        className,
        methodName,
        paramCode
    );

    outJniMethodInfo->envPtr   = envPtr;
    outJniMethodInfo->cls      = cls;
    outJniMethodInfo->methodID = methodID;
}


static void GetMethodInfo
(
    const char*    className,
    const char*    methodName,
    const char*    paramCode, 
    JniMethodInfo* outJniMethodInfo
)
{
    GetJniMethodInfo(className, methodName, paramCode, false, outJniMethodInfo);
}


static void GetStaticMethodInfo
(
    const char*    className,
    const char*    methodName,
    const char*    paramCode, 
    JniMethodInfo* outJniMethodInfo
)
{
    GetJniMethodInfo(className, methodName, paramCode, true, outJniMethodInfo);
}


static inline jvalue CallMethodV(jclass cls, jobject object, const char* methodName, const char* paramCode, ...)
{
    JNIEnv*   envPtr   = GetEnvPtr();
    jmethodID methodId = (*envPtr)->GetMethodID(envPtr, cls, methodName, paramCode);

    ALog_A
    (
        methodId != NULL,
        "AJniTool CallObjectMethodV cannot get methodID, methodName = %s, paramCode = %s",
        methodName,
        paramCode
    );

    const char* p = paramCode;
    // skip '()' to find out the return type
    while (*p++ != ')');

    jvalue  value = {};
    va_list args;
    va_start(args, paramCode);

    switch (*p)
    {
        case 'V':
            (*envPtr)->CallVoidMethodV             (envPtr, object, methodId, args);
            break;

        case '[':
        case 'L':
            value.l = (*envPtr)->CallObjectMethodV (envPtr, object, methodId, args);
            break;

        case 'Z':
            value.z = (*envPtr)->CallBooleanMethodV(envPtr, object, methodId, args);
            break;

        case 'B':
            value.b = (*envPtr)->CallByteMethodV   (envPtr, object, methodId, args);
            break;

        case 'C':
            value.c = (*envPtr)->CallCharMethodV   (envPtr, object, methodId, args);
            break;

        case 'S':
            value.s = (*envPtr)->CallShortMethodV  (envPtr, object, methodId, args);
            break;

        case 'I':
            value.i = (*envPtr)->CallIntMethodV    (envPtr, object, methodId, args);
            break;

        case 'J':
            value.j = (*envPtr)->CallLongMethodV   (envPtr, object, methodId, args);
            break;

        case 'F':
            value.f = (*envPtr)->CallFloatMethodV  (envPtr, object, methodId, args);
            break;

        case 'D':
            value.d = (*envPtr)->CallDoubleMethodV (envPtr, object, methodId, args);
            break;

        default:
            ALog_A(false, "AJniTool CallObjectMethodV paramCode = %s, illegal", paramCode);
    }

    va_end(args);

    return value;
}


static jvalue CallStaticMethod(const char* className, const char* methodName, const char* paramCode, ...)
{
    JNIEnv*   envPtr    = GetEnvPtr();
    jclass    cls       = GetClass(className);
    jmethodID methodId  = (*envPtr)->GetStaticMethodID(envPtr, cls, methodName, paramCode);

    ALog_A
    (
        methodId != NULL,
        "AJniTool CallStaticMethod cannot get methodID, methodName = %s, paramCode = %s",
        methodName,
        paramCode
    );

    const char* p = paramCode;
    // skip '()' to find out the return type
    while (*p++ != ')');

    jvalue  value = {};
    va_list args;
    va_start(args, paramCode);

    switch (*p)
    {
        case 'V':
            (*envPtr)->CallStaticVoidMethodV             (envPtr, cls, methodId, args);
            break;

        case '[':
        case 'L':
            value.l = (*envPtr)->CallStaticObjectMethodV (envPtr, cls, methodId, args);
            break;

        case 'Z':
            value.z = (*envPtr)->CallStaticBooleanMethodV(envPtr, cls, methodId, args);
            break;

        case 'B':
            value.b = (*envPtr)->CallStaticByteMethodV   (envPtr, cls, methodId, args);
            break;

        case 'C':
            value.c = (*envPtr)->CallStaticCharMethodV   (envPtr, cls, methodId, args);
            break;

        case 'S':
            value.s = (*envPtr)->CallStaticShortMethodV  (envPtr, cls, methodId, args);
            break;

        case 'I':
            value.i = (*envPtr)->CallStaticIntMethodV    (envPtr, cls, methodId, args);
            break;

        case 'J':
            value.j = (*envPtr)->CallStaticLongMethodV   (envPtr, cls, methodId, args);
            break;

        case 'F':
            value.f = (*envPtr)->CallStaticFloatMethodV  (envPtr, cls, methodId, args);
            break;

        case 'D':
            value.d = (*envPtr)->CallStaticDoubleMethodV (envPtr, cls, methodId, args);
            break;

        default:
            ALog_A(false, "AJniTool CallStaticMethod paramCode = %s, illegal", paramCode);
    }

    va_end(args);
    
    return value;
}


static jvalue CallMethod(jobject object, const char* methodName, const char* paramCode, ...)
{
    JNIEnv* envPtr = GetEnvPtr();
    jclass  cls    = (*envPtr)->GetObjectClass(envPtr, object);

    va_list args;
    va_start(args, paramCode);
    jvalue  value  = CallMethodV(cls, object, methodName, paramCode, args);
    va_end  (args);

    return value;
}


static jvalue CallNativeActivityMethod(const char* methodName, const char* paramCode, ...)
{
    va_list args;
    va_start(args, paramCode);
    jvalue  value = CallMethod(nativeActivity->clazz, methodName, paramCode, args);
    va_end  (args);
    
    return value;
}


static jvalue CallActivityMethod(const char* className, const char* methodName, const char* paramCode,  ...)
{
    va_list args;
    va_start(args, paramCode);
    jvalue  value  = CallMethodV(GetClass(className), nativeActivity->clazz, methodName, paramCode, args);
    va_end  (args);

    return value;
}


static int GetSignHashCode()
{
    jobject      packageManager = CallNativeActivityMethod
                                  (
                                      "getPackageManager",
                                      "()Landroid/content/pm/PackageManager;"
                                  ).l;

    jstring      packageName    = (jstring) CallNativeActivityMethod
                                  (
                                      "getPackageName",
                                      "()Ljava/lang/String;"
                                  ).l;

    jobject      packageInfo    = CallMethod
                                  (
                                      packageManager,
                                      "getPackageInfo",
                                      "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;",
                                      packageName,
                                      64
                                  ).l;

    JNIEnv*      envPtr         = GetEnvPtr();
    jclass       packageInfoCls = (*envPtr)->GetObjectClass(envPtr, packageInfo);
    jfieldID     signaturesFid  = (*envPtr)->GetFieldID
                                  (
                                      envPtr,
                                      packageInfoCls,
                                      "signatures",
                                      "[Landroid/content/pm/Signature;"
                                  );

    jobjectArray signatureArr   = (*envPtr)->GetObjectField       (envPtr, packageInfo,  signaturesFid);
    jobject      signature      = (*envPtr)->GetObjectArrayElement(envPtr, signatureArr, 0);

    return CallMethod(signature, "hashCode", "()I").i;
}


struct AJniTool AJniTool[1] =
{{
    GetMethodInfo,
    GetStaticMethodInfo,
    
    CallStaticMethod,
    CallMethod,
    
    CallNativeActivityMethod,
    CallActivityMethod,

    GetSignHashCode,
}};


#endif
