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
 * Since    : 2016-12-20
 * Update   : 2019-2-17
 * Author   : scott.cgi
 */


#include <stdio.h>

#include "Engine/Application/Platform/Vibrator.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Application/Input.h"
#include "Engine/Application/Scheduler.h"
#include "Engine/Application/Application.h"
#include "Engine/Extension/Font.h"
#include "Engine/Extension/Spine/SkeletonBone.h"
#include "Engine/Extension/Spine/SkeletonSlot.h"
#include "Engine/Toolkit/Utils/TweenTool.h"
#include "Engine/Toolkit/HeaderUtils/Rect.h"
#include "Engine/Toolkit/Utils/Coroutine.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"
#include "Engine/Graphics/OpenGL/GLTool.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"

#include "UI.h"
#include "Tool.h"
#include "HUD.h"
#include "Hero.h"
#include "EnemyAI.h"
#include "GameMap.h"
#include "GameActor.h"
#include "GameData.h"
#include "AudioTool.h"
#include "ADTool.h"


enum
{
    Fail_Again,
    Fail_Menu,

    Menu_Start,
    Menu_Record,
    Menu_Tutorial,

    HUD_StoneLeft,
    HUD_WoodLeft,
    HUD_StoneRight,
    HUD_WoodRight,

    Record_Close,
    Tutorial_Close,
};

enum
{
    ItemCount_Fail      = 2,
    ItemCount_Menu      = 3,
    ItemCount_HUD       = 4,
    ItemCount_Record    = 1,
    ItemCount_Tutorial  = 1,
    ItemCount_Language  = 4,
};


static Font*                    numFont;
static Font*                    talkFont;
static SkeletonAnimationPlayer* curPlayer         = NULL;
static SkeletonAnimationPlayer* prePlayer         = NULL;

static int                      curSelectBox      = -1;
static Drawable*                curUIDrawable     = NULL;
static FontText*                coinNumText       = NULL;

static ArrayList(FontText*)     uiTextList[1]     = AArrayList_Init(FontText*, 10);


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


static bool inline TestBox(const char* slotName, float x, float y)
{
    SkeletonSlot* boxSlot = ASkeletonAnimationPlayer_GetSlot(curPlayer, slotName);
    float*        box     = (float*) ASkeletonSlot_GetBoundingBox(boxSlot)->vertexArr->data;
    Rect          rect[1];

    ADrawable->ConvertToWorldPositionV2(boxSlot->bone->drawable, (Vector2*)  box,      (Vector2*)           rect     );
    ADrawable->ConvertToWorldPositionV2(boxSlot->bone->drawable, (Vector2*) (box + 4), (Vector2*) ((float*) rect + 2));

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
    for (int i = 0; i < uiTextList->size; ++i)
    {
        AFont->ReleaseText(AArrayList_Get(uiTextList, i, FontText*));
    }

    AArrayList->Clear(uiTextList);

    curPlayer     = NULL;
    curUIDrawable = NULL;
    curSelectBox  = -1;
}


/**
 * If attachmentNames NULL, will use slotName as attachmentName.
 */
