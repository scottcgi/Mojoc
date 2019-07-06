/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License.
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub.
 * This Mojoc game engine is a serious project with extreme code pursuit, and will continue to iterate.
 *
 * License: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub : https://github.com/scottcgi/Mojoc
 *
 * Since  : 2014-9-8
 * Update : 2019-1-8
 * Author : scott.cgi
 */
 

#ifndef TOOLKIT_H
#define TOOLKIT_H


/**
 * Manage Toolkit module.
 */
struct AToolkit
{
    void (*Init)(void);
};


extern struct AToolkit AToolkit[1];


#endif 
