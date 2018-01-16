/*
 * Copyright (c) 2017-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2016-12-20
 * Author: scott.cgi
 */

#include <stdio.h>

#include "Engine/Application/Input.h"
#include "Engine/Toolkit/Head/UserData.h"
#include "Engine/Extension/Font.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Extension/Font.h"
#include "Engine/Application/Input.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Toolkit/Head/Rect.h"
#include "Engine/Application/Application.h"
#include "Engine/Graphics/OpenGL/GLTool.h"

#include "UI.h"
#include "Tool.h"
#include "HUD.h"
#include "Hero.h"
#include "EnemyAI.h"
#include "GameMap.h"
#include "GameActor.h"
#include "GameData.h"
#include "AudioTool.h"


enum
{
    Fail_Again,

    HUD_Stone,
    HUD_Wood,

    Record_Close,
    Tutorial_Close,
};


//----------------------------------------------------------------------------------------------------------------------


static Font*                    numFont;
static Font*                    talkFont;

static SkeletonAnimationPlayer* curPlayer       = NULL;
static SkeletonAnimationPlayer* prePlayer       = NULL;


//----------------------------------------------------------------------------------------------------------------------


static int                      curSelectBox    = -1;
static Drawable*                curUIDrawable   = NULL;
static ArrayList(FontText*)     uiTextList[1]   = AArrayList_Init(sizeof(FontText*), 10);


//----------------------------------------------------------------------------------------------------------------------


static void Update(Component* component, float deltaSeconds)
{
    if (prePlayer != NULL)
    {
        ASkeletonAnimationPlayer_Update(prePlayer, deltaSeconds);
    }

    if (curPlayer != NULL)
    {
        ASkeletonAnimationPlayer_Update(curPlayer, deltaSeconds);
    }

    AFont->Draw(numFont);
    AFont->Draw(talkFont);
}


static bool inline TestBox(char* slotName, float x, float y)
{
    SkeletonSlot* boxSlot = ASkeletonAnimationPlayer_GetSlot(curPlayer, slotName);
    float*        box     = (float*) ASkeletonSlot_GetBoundingBox(boxSlot)->vertexArr->data;
    Rect          rect[1];

    ADrawable->ConvertToWorldPoint(boxSlot->bone->drawable, (Vector2*)  box,      (Vector2*)          rect     );
    ADrawable->ConvertToWorldPoint(boxSlot->bone->drawable, (Vector2*) (box + 4), (Vector2*)((float*) rect + 2));

    return ARect_TestPoint(rect, x, y);
}


static void inline ShowUI(SkeletonAnimationPlayer* player, TweenEaseType easeType, TweenActionOnComplete OnComplete)
{
    curPlayer = player;

    ADrawable_SetScale2
    (
        ASkeletonAnimationPlayer_GetDrawable(curPlayer),
        ATool->globalScaleX,
        ATool->globalScaleY
    );

    Drawable* drawable = ASkeletonAnimationPlayer_GetDrawable(curPlayer);

    ADrawable_SetPositionY
    (
        drawable,
        1.0f + ASkeletonAnimationPlayer_GetHeight(curPlayer) * drawable->scaleY / 2
    );

    ATweenTool->AddMoveY     (0.0f, GOLDEN_RATIO)
              ->SetEaseType  (easeType)
              ->SetRelative  (false)
              ->SetOnComplete(OnComplete)
              ->RunActions   (drawable);

    if (prePlayer != NULL)
    {
        ATweenTool->AddFadeTo  (0.0f, 0.25f)
                  ->SetEaseType(TweenEaseType_SineInOut)
                  ->SetRelative(false)
                  ->RunActions (ASkeletonAnimationPlayer_GetDrawable(prePlayer));
    }
}


static void inline CloseUI()
{
    for (int i = 0; i < uiTextList->size; i++)
    {
        AFont->ReuseText
        (
            AArrayList_Get(uiTextList, i, FontText*)
        );
    }

    AArrayList->Clear(uiTextList);

    curPlayer     = NULL;
    curUIDrawable = NULL;
    curSelectBox  = -1;
}


