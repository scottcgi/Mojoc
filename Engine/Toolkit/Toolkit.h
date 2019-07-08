/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 *
 * The Mojoc Game Engine is a serious project with extreme code pursuit,
 * and will continue to iterate,
 * and licensed under the MIT License.
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