static bool inline ClickUI
(
    const char* boneNames[],
    const char* slotNames[],
    const char* attachmentNames[],
    const int   selectBox[],
    int         length,
    InputTouch* touch
)
{
    const char* boneName = NULL;

    for (int i = 0; i < length; ++i)
    {
        // check SubMesh has same name with bone
        const char* name    = boneNames[i];
        SubMesh*    subMesh;

        if (attachmentNames == NULL)
        {
            subMesh = ASkeletonAnimationPlayer_GetSubMesh(curPlayer, name, name);
        }
        else
        {
            subMesh = ASkeletonAnimationPlayer_GetSubMesh(curPlayer, name, attachmentNames[i]);
        }

        if (subMesh != NULL && (ADrawable_CheckVisible(subMesh->drawable) == false))
        {
            continue;
        }

        if (TestBox(slotNames[i], touch->x, touch->y))
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

    ATweenTool->AddScaleSame2(0.0f, 0.5f, false, TweenEaseType_BackOut)
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


static void CostActionOnComplete(TweenAction* action)
{
    AFont->ReleaseText(AUserData_GetSlotPtrWithType(action->userData, 0, FontText*));
}


static inline void AddFontText(char* boneName, int num, bool isTime)
{
    FontText* text = AFont->GetText(numFont);
    AArrayList_Add(uiTextList, text);

    ADrawable_SetParent
    (
        text->drawable,
        ASkeletonAnimationPlayer_GetBone(curPlayer, boneName)->drawable
    );

    if (isTime)
    {
        char buff[10];
        ATool->SetTimeToBuff(buff, num);
        AFont->SetString    (text, buff);
    }
    else
    {
        AFont->SetInt(text, num);
    }

    ADrawable_SetScaleSame2(text->drawable, 1.2f);
    ADrawable_SetPositionX (text->drawable, -text->drawable->width / 2);
}


static void FailCloseOnComplete(TweenAction* action)
{
    CloseUI();

    AGameMap->RandomMap();

    AHUD->CloseCurtain(NULL);
    AEnemyAI->Restart();
    AHero->Revive();

    // reset outside screen
    ASkeleton->ResetBones(AHUD->hudPlayer->skeleton);

    AADTool->Hide(ADType_Banner);
}


static void StartRunGame(Coroutine* coroutine)
{
    ACoroutine_Begin();

    AGameMap->Run();
    ACoroutine_YieldSeconds(0.2f);
    AHUD->CloseCurtain(NULL);
    AHero->Run();
    AGameActor->Run();
    AEnemyAI->Run();

    ACoroutine_End();
}


static void MenuCloseOnComplete(TweenAction* action)
{
    CloseUI();

    static bool isFirst = true;

    if (isFirst)
    {
        isFirst = false;
        ACoroutine->StartCoroutine(StartRunGame);
    }
    else
    {
        FailCloseOnComplete(NULL);
    }
}


static void StoreCloseOnComplete(TweenAction* action)
{
    curUIDrawable = NULL;
    curSelectBox  = -1;
    curPlayer     = prePlayer;
    prePlayer     = NULL;

    AFont->ReleaseText(coinNumText);
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


static void ShowTutorial(TweenActionOnComplete OnComplete)
{
    prePlayer = curPlayer;
    ShowUI(AUI->uiTutorialPlayer, TweenEaseType_BackInElasticOut, OnComplete);
}


static void ShowAD(TweenAction* action)
{
    const int num = 3;

    if (AHero->roundCount > num - 1 && AHero->roundCount % num == 0)
    {
        AADTool->Show(ADType_Interstitial);
    }
    else
    {
        AADTool->Show(ADType_Banner);
    }
}


static void ShowTutorialScheduler(Scheduler* scheduler, float deltaSeconds)
{
    ShowTutorial(ShowAD);
}


static void ShowFail()
{
    if (AHero->roundCount > AGameData->maxRoundCount)
    {
        AGameData->maxRoundCount = AHero->roundCount;
    }

    if (AHero->roundTime > AGameData->maxRoundTime)
    {
        AGameData->maxRoundTime = AHero->roundTime;
    }

    if (AHero->roundKillCount > AGameData->maxRoundKillCount)
    {
        AGameData->maxRoundKillCount = AHero->roundKillCount;
    }

    if (AGameData->maxScore < AHero->roundScore)
    {
        AGameData->maxScore = AHero->roundScore;
    }

    const char* names[] =
    {
        "Score",
        "Kill",
        "Hit",
        "Time",
    };

    int values[] =
    {
        AHero->roundScore,
        AHero->roundKillCount,
        AHero->roundArrowCount == 0 ? 0 :
        (AHero->roundArrowCount - AHero->roundMissCount) * 100 / AHero->roundArrowCount,
    };

    for (int i = 0; i < 4; ++i)
    {
        Drawable* posDrawable = ASkeletonAnimationPlayer_GetBone(AUI->uiFailPlayer, names[i])->drawable;
        FontText* text;

        text = AFont->GetText(numFont);
        ADrawable_SetParent  (text->drawable, posDrawable);
        AArrayList_Add       (uiTextList,     text);

        switch (i)
        {
            case 2:
            {
                char buff[6];
                sprintf(buff, "%d%%", values[i]);
                AFont->SetString(text, buff);
                break;
            }

            case 3:
            {
                char buff[10];
                ATool->SetTimeToBuff(buff, (int) AHero->roundTime);
                AFont->SetString    (text, buff);
                break;
            }

            default:
                AFont->SetInt(text, values[i]);
                break;
        }
    }

    if (AHero->roundKillCount < 2)
    {
        AScheduler->ScheduleOnce(ShowTutorialScheduler, 0.5f);
        ShowUI(AUI->uiFailPlayer, TweenEaseType_BackOut, NULL);
    }
    else
    {
        ShowUI(AUI->uiFailPlayer, TweenEaseType_BackOut, ShowAD);
    }
}


static void ShowMenu()
{
    ShowUI(AUI->uiMenuPlayer, TweenEaseType_BackOut, NULL);
}


static void ShowRecord()
{
    prePlayer = curPlayer;
    ShowUI(AUI->uiRecordPlayer, TweenEaseType_BounceOut, NULL);

    AddFontText("MaxScore",           AGameData->maxScore,          false);
    AddFontText("MaxKill",            AGameData->maxRoundKillCount, false);
    AddFontText("MaxRound",           AGameData->maxRoundCount,     false);
    AddFontText("MaxRoundTime", (int) AGameData->maxRoundTime,      true);
    AddFontText("PlayGameTime",       AGameData->playGameCount,     false);
}


//----------------------------------------------------------------------------------------------------------------------


static bool OnMessage(Component* component, void* sender, int subject, void* extraData)
{
    if (sender == AApplication)
    {
        InputTouch* touch = AArray_Get((Array(InputTouch*)*) extraData, 0, InputTouch*);

        if (touch->fingerID > 0 && curPlayer == NULL)
        {
            if (touch->type == InputTouchType_Down)
            {
                // hud btn support multitouch
                goto HUDTouch;
            }
        }

        switch (touch->type)
        {
            case InputTouchType_Cancel:
                break;
            
            case InputTouchType_Move:
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
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case Fail_Menu:
                            CloseUITween(NULL);
                            ShowMenu();
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case Menu_Start:
                            CloseUITween(MenuCloseOnComplete);
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case Menu_Record:
                            ShowRecord();
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case Menu_Tutorial:
                            ShowTutorial(NULL);
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case HUD_StoneLeft:
                        case HUD_StoneRight:
                            if (AGameData->stone > 0)
                            {
                                AHUD->DropStone();
                                AFont->SetInt(AHUD->stoneText, --AGameData->stone);
                                AAudioTool->Play(AudioID_ClickBtn);
                                AVibrator->Vibrate(100);
                            }
                            else
                            {
                                ATool->ShakeX(curUIDrawable);
                            }

                            curUIDrawable = NULL;
                            curSelectBox  = -1;
                            break;

                        case HUD_WoodLeft:
                        case HUD_WoodRight:
                            if (AGameData->wood > 0)
                            {
                                AHUD->DropWood();
                                AFont->SetInt(AHUD->woodText, --AGameData->wood);
                                AAudioTool->Play(AudioID_ClickBtn);
                                AVibrator->Vibrate(100);
                            }
                            else
                            {
                                ATool->ShakeX(curUIDrawable);
                            }

                            curUIDrawable = NULL;
                            curSelectBox  = -1;
                            break;

                        case Record_Close:
                            CloseUITween(RecordCloseOnComplete);
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        case Tutorial_Close:
                            CloseUITween(TutorialCloseOnComplete);
                            AAudioTool->Play(AudioID_ClickBtn);
                            break;

                        default:
                            break;
                    }

                    return true;
                }
                break;

            case InputTouchType_Down:
                if (curPlayer == AUI->uiFailPlayer)
                {
                    ClickUI
                    (
                        (const char*[ItemCount_Fail])
                        {
                            "Again",
                            "Menu",
                        },

                        (const char*[ItemCount_Fail])
                        {
                            "AgainBox",
                            "MenuBox",
                        },

                        NULL,

                        (int[ItemCount_Fail])
                        {
                            Fail_Again,
                            Fail_Menu,
                        },

                        ItemCount_Fail,
                        touch
                    );
                }
                else if (curPlayer == AUI->uiMenuPlayer)
                {
                    ClickUI
                    (
                        (const char*[ItemCount_Menu])
                        {
                            "Start",
                            "Record",
                            "Tutorial",
                        },

                        (const char*[ItemCount_Menu])
                        {
                            "StartBox",
                            "RecordBox",
                            "TutorialBox",
                        },

                        NULL,
                        
                        (int[ItemCount_Menu])
                        {
                            Menu_Start,
                            Menu_Record,
                            Menu_Tutorial,
                        },

                        ItemCount_Menu,
                        touch
                    );
                }
                else if (curPlayer == AUI->uiRecordPlayer)
                {
                    ClickUI
                    (
                        (const char*[ItemCount_Record])
                        {
                            "Close",
                        },

                        (const char*[ItemCount_Record])
                        {
                            "CloseBox",
                        },

                        NULL,

                        (int[ItemCount_Record])
                        {
                            Record_Close,
                        },
                        
                        ItemCount_Record,
                        touch
                    );
                }
                else if (curPlayer == AUI->uiTutorialPlayer)
                {
                    ClickUI
                    (
                        (const char*[ItemCount_Tutorial])
                        {
                            "Close",
                        },

                        (const char*[ItemCount_Tutorial])
                        {
                            "CloseBox",
                        },

                        NULL,

                        (int[ItemCount_Tutorial])
                        {
                            Tutorial_Close,
                        },

                        ItemCount_Tutorial,
                        touch
                    );
                }
                else // click on HUD
                {
                    HUDTouch:

                    curPlayer = AHUD->hudPlayer;

                    if
                    (
                        ClickUI
                        (
                            (const char*[ItemCount_HUD])
                            {
                                "StoneLeft",
                                "WoodLeft",
                                "StoneRight",
                                "WoodRight",
                            },

                            (const char*[ItemCount_HUD])
                            {
                                "StoneBoxLeft",
                                "WoodBoxLeft",
                                "StoneBoxRight",
                                "WoodBoxRight",
                            },

                            (const char*[ItemCount_HUD])
                            {
                                "Stone",
                                "Wood",
                                "Stone",
                                "Wood",
                            },

                            (int[ItemCount_HUD])
                            {
                                HUD_StoneLeft,
                                HUD_WoodLeft,
                                HUD_StoneRight,
                                HUD_WoodRight,
                            },
                            
                            ItemCount_HUD,
                            touch
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

    SkeletonAnimationPlayer* uiSkeletonPlayers[ItemCount_Language] =
    {
        AUI->uiMenuPlayer,
        AUI->uiFailPlayer,
        AUI->uiTutorialPlayer,
        AUI->uiRecordPlayer,
    };

    const char* uiSkeletonPaths[ItemCount_Language] =
    {
        "UI/UIMenu",
        "UI/UIFail",
        "UI/UITutorial",
        "UI/UIRecord",
    };

    const char* uiSkeletonPathsZH[ItemCount_Language] =
    {
        "UI/UIMenu-zh",
        "UI/UIFail-zh",
        "UI/UITutorial-zh",
        "UI/UIRecord-zh",
    };

    for (int i = 0; i < ItemCount_Language; ++i)
    {
        if (ATool->languageCode == LanguageCode_zh)
        {
            ASkeletonAnimationPlayer->Init(uiSkeletonPathsZH[i], "animation", uiSkeletonPlayers[i]);

        }
        else if (ATool->languageCode == LanguageCode_en)
        {
            ASkeletonAnimationPlayer->Init(uiSkeletonPaths[i], "animation", uiSkeletonPlayers[i]);
        }
    }

    numFont   = AFont->Get("Font/UINumber.atlas");
    talkFont  = AFont->Get("Font/TalkChar.atlas");
}


static void Run()
{
    AApplication_AddChild(AUI->component, 110);
}


struct AUI AUI[1] =
{{
    .Init     = Init,
    .Run      = Run,
    .ShowFail = ShowFail,
    .ShowMenu = ShowMenu,
}};


struct AUIStore AUIStore[1] =
{{
    .weaponNames =
    {
        "Copper",
        "Dragon",
        "Gold",
        "Heart",
        "Ice",
        "Iron",
        "Lightning",
        "Silver",
        "Steel",
        "Wood",
    },

    .weaponBoxes =
    {
        "CopperBox",
        "DragonBox",
        "GoldBox",
        "HeartBox",
        "IceBox",
        "IronBox",
        "LightningBox",
        "SilverBox",
        "SteelBox",
        "WoodBox",
    },

    .weaponIcons =
    {
        "CopperIcon",
        "DragonIcon",
        "GoldIcon",
        "HeartIcon",
        "IceIcon",
        "IronIcon",
        "LightningIcon",
        "SilverIcon",
        "SteelIcon",
        "WoodIcon",
    },

    .weaponLocks =
    {
        "CopperLock",
        "DragonLock",
        "GoldLock",
        "HeartLock",
        "IceLock",
        "IronLock",
        "LightningLock",
        "SilverLock",
        "SteelLock",
        "WoodLock",
    },

    .dressNames =
    {
        "Armour",
        "RedGirl",
        "Bunny",
        "Egypt",
        "Luffy",
        "Swan",
    },

    .dressBoxes =
    {
        "ArmourBox",
        "RedGirlBox",
        "BunnyBox",
        "EgyptBox",
        "LuffyBox",
        "SwanBox",
    },

    .dressIcons =
    {
        "ArmourIcon",
        "RedGirlIcon",
        "BunnyIcon",
        "EgyptIcon",
        "LuffyIcon",
        "SwanIcon",
    },

    .dressLocks =
    {
        "ArmourLock",
        "RedGirlLock",
        "BunnyLock",
        "EgyptLock",
        "LuffyLock",
        "SwanLock",
    },

    .propertyNames =
    {
        "Speed",
        "HP",
        "Energy",
        "EnergyRecovery",
        "DizzyDistance",
        "DizzyTime",
        "StonePower",
        "WoodTime",
        "ArrowSpeed",
        "ArrowPower",
    },

    .propertyBoxes =
    {
        "SpeedBox",
        "HPBox",
        "EnergyBox",
        "EnergyRecoveryBox",
        "DizzyDistanceBox",
        "DizzyTimeBox",
        "StonePowerBox",
        "WoodTimeBox",
        "ArrowSpeedBox",
        "ArrowPowerBox",
    },
}};
