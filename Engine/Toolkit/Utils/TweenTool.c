/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/blob/master/Docs/CodeStyle.md
 *
 * Since    : 2016-6-24
 * Update   : 2019-1-8
 * Author   : scott.cgi
 */


#include <string.h>
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Toolkit/Platform/Log.h"


enum
{
    /**
     * Max number of TweenActions in context.
     */
    TweenAction_Length = 30,
};


static Array(TweenAction*) actionArr[1] =
{
    (TweenAction*[TweenAction_Length]) {},
    0
};


static TweenAction*      action      = NULL;
static TweenActionValue* actionValue = NULL;


#define CheckAction(tag) \
    ALog_A(action      != NULL, "ATweenTool " tag " TweenAction not created.");


#define CheckActionValue(tag) \
    ALog_A(actionValue != NULL, "ATweenTool " tag " TweenActionValue cannot NULL.");


static struct ATweenTool* AddAction()
{
    ALog_A
    (
        actionArr->length <= TweenAction_Length,
        "ATweenTool cannot cache TweenActions = %d more than %d",
        actionArr->length,
        TweenAction_Length
    );

    action      = ATween->GetAction();
    actionValue = NULL;

    AArray_Set
    (
        actionArr,
        actionArr->length++,
        action,
        TweenAction*
    );

    return ATweenTool;
}


static inline struct ATweenTool* SetValue(TweenActionValueGetSet* valueGetSet, float value)
{
    CheckAction("SetValue");

    actionValue        = ATween->AddTweenActionValue(action);
    actionValue->OnGet = valueGetSet->OnGet;
    actionValue->OnSet = valueGetSet->OnSet;
    actionValue->value = value;

    return ATweenTool;
}


static struct ATweenTool* SetDuration(float duration)
{
    CheckAction("SetDuration");
    action->duration = duration;

    return ATweenTool;
}


static struct ATweenTool* SetRelative(bool isRelative)
{
    CheckAction     ("SetRelative");
    CheckActionValue("SetRelative");
    actionValue->isRelative = isRelative;

    return ATweenTool;
}


static struct ATweenTool* SetEaseType(TweenEaseType easeType)
{
    CheckAction     ("SetEaseType");
    CheckActionValue("SetEaseType");
    actionValue->easeType = easeType;

    return ATweenTool;
}


static struct ATweenTool* AddInterval(float duration)
{
    AddAction();
    SetDuration(duration);

    return ATweenTool;
}


//----------------------------------------------------------------------------------------------------------------------


