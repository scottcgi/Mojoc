/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * Since  : 2017-4-23
 * Author : scott.cgi
 */

#ifndef array_int_set_h
#define array_int_set_h


#include "Engine/Toolkit/Utils/ArrayList.h"


typedef struct
{
    /**
     * Hold all Elements
     */
    ArrayList elementList[1];
}
ArrayIntSet;


struct AArrayIntSet
{

};


extern struct AArrayIntSet AArrayIntSet;


#endif
