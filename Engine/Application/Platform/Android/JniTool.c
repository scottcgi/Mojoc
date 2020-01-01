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
 * Update   : 2019-8-1
 * Author   : scott.cgi
 */


#include "Engine/Application/Platform/Android/JniTool.h"


//------------------------
#ifdef IS_PLATFORM_ANDROID
//------------------------


#include <android/native_activity.h>
#include <stdbool.h>
#include "Engine/Toolkit/Platform/Log.h"


extern ANativeActivity* nativeActivity;
static JNIEnv*          envPtr = NULL;


static void Init()
{
    (*nativeActivity->vm)->AttachCurrentThread(nativeActivity->vm, &envPtr, NULL);
}


static JNIEnv* GetEnvPtr()
{
    return envPtr;
}


static inline jclass GetClass(const char* className)
{
    static  jobject   classLoaderObj = NULL;
    static  jmethodID loadClassID    = NULL;

    if (classLoaderObj == NULL)
    {
        jclass    activityCls      = (*envPtr)->FindClass(envPtr, "android/app/NativeActivity");
        jclass    loaderCls        = (*envPtr)->FindClass(envPtr, "java/lang/ClassLoader");

        jmethodID getClassLoaderID = (*envPtr)->GetMethodID
                                     (
                                        envPtr,
                                        activityCls,
                                        "getClassLoader",
                                        "()Ljava/lang/ClassLoader;"
                                     );

        classLoaderObj             = (*envPtr)->CallObjectMethod(envPtr, nativeActivity->clazz, getClassLoaderID);
        loadClassID                = (*envPtr)->GetMethodID
                                     (
                                         envPtr,
                                         loaderCls,
                                         "loadClass",
                                         "(Ljava/lang/String;)Ljava/lang/Class;"
                                     );
    }

    jstring classNameStr = (*envPtr)->NewStringUTF    (envPtr, className);
    jclass  cls          = (*envPtr)->CallObjectMethod(envPtr, classLoaderObj, loadClassID, classNameStr);

    ALog_A(cls != NULL, "AJniTool GetClass cannot load class = %s", className);

    (*envPtr)->DeleteLocalRef(envPtr, classNameStr);

    return cls;
}


static jvalue CallStaticMethod(const char* className, const char* methodName, const char* paramCode, ...)
{
    jclass      cls      = GetClass(className);
    jmethodID   methodID = (*envPtr)->GetStaticMethodID(envPtr, cls, methodName, paramCode);

    const char* p        = paramCode;
    // skip '()' to find out the return type
    while (*p++ != ')');

    va_list args;
    va_start(args, paramCode);
    jvalue  value = {};

    switch (*p)
    {
        case 'V':
            (*envPtr)->CallStaticVoidMethodV             (envPtr, cls, methodID, args);
            break;

        case '[':
        case 'L':
            value.l = (*envPtr)->CallStaticObjectMethodV (envPtr, cls, methodID, args);
            break;

        case 'Z':
            value.z = (*envPtr)->CallStaticBooleanMethodV(envPtr, cls, methodID, args);
            break;

        case 'B':
            value.b = (*envPtr)->CallStaticByteMethodV   (envPtr, cls, methodID, args);
            break;

        case 'C':
            value.c = (*envPtr)->CallStaticCharMethodV   (envPtr, cls, methodID, args);
            break;

        case 'S':
            value.s = (*envPtr)->CallStaticShortMethodV  (envPtr, cls, methodID, args);
            break;

        case 'I':
            value.i = (*envPtr)->CallStaticIntMethodV    (envPtr, cls, methodID, args);
            break;

        case 'J':
            value.j = (*envPtr)->CallStaticLongMethodV   (envPtr, cls, methodID, args);
            break;

        case 'F':
            value.f = (*envPtr)->CallStaticFloatMethodV  (envPtr, cls, methodID, args);
            break;

        case 'D':
            value.d = (*envPtr)->CallStaticDoubleMethodV (envPtr, cls, methodID, args);
            break;

        default:
            ALog_A(false, "AJniTool CallStaticMethod paramCode = %s, illegal", paramCode);
    }

    va_end(args);

    return value;
}


