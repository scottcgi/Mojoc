//
// Created by scott.cgi on 2017/4/1.
//

#ifndef jni_tool_h
#define jni_tool_h

#include <jni.h>

typedef struct
{
    JNIEnv*   envPtr;
    jclass    cls;
    jmethodID methodID;
}
JniMethodInfo;


struct AJniTool
{
//--------------------------------------------------------------------------------------------------
// className:
// pass to FindClass which is java class name with package like "java/lang/ClassLoader"
//
// methodName:
// java class method name like "getClassLoader"
//
// paramCode:
// java class method arguments like "()Ljava/lang/ClassLoader;"
//
// ...:
// varargs parameter which are method arguments
//--------------------------------------------------------------------------------------------------

    void   (*GetMethodInfo)      (char*   className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo);
    void   (*GetStaticMethodInfo)(char*   className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo);

    jvalue (*CallStaticMethod)   (char*   className, char* methodName, char* paramCode, ...);
    jvalue (*CallObjectMethod)   (jobject object,    char* methodName, char* paramCode, ...);
    jvalue (*CallClassMethod)    (jclass  cls,       char* methodName, char* paramCode, ...);

    /**
     * Get has code from apk signature
     */
    int    (*GetSignHashCode)    ();
};

extern struct AJniTool AJniTool[1];

#endif