static bool inline ClickUI(char* boneNames[], char* boxNames[], int selectBox[], int length, InputTouch* touch)
{
    char* boneName = NULL;

    for (int i = 0; i < length; i++)
    {
        // check SubMesh has same name with bone
        char*    name    = boneNames[i];
        SubMesh* subMesh = ASkeletonAnimationPlayer_GetSubMesh(curPlayer, name, name);

        if (subMesh != NULL && (ADrawable_CheckVisible(subMesh->drawable) == false))
        {
            continue;
        }

        if (TestBox(boxNames[i], touch->x, touch->y))
        {
            boneName     = name;
            curSelectBox = selectBox[i];
            break;
        }
    }

    if (boneName == NULL)
    {
        curSelectBox  = -1;
        curUIDrawable = NULL;

        return false;
    }
    else
    {
        curUIDrawable = ASkeletonAnimationPlayer_GetBone(curPlayer, boneName)->drawable;
        ATweenTool->AddScaleSame2(1.11f, 0.1f, false, TweenEaseType_SineInOut)
                  ->RunActions   (curUIDrawable);

        return true;
    }
}


static void inline CloseUITween(TweenActionOnComplete OnComplete)
{
    Drawable* drawable = ASkeletonAnimationPlayer_GetDrawable(curPlayer);

    ATweenTool->AddScaleSame2
                (
                    0.0f,
                    0.5f,
                    false,
                    TweenEaseType_BackOut
                )
              ->SetOnComplete(OnComplete)
              ->RunActions   (drawable);

    if (OnComplete == NULL)
    {
        CloseUI();
    }

    if (prePlayer != NULL)
    {
        ATweenTool->AddFadeTo  (1.0f, GOLDEN_RATIO)
                  ->SetRelative(false)
                  ->SetEaseType(TweenEaseType_SineInOut)
                  ->RunActions (ASkeletonAnimationPlayer_GetDrawable(prePlayer));
    }
}


//----------------------------------------------------------------------------------------------------------------------


static void FailCloseOnComplete(TweenAction* action)
{
    CloseUI();

    AGameMap->RandomMap();

    AHUD->CloseCurtain(NULL);
    AEnemyAI->Restart();
    AHero->Revive();

    // reset out screen
    ASkeleton->ResetBones(AHUD->hudPlayer->skeleton);
}


static void RecordCloseOnComplete(TweenAction* action)
{
    curPlayer     = NULL;
    curUIDrawable = NULL;
    curSelectBox  = -1;
    curPlayer     = prePlayer;
    prePlayer     = NULL;
}


static void TutorialCloseOnComplete(TweenAction* action)
{
    curPlayer     = NULL;
    curUIDrawable = NULL;
    curSelectBox  = -1;
    curPlayer     = prePlayer;
    prePlayer     = NULL;
}


//----------------------------------------------------------------------------------------------------------------------


static void ShowFail()
{
    int score = AHero->roundKillCount == 0 ? AHero->roundTime / 20 : AHero->roundKillCount * AHero->roundTime / 25;

    char* names[] =
    {
        "Score",
        "Kill",
        "Hit",
        "Time",
    };

    int  values[] =
    {
        score,
        AHero->roundKillCount,
        AHero->roundArrowCount == 0 ? 0 :
        (AHero->roundArrowCount - AHero->roundMissCount) * 100 / AHero->roundArrowCount,
    };

    for (int i = 0; i < 4; i++)
    {
        Drawable* posDrawable = ASkeletonAnimationPlayer_GetBone(AUI->uiFailPlayer, names[i])->drawable;
        FontText* text;

        text = AFont->GetText(numFont);

        ADrawable_SetParent(text->drawable, posDrawable);
        AArrayList_Add     (uiTextList,     text);

        if (i == 2)
        {
            char buff[6];
            sprintf(buff, "%d%%", values[i]);
            AFont->SetString(text, buff);
        }
        else if (i == 3)
        {
            char buff[10];
            ATool->SetTimeToBuff(buff, AHero->roundTime);
            AFont->SetString    (text, buff);
        }
        else
        {
            AFont->SetInt(text, values[i]);
        }
    }

    ShowUI(AUI->uiFailPlayer, TweenEaseType_BackOut, NULL);
}