static inline jvalue CallMethodV(jobject object, const char* methodName, const char* paramCode, va_list args)
{
    jmethodID   methodID = (*envPtr)->GetMethodID(envPtr, (*envPtr)->GetObjectClass(envPtr, object), methodName, paramCode);

    const char* p        = paramCode;
    // skip '()' to find out the return type
    while (*p++ != ')');

    jvalue value = {};

    switch (*p)
    {
        case 'V':
            (*envPtr)->CallVoidMethodV             (envPtr, object, methodID, args);
            break;

        case '[':
        case 'L':
            value.l = (*envPtr)->CallObjectMethodV (envPtr, object, methodID, args);
            break;

        case 'Z':
            value.z = (*envPtr)->CallBooleanMethodV(envPtr, object, methodID, args);
            break;

        case 'B':
            value.b = (*envPtr)->CallByteMethodV   (envPtr, object, methodID, args);
            break;

        case 'C':
            value.c = (*envPtr)->CallCharMethodV   (envPtr, object, methodID, args);
            break;

        case 'S':
            value.s = (*envPtr)->CallShortMethodV  (envPtr, object, methodID, args);
            break;

        case 'I':
            value.i = (*envPtr)->CallIntMethodV    (envPtr, object, methodID, args);
            break;

        case 'J':
            value.j = (*envPtr)->CallLongMethodV   (envPtr, object, methodID, args);
            break;

        case 'F':
            value.f = (*envPtr)->CallFloatMethodV  (envPtr, object, methodID, args);
            break;

        case 'D':
            value.d = (*envPtr)->CallDoubleMethodV (envPtr, object, methodID, args);
            break;

        default:
            ALog_A(false, "AJniTool CallMethodV paramCode = %s, illegal", paramCode);
    }

    return value;
}


static jvalue CallMethod(jobject object, const char* methodName, const char* paramCode, ...)
{
    va_list args;
    va_start(args, paramCode);
    jvalue  value  = CallMethodV(object, methodName, paramCode, args);
    va_end  (args);

    return value;
}


static jvalue CallNativeActivityMethod(const char* methodName, const char* paramCode, ...)
{
    va_list args;
    va_start(args, paramCode);
    jvalue  value = CallMethodV(nativeActivity->clazz, methodName, paramCode, args);
    va_end  (args);
    
    return value;
}


//----------------------------------------------------------------------------------------------------------------------


static jvalue GetStaticField(const char* className, const char* fieldName, const char* typeCode)
{
    jclass   cls     = GetClass(className);
    jfieldID fieldID = (*envPtr)->GetStaticFieldID(envPtr, cls, fieldName, typeCode);
    jvalue   value;

    switch (*typeCode)
    {
        case '[':
        case 'L':
            value.l = (*envPtr)->GetStaticObjectField (envPtr, cls, fieldID);
            break;

        case 'Z':
            value.z = (*envPtr)->GetStaticBooleanField(envPtr, cls, fieldID);
            break;

        case 'B':
            value.b = (*envPtr)->GetStaticByteField   (envPtr, cls, fieldID);
            break;

        case 'C':
            value.c = (*envPtr)->GetStaticCharField   (envPtr, cls, fieldID);
            break;

        case 'S':
            value.s = (*envPtr)->GetStaticShortField  (envPtr, cls, fieldID);
            break;

        case 'I':
            value.i = (*envPtr)->GetStaticIntField    (envPtr, cls, fieldID);
            break;

        case 'J':
            value.j = (*envPtr)->GetStaticLongField   (envPtr, cls, fieldID);
            break;

        case 'F':
            value.f = (*envPtr)->GetStaticFloatField  (envPtr, cls, fieldID);
            break;

        case 'D':
            value.d = (*envPtr)->GetStaticDoubleField (envPtr, cls, fieldID);
            break;

        default:
            ALog_A(false, "AJniTool GetStaticField typeCode = %s, illegal", typeCode);
    }

    return value;
}


