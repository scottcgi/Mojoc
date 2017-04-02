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
    void (*GetJniMethodInfo)      (char* className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo);
    void (*GetJniStaticMethodInfo)(char* className, char* methodName, char* paramCode, JniMethodInfo* outJniMethodInfo);
};

extern struct AJniTool AJniTool[1];

#endif