//----------------------------------------------------------------------------------------------------------------------


static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (sender == AApplication)
    {
        InputTouch* touch = AArray_Get((Array*) extraData, 0, InputTouch*);

        if (touch->fingerId > 0 && curPlayer == NULL)
        {
            if (touch->type == InputTouchType_Down)
            {
                // hud btn support multitouch
                goto HUDTouch;
            }
        }

//----------------------------------------------------------------------------------------------------------------------

        switch (touch->type)
        {
            case InputTouchType_Move:
                break;
                
            case InputTouchType_Cancel:
                break;
                
            case InputTouchType_Up:
                if (curUIDrawable != NULL)
                {
                    ATweenTool->AddScaleSame2(1.0f, 0.1f, false, TweenEaseType_SineInOut)
                              ->RunActions   (curUIDrawable);

                    switch (curSelectBox)
                    {
                        case Fail_Again:
                            CloseUITween(FailCloseOnComplete);
                            AAudioTool->Play(AudioId_ClickBtn);
                            break;

//----------------------------------------------------------------------------------------------------------------------

                        case HUD_Stone:
                            if (AGameData->stone > 0)
                            {
                                AHUD->DropStone();
                                AFont->SetInt(AHUD->stoneText, --AGameData->stone);
                                AAudioTool->Play(AudioId_ClickBtn);
                            }
                            else
                            {
                                ATool->ShakeX(curUIDrawable);
                            }

                            curUIDrawable = NULL;
                            curSelectBox  = -1;
                            break;

                        case HUD_Wood:
                            if (AGameData->wood > 0)
                            {
                                AHUD->DropWood();
                                AFont->SetInt(AHUD->woodText, --AGameData->wood);
                                AAudioTool->Play(AudioId_ClickBtn);
                            }
                            else
                            {
                                ATool->ShakeX(curUIDrawable);
                            }

                            curUIDrawable = NULL;
                            curSelectBox  = -1;
                            break;

//----------------------------------------------------------------------------------------------------------------------

                        case Record_Close:
                            CloseUITween(RecordCloseOnComplete);
                            AAudioTool->Play(AudioId_ClickBtn);
                            break;

                        case Tutorial_Close:
                            CloseUITween(TutorialCloseOnComplete);
                            AAudioTool->Play(AudioId_ClickBtn);
                            break;
                    }

                    return true;
                }
                break;

//----------------------------------------------------------------------------------------------------------------------

            case InputTouchType_Down:
                if (curPlayer == AUI->uiFailPlayer)
                {
                    ClickUI
                    (
                        (char*[])
                        {
                            "Again",
                        },

                        (char*[])
                        {
                            "AgainBox",
                        },

                        (int[])
                        {
                            Fail_Again,
                        },
                        1, touch
                    );
                }
                else
                {

                    HUDTouch:  // click on HUD

                    curPlayer = AHUD->hudPlayer;

                    if
                    (
                        ClickUI
                        (
                            (char*[])
                            {
                                "Stone",
                                "Wood",
                            },

                            (char*[])
                            {
                                "StoneBox",
                                "WoodBox",
                            },

                            (int[])
                            {
                                HUD_Stone,
                                HUD_Wood,
                            },
                            2, touch
                        )
                    )
                    {
                        return true;
                    }

                    curPlayer = NULL;
                }

                if (curPlayer != NULL)
                {
                    return true;
                }

                break;
        }
    }

    return false;
}


//----------------------------------------------------------------------------------------------------------------------


static void Init()
{
    AComponent->Init(AUI->component);
    AUI->component->curState->Update    = Update;
    AUI->component->curState->OnMessage = OnMessage;

    ASkeletonAnimationPlayer->Init("UI/UIFail", "animation", AUI->uiFailPlayer);

    numFont  = AFont->Get("Font/UINumber.atlas");
    talkFont = AFont->Get("Font/TalkChar.atlas");
}


static void Run()
{
    AApplication_AddChild(AUI->component, 110);
}


struct AUI AUI[1] =
{
    {
        .Init     = Init,
        .Run      = Run,
        .ShowFail = ShowFail,
    }
};
