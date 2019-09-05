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
 * Since    : 2015-4-27
 * Update   : 2019-2-16
 * Author   : scott.cgi
 */


#ifndef ENEMY_AI_H
#define ENEMY_AI_H


#include "Enemy.h"


struct AEnemyAI
{
    int               currentEnemyNum;
    ArrayList(Enemy*) enemyDeadList[1];
    bool              isInit;
    bool              isHasBoss;

    void (*Init)       (void);
    void (*Run)        (void);
    void (*CreateCache)(int kind);
    void (*SetAllEnemy)(int enemyStateID, float enemyMoveDis);
    void (*Restart)    (void);
};


extern struct AEnemyAI AEnemyAI[1];


#endif