static struct ATweenTool* AddMoveX(float moveX, float duration)
{
    AddAction();
    SetValue(ATweenActionValueGetSetImpl->moveX, moveX);
    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddMoveY(float moveY, float duration)
{
    AddAction();
    SetValue(ATweenActionValueGetSetImpl->moveY, moveY);
    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddMove2(float moveX, float moveY, float duration, bool isRelative, TweenEaseType easeType)
{
    AddAction  ();

    SetValue   (ATweenActionValueGetSetImpl->moveX, moveX);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetValue   (ATweenActionValueGetSetImpl->moveY, moveY);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddMoveSame2(float move, float duration, bool  isRelative, TweenEaseType easeType)
{
    return AddMove2(move, move, duration, isRelative, easeType);
}


static struct ATweenTool* AddScaleX(float scaleX, float duration)
{
    AddAction();

    SetValue(ATweenActionValueGetSetImpl->scaleX, scaleX);
    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddScaleY(float scaleY, float duration)
{
    AddAction();

    SetValue(ATweenActionValueGetSetImpl->scaleY, scaleY);
    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddScale2(float scaleX, float scaleY, float duration, bool isRelative, TweenEaseType easeType)
{
    AddAction  ();

    SetValue   (ATweenActionValueGetSetImpl->scaleX, scaleX);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetValue   (ATweenActionValueGetSetImpl->scaleY, scaleY);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddScaleSame2(float scale, float duration, bool  isRelative, TweenEaseType  easeType)
{
    return AddScale2(scale, scale, duration, isRelative, easeType);
}


static struct ATweenTool* AddRotateZ(float rotateZ, float duration)
{
    AddAction();

    SetValue(ATweenActionValueGetSetImpl->rotateZ, rotateZ);
    SetDuration(duration);

    return ATweenTool;
}


static struct ATweenTool* AddFadeTo(float fadeTo, float duration)
{
    AddAction();

    SetValue(ATweenActionValueGetSetImpl->fadeTo, fadeTo);
    SetDuration(duration);

    return ATweenTool;
}


//----------------------------------------------------------------------------------------------------------------------


static struct ATweenTool* SetUserDataInt(int slotIndex, int value)
{
    CheckAction("SetUserDataInt");
    AUserData_SetSlotInt(action->userData, slotIndex, value);
    return ATweenTool;
}


static struct ATweenTool* SetUserDataFloat(int slotIndex, float value)
{
    CheckAction("SetUserDataFloat");
    AUserData_SetSlotFloat(action->userData, slotIndex, value);
    return ATweenTool;
}


static struct ATweenTool* SetUserDataPtr(int slotIndex, void* value)
{
    CheckAction("SetUserDataPtr");
    AUserData_SetSlotPtr(action->userData, slotIndex, value);
    return ATweenTool;
}


static struct ATweenTool* SetUserDataString(int slotIndex, char* value)
{
    CheckAction("SetUserDataString");
    AUserData_SetSlotString(action->userData, slotIndex, value);
    return ATweenTool;
}


//----------------------------------------------------------------------------------------------------------------------


static struct ATweenTool* SetQueue(bool isQueue)
{
    CheckAction("SetQueue");
    action->isQueue = isQueue;

    return ATweenTool;
}


static struct ATweenTool* SetOnComplete(TweenActionOnComplete OnComplete)
{
    CheckAction("SetOnComplete");
    action->OnComplete = OnComplete;

    return ATweenTool;
}


static struct ATweenTool* SetTarget(void* target)
{
    CheckAction("SetTarget");
    action->target = target;

    return ATweenTool;
}


static struct ATweenTool* GetAction(TweenAction** outActionPtr)
{
    CheckAction("GetAction");
    *outActionPtr = action;

    return ATweenTool;
}


//----------------------------------------------------------------------------------------------------------------------


static struct ATweenTool* SetMoveX(float moveX)
{
    return SetValue(ATweenActionValueGetSetImpl->moveX, moveX);
}


static struct ATweenTool* SetMoveY(float moveY)
{
    return SetValue(ATweenActionValueGetSetImpl->moveY, moveY);
}


struct ATweenTool* SetMove2(float moveX, float moveY, bool isRelative, TweenEaseType easeType)
{
    SetValue   (ATweenActionValueGetSetImpl->moveX, moveX);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetValue   (ATweenActionValueGetSetImpl->moveY, moveY);
    SetRelative(isRelative);
    SetEaseType(easeType);

    return ATweenTool;
}


struct ATweenTool* SetMoveSame2(float move, bool isRelative, TweenEaseType easeType)
{
    return SetMove2(move, move, isRelative, easeType);
}


static struct ATweenTool* SetScaleX(float scaleX)
{
    return SetValue(ATweenActionValueGetSetImpl->scaleX, scaleX);
}


static struct ATweenTool* SetScaleY(float scaleY)
{
    return SetValue(ATweenActionValueGetSetImpl->scaleY, scaleY);
}


struct ATweenTool* SetScale2(float scaleX, float scaleY, bool isRelative, TweenEaseType easeType)
{
    SetValue   (ATweenActionValueGetSetImpl->scaleX, scaleX);
    SetRelative(isRelative);
    SetEaseType(easeType);

    SetValue   (ATweenActionValueGetSetImpl->scaleY, scaleY);
    SetRelative(isRelative);
    SetEaseType(easeType);

    return ATweenTool;
}


struct ATweenTool* SetScaleSame2(float scale, bool isRelative, TweenEaseType easeType)
{
    return SetScale2(scale, scale, isRelative, easeType);
}


static struct ATweenTool* SetRotateZ(float rotateZ)
{
    return SetValue(ATweenActionValueGetSetImpl->rotateZ, rotateZ);
}


static struct ATweenTool* SetFadeTo(float fadeTo)
{
    return SetValue(ATweenActionValueGetSetImpl->fadeTo, fadeTo);
}


//----------------------------------------------------------------------------------------------------------------------


static void RunActions(void* target)
{
    ALog_A(target != NULL, "ATweenTool RunActions, target cannot NULL.");

    for (int i = 0; i < actionArr->length; ++i)
    {
        TweenAction* action = AArray_Get(actionArr, i, TweenAction*);

        if (action->actionValueList->size > 0)
        {
            action->target = target;
        }
    }

    ATween->RunActions(actionArr, target);

    actionArr->length = 0;
    action            = NULL;
    actionValue       = NULL;
}


static void* RunTargets()
{
    for (int i = 0; i < actionArr->length; ++i)
    {
        TweenAction* action = AArray_Get(actionArr, i, TweenAction*);

        if (action->actionValueList->size > 0)
        {
            ALog_A
            (
                action->target != NULL,
                "ATweenTool RunTargets, the [%d] action has actionValue, so must set target.",
                i
            );
        }
    }

    void* tweenID = ATween->RunActions(actionArr, NULL);

    actionArr->length = 0;
    action            = NULL;
    actionValue       = NULL;

    return tweenID;
}


struct ATweenTool ATweenTool[1] =
{{
    AddAction,
    AddInterval,

    AddMoveX,
    AddMoveY,
    AddMove2,
    AddMoveSame2,

    AddScaleX,
    AddScaleY,
    AddScale2,
    AddScaleSame2,

    AddRotateZ,
    AddFadeTo,

    SetDuration,
    SetQueue,
    SetOnComplete,
    SetTarget,
    SetUserDataInt,
    SetUserDataFloat,
    SetUserDataPtr,
    SetUserDataString,

    GetAction,

    SetMoveX,
    SetMoveY,
    SetMove2,
    SetMoveSame2,
    SetScaleX,
    SetScaleY,
    SetScale2,
    SetScaleSame2,
    SetRotateZ,
    SetFadeTo,

    SetRelative,
    SetEaseType,

    RunActions,
    RunTargets,
}};


#undef CheckAction
#undef CheckActionValue