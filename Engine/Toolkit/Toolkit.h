/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code and its project Mojoc are licensed under [the MIT License],
 * and the project Mojoc is a game engine hosted on github at [https://github.com/scottcgi/Mojoc],
 * and the author's personal website is [https://scottcgi.github.io],
 * and the author's email is [scott.cgi@qq.com].
 *
 * Since : 2014-9-8
 * Update: 2019-1-8
 * Author: scott.cgi
 */
 

#ifndef TOOLKIT_H
#define  TOOLKIT_H


/**
 * Manage Toolkit module.
 */
struct AToolkit
{
    void (*Init)();
};


extern struct AToolkit AToolkit[1];


#endif 
