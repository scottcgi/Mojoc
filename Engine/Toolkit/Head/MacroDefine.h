/*
 * Copyright (C) scott.cgi All Rights Reserved.
 *
 * Since  : 2016-10-2
 * Author : scott.cgi
 */

#ifndef marco_define_h
#define marco_define_h


/**
 * The pointer point NULL
 */
#define null_ptr (void*[]) {NULL}


/**
 * Make str to string
 */
#define Stringizing(str) #str


/**
 * Marked variable can only get value or call get method
 *
 * use function or macro to instead of set value directly
 * if variable is struct object (not ptr) each property value also get_only
 * if construct struct object variable, the property not follow the rule
 */
#define get_only


/**
 * Marked function argument for get value out
 */
#define out_param


/**
 * Marked variable must set, or may cause crash, or running incorrectly
 */
#define set_required


#endif