static inline jvalue GetField(jobject object, const char* fieldName, const char* typeCode)
{
    jfieldID fieldID = (*envPtr)->GetFieldID(envPtr, (*envPtr)->GetObjectClass(envPtr, object), fieldName, typeCode);
    jvalue   value   = {};

    switch (*typeCode)
    {
        case '[':
        case 'L':
            value.l = (*envPtr)->GetObjectField (envPtr, object, fieldID);
            break;

        case 'Z':
            value.z = (*envPtr)->GetBooleanField(envPtr, object, fieldID);
            break;

        case 'B':
            value.b = (*envPtr)->GetByteField   (envPtr, object, fieldID);
            break;

        case 'C':
            value.c = (*envPtr)->GetCharField   (envPtr, object, fieldID);
            break;

        case 'S':
            value.s = (*envPtr)->GetShortField  (envPtr, object, fieldID);
            break;

        case 'I':
            value.i = (*envPtr)->GetIntField    (envPtr, object, fieldID);
            break;

        case 'J':
            value.j = (*envPtr)->GetLongField   (envPtr, object, fieldID);
            break;

        case 'F':
            value.f = (*envPtr)->GetFloatField  (envPtr, object, fieldID);
            break;

        case 'D':
            value.d = (*envPtr)->GetDoubleField (envPtr, object, fieldID);
            break;

        default:
            ALog_A(false, "AJniTool GetField typeCode = %s, illegal", typeCode);
    }

    return value;
}


static jsize GetArrayLength(jarray array)
{
    return (*envPtr)->GetArrayLength(envPtr, array);
}


static jvalue GetArrayAt(jarray array, jint index, char typeChar)
{
    jvalue value = {};

    switch (typeChar)
    {
        case '[':
        case 'L':
            value.l = (*envPtr)->GetObjectArrayElement  (envPtr, array, index);
            break;

        case 'Z':
            value.z = (*envPtr)->GetBooleanArrayElements(envPtr, array, false)[index];
            break;

        case 'B':
            value.b = (*envPtr)->GetByteArrayElements   (envPtr, array, false)[index];
            break;

        case 'C':
            value.c = (*envPtr)->GetCharArrayElements   (envPtr, array, false)[index];
            break;

        case 'S':
            value.s = (*envPtr)->GetShortArrayElements  (envPtr, array, false)[index];
            break;

        case 'I':
            value.i = (*envPtr)->GetIntArrayElements    (envPtr, array, false)[index];
            break;

        case 'J':
            value.j = (*envPtr)->GetLongArrayElements   (envPtr, array, false)[index];
            break;

        case 'F':
            value.f = (*envPtr)->GetFloatArrayElements  (envPtr, array, false)[index];
            break;

        case 'D':
            value.d = (*envPtr)->GetDoubleArrayElements (envPtr, array, false)[index];
            break;

        default:
            ALog_A(false, "AJniTool GetArrayAt typeChar = %c, illegal", typeChar);
    }

    return value;
}


static jvalue GetNativeActivityField(const char* fieldName, const char* typeCode)
{
    return GetField(nativeActivity->clazz, fieldName, typeCode);
}


static int GetSignHashCode()
{
    jobject packageManager = CallNativeActivityMethod
                             (
                                 "getPackageManager",
                                 "()Landroid/content/pm/PackageManager;"
                             ).l;

    jstring packageName    = CallNativeActivityMethod
                             (
                                 "getPackageName",
                                 "()Ljava/lang/String;"
                             ).l;

    jobject packageInfo    = CallMethod
                             (
                                 packageManager,
                                 "getPackageInfo",
                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;",
                                 packageName,
                                 GetStaticField("android/content/pm/PackageManager", "GET_SIGNATURES", "I").i
                             ).l;

    jarray  signatureArr   = GetField(packageInfo, "signatures", "[Landroid/content/pm/Signature;").l;
    jobject signature      = (*envPtr)->GetObjectArrayElement(envPtr, signatureArr, 0);

    return CallMethod(signature, "hashCode", "()I").i;
}


struct AJniTool AJniTool[1] =
{{
    Init,
    GetEnvPtr,

    CallStaticMethod,
    CallMethod,
    CallNativeActivityMethod,

    GetStaticField,
    GetField,
    GetArrayLength,
    GetArrayAt,
    GetNativeActivityField,

    GetSignHashCode,
}};


//---------------------------
#endif // IS_PLATFORM_ANDROID
//---------------------------

