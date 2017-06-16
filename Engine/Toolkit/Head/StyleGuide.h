/*
 * Copyright (c) 2012-2017 scott.cgi All Rights Reserved.
 *
 * Since  : 2013-1-26
 * Author : scott.cgi
 * Version: 0.1
 */


#ifndef STYLE_GUIDE_H
#define STYLE_GUIDE_H


/*
----------------------------------------------------------------------------------------------------



  The macro without parameter lower case words separated by '_'.



  The define head macro lower case words separated by '_' and end with 'h'

//--------------------------------------------------------------------------------------------------



  The function pointer or function name first word upper case.

  The struct first word upper case.

  The file folder name first word upper case.

  The macro with parameter first word upper case.

//--------------------------------------------------------------------------------------------------

   The macro use for inline function not working.

   such as shortcut parameters of function call.

   such as macro specific functionality.

   such as generic parameter.

   such as varargs.

   such as type parameter.

   The private macro begin with '_'

//--------------------------------------------------------------------------------------------------


  The function return boolean which show operation result named with prefix 'Try'.

  The function return boolean which show result named with prefix 'Is'.


//--------------------------------------------------------------------------------------------------

  In function body code comment use '//', struct property and function define use '/**'.

  The "{" and "}" in separate line.

  Try regional vertical alignment.

//--------------------------------------------------------------------------------------------------

  Method "Release" means free struct member memory space.

  Method with prefix "Create" means return malloc pointer, so first release and then free.

  Method with prefix "Init" or others means pointer on stack, so just release for delete.

//--------------------------------------------------------------------------------------------------

  The non-boolean value use conditional expressions in conditional judgments.

  Use this '/------------------------' for split line.

//--------------------------------------------------------------------------------------------------

  Empty two lines between method and block comment and type define.
  Empty one line  between struct property and split line

//--------------------------------------------------------------------------------------------------

   The 'const' for variables is not modified in both user and implementor.

   Use tab key indent which 4 spaces

   The 'break' tab indent in switch case

----------------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------------
 This is means comment few blocks of code
----------------------------------------------------------------------------------------------------
*/


#endif
