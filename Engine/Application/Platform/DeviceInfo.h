/*
 * Copyright (c) 2012-2018 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Since : 2018-12-6
 * Author: scott.cgi
 */


#ifndef DEVICE_INFO_H
#define  DEVICE_INFO_H


#include <stdbool.h>


struct ADeviceInfo
{
    /**
     * The outLanguageCode will be filled with an array of two characters.
     */
    void (*GetLanguageCode)(char outLanguageCode[2]);
};


extern struct ADeviceInfo ADeviceInfo[1];


#endif
