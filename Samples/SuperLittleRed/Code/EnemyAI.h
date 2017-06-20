/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2015-4-27
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef ENEMY_AI_H
#define ENEMY_AI_H


#include "Enemy.h"


#define ENEMY_KINDS_COUNT   1
#define KILL_INCREASE_SCALE 10
#define TIME_INCREASE_SCALE 5


struct AEnemyAI
{
    int               currentEnemyNum;
    ArrayList(Enemy*) enemyDeadList[1];
    bool              isInit;

    void (*Init)       ();
    void (*Run)        ();
    void (*CreateCache)(int kind);
    void (*SetAllEnemy)(int enemyState, float enemyMoveDis);
    void (*Restart)    ();
};


extern struct AEnemyAI AEnemyAI[1];


#endif
