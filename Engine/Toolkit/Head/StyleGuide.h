/*
 *
 *
 *  Created on: 2013-1-26
 *  Author: scott.cgi
 */

#ifndef style_guide_h
#define style_guide_h

/*
----------------------------------------------------------------------------------------------------

  The name use CamelCased.

  The variable first word lower case.

  The global function and struct's function first word upper case.

  The struct first word upper case.

  The file folder name first word upper case.

  The function pointer or function name first word upper case.

  The macro with parameter bracket first word upper case.

  The macro without parameter bracket lower case words separated by '_'.

  The const and enum lower case words separated by '_'.

  The goto label lower case words separated by '_'


//--------------------------------------------------------------------------------------------------


   The macro use for inline function not working.
   such as shortcut parameters of function call.
   such as macro specific functionality.
   such as generic parameter.
   such as varargs.
   such as type parameter.


//--------------------------------------------------------------------------------------------------


  The bool variable begin with prefix 'Is'.

  The singleton begin with prefix 'A'.

  The function return boolean show operation result named with prefix 'Try'.


//--------------------------------------------------------------------------------------------------


  In function body code comment use '//', all other like struct property, function define use '/**'.

  The "{" and "}" in separate line.

  The 'break' in separate line, not in same line with '{' or '}'.

  Try regional vertical alignment.


//--------------------------------------------------------------------------------------------------

  Method "Release" means delete struct member memory space.

  Method with prefix "Create" means return malloc pointer, so first release and then free for delete.

  Method with prefix "Init" or others means pointer on stack, so just release for delete.

//--------------------------------------------------------------------------------------------------


  If comment between
  /*
  ----------------------------------------------------------------------------------------------------
  ----------------------------------------------------------------------------------------------------
  */
  // means comment few blocks of code

/*

  Use
  /---------------------------------------------------------------------------------------------------
  for split line

non-boolean value use conditional expressions in conditional judgments

In head file that the macro of conditional inclusion use lower case words separated by '_' with suffix '_h'

----------------------------------------------------------------------------------------------------
*/

#endif
