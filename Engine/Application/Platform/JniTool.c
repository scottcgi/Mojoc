//
// Created by scottcgi on 2017/4/1.
//


#include <android/native_activity.h>

#include "Engine/Application/Platform/JniTool.h"
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


static inline jclass GetClass(const char *className)
{
    JNIEnv* envPtr                  = GetEnvPtr();

    static  jobject   classLoader   = NULL;
    static  jmethodID loadClassId   = NULL;

    if (classLoader == NULL)
    {
        jclass    activityClass    = (*envPtr)->FindClass(envPtr, "android/app/NativeActivity");
        jclass    loaderClass      = (*envPtr)->FindClass(envPtr, "java/lang/ClassLoader");

        jmethodID getClassLoaderId = (*envPtr)->GetMethodID
                                     (
                                        envPtr,
                                        activityClass,
                                        "getClassLoader",
                                        "()Ljava/lang/ClassLoader;"
                                     );

        classLoader                = (*envPtr)->CallObjectMethod(envPtr, nativeActivity->clazz, getClassLoaderId);
        loadClassId                = (*envPtr)->GetMethodID
                                     (
                                         envPtr,
                                         loaderClass,
                                         "loadClass",
                                         "(Ljava/lang/String;)Ljava/lang/Class;"
                                     );
    }

    jstring classNameStr = (*envPtr)->NewStringUTF             (envPtr, className);
    jclass  cls          = (jclass) (*envPtr)->CallObjectMethod(envPtr, classLoader, loadClassId, classNameStr);

    ALogA(cls != NULL, "JniTool GetClass can not load class = %s", className);

    (*envPtr)->DeleteLocalRef(envPtr, classNameStr);

    return cls;
}


static void GetJniMethodInfo(char* className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo)
{
    JNIEnv*   envPtr   = GetEnvPtr();
    jclass    cls      = GetClass(className);

    jmethodID methodID = (*envPtr)->GetStaticMethodID(envPtr, cls, methodName, paramCode);

    ALogA
    (
        methodID != NULL,
        "JniTool GetJniMethodInfo can not get methodID, className = %s, methodName = %s, paramCode = %s",
        className,
        methodName,
        paramCode
    );

    outJniMethodInfo->envPtr   = envPtr;
    outJniMethodInfo->cls      = cls;
    outJniMethodInfo->methodID = methodID;
}

static void GetJniStaticMethodInfo(char* className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo)
{
    JNIEnv*   envPtr   = GetEnvPtr();
    jclass    cls      = GetClass(className);

    jmethodID methodID = (*envPtr)->GetStaticMethodID(envPtr, cls, methodName, paramCode);

    ALogA
    (
        methodID != NULL,
        "JniTool GetJniStaticMethodInfo can not get methodID, className = %s, methodName = %s, paramCode = %s",
        className,
        methodName,
        paramCode
    );

    outJniMethodInfo->envPtr   = envPtr;
    outJniMethodInfo->cls      = cls;
    outJniMethodInfo->methodID = methodID;
}

struct AJniTool AJniTool[1] =
{
    GetJniMethodInfo,
    GetJniStaticMethodInfo,
};