/*
 * Def.h
 *
 *  Created on: 2016-10-2
 *      Author: scott.cgi
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
 * Marked variable can only get value
 *
 * the variable can not set directly
 * use function or macro to instead of set value
 *
 * the marker effect use data structure variable
 * if construct data structure variable not follow the rule
 */
#define get_only


/**
 * Marked variable can only set value or call set method
 *
   the variable can not get directly
 * use function or macro to instead of get value
 *
 * the marker effect use data structure variable
 * if construct data structure variable not follow the rule
 */
#define set_only


/**
 * Marked function argument for get value out
 */
#define out_param


/**
 * Marked variable must set, or may cause crash, or running incorrectly
 */
#define set_required


#endif
