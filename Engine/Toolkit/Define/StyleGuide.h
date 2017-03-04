/*
 *
 *
 *  Created on: 2013-1-26
 *  Author: scott.cgi
 */

#ifndef style_guid_h
#define style_guid_h

/**
 *  1.  Use camel named
 *
 *  2.  The variable first word lower case
 *
 *  3.  The global function and struct's function first word upper case
 *
 *  4.  The struct first word upper case
 *
 *  5.  The file folder name first word upper case
 *
 *  6.  The const and enum lower case words separated by '_'
 *
 *  7.  The macro define statement first word upper case with prefix struct 'AType_'
 *
 *  8.  The macro just for semantic marker first word upper case
 *
 *  9.  All other macro define lower case words separated by '_'
 *
 *  10. All bool variable begin with prefix 'Is'
 *
 *  11. Singleton begin with prefix 'A'
 *
 *  12. In function body code comment use '//'
 *
 *  13. All other like struct property, function define use '/**'
 *
 *  14. "{" and "}" in separate line
 *
 *  15. Method "Release" means delete struct member memory space
 *
 *  16. Method with prefix "create" means return malloc pointer, so first release and then free for delete
 *
 *  17. Method with prefix "init" or others means pointer on stack, so just release for delete
 *
 *  18. The macro sometimes give convenient but probably make code bloat
 *      the less macro force use more temporary variable sometimes more efficient
 *      so we minimal use macro
 *
 *  19. The object-oriented language can easily visit inheritance parent method
 *      also parent's parent's and so on. But in c not has this ability, inherit
 *      function in c must use struct visit one by one. I found this the good side
 *      is make design less inheritance hierarchy, for easy use and remember
 *      parent's function
 *
 *  20. If inheritance one type, we can named end with this type
 *
 *  21. If one type has relationship with other type, we can named end with other type
 *
 *  22. UserData not use in inheritance, we use userData for extra parameter
 *
 *  23. Function return boolean show operation result named with prefix "try"
 *
 *  24. The 'break' in separate line, not in same line with '{' or '}'
 *
 *  25. If struct has other struct's pointer, this is composite, other struct's pointer can NULL
 *
 *  26. If struct has other struct data, this is inheritance, other struct data must be initialized
 *
 *  27. Try regional vertical alignment
 *
 *  28. Goto label lower case words separated by '_'
 *
 *  29. The function arguments variable named with prefix 'out',  means out value variable
 *
 *  30. The struct function means interface not implementation, user must implement it
 *
 *  31. The Singleton struct function means can direct use it
 *
 *  32. The macro use for inline function not working
 *      such as shortcut parameters of function call
 *      such as macro specific functionality
 *      such as generic parameter
 *      such as varargs
 *      such as type parameter
 *
 *  34. function pointer or function name first word upper case
 *
 *  35. if comment between
 *      '--------------------------------------------------------------------------------------------------'
 *      means comment few blocks of code
 *
 *  36. Use
 *      '//--------------------------------------------------------------------------------------------------'
 *      for split line
 *
 *  37. use
 *      '/*--------------------------------------------------------------------------------------------------'
 *      for code block comment
 *
 *  38. non-boolean value use conditional expressions in conditional judgments
 *
 *  39. In head file that the macro of conditional inclusion use lower case words separated by '_' with suffix '_h'
 */

#endif
