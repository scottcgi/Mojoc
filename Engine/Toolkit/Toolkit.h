/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since  : 2014-9-8
 * Author : scott.cgi
 * Version: 0.0.0
 */


#ifndef TOOLKIT_H
#define TOOLKIT_H


struct AToolkit
{
    void (*Init)();
};


extern struct AToolkit AToolkit[1];


#endif 
