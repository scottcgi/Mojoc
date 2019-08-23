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
 * Since    : 2017-2-25
 * Update   : 2019-7-31
 * Author   : scott.cgi
 */


#ifndef AD_TOOL_H
#define AD_TOOL_H


/**
 * The advertisement type.
 */
typedef enum
{
    ADType_Banner,
    ADType_Interstitial,
}
ADType;


/**
 * Control and manage AD.
 */
struct AADTool
{
    void (*Show)(ADType type);
    void (*Hide)(ADType type);
};


extern struct AADTool AADTool[1];


#endif
