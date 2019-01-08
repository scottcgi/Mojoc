/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2014-9-8
 * Update: 2019-1-8
 * Author: scott.cgi
 */
 

#ifndef TOOLKIT_H
#define  TOOLKIT_H


struct AToolkit
{
    void (*Init)();
};


extern struct AToolkit AToolkit[1];


#endif 
