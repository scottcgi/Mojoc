//
// Created by scott.cgi on 2017/4/1.
//

#ifndef jni_tool_h
#define jni_tool_h

#include <jni.h>
#include "Engine/Toolkit/Head/MacroDefine.h"


typedef struct
{
    JNIEnv*   get_only envPtr;
    jclass    get_only cls;
    jmethodID get_only methodID;
}
JniMethodInfo;


struct AJniTool
{
/*
----------------------------------------------------------------------------------------------------
Must Called in same thread with OpenGL

className:
pass to FindClass which is java class name with package like "java/lang/ClassLoader"

methodName:
java class method name like "getClassLoader"

paramCode:
java class method arguments like "()Ljava/lang/ClassLoader;"

...:
varargs parameter which are method arguments
----------------------------------------------------------------------------------------------------
*/

    void   (*GetMethodInfo)      (const char*   className, const char* methodName, const char* paramCode, JniMethodInfo* out_param jniMethodInfo);
    void   (*GetStaticMethodInfo)(const char*   className, const char* methodName, const char* paramCode, JniMethodInfo* out_param jniMethodInfo);

    jvalue (*CallStaticMethod)   (const char*   className, const char* methodName, const char* paramCode, ...);
    jvalue (*CallObjectMethod)   (jobject       object,    const char* methodName, const char* paramCode, ...);
    jvalue (*CallClassMethod)    (jclass        cls,       const char* methodName, const char* paramCode, ...);

    /**
     * Get has code from apk signature
     */
    int    (*GetSignHashCode)    ();
};


extern struct AJniTool AJniTool[1];


#endif
