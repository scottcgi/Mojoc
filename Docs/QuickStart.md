## Three Steps To Quick Start

#### Step 1.
Get the source code, have three options.

* Git from <https://github.com/scottcgi/Mojoc.git>
* Download git master zip file <https://github.com/scottcgi/Mojoc/archive/master.zip>
* Download released version <https://github.com/scottcgi/Mojoc/releases>

#### Step 2.

Running sample [SuperLittleRed](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed), debugging on phone.

  * [IOS Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/IOS) is XCode project that all set up well, just open it with XCode. 
  * [Android Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Android) is AndroidStudio project that all set up well, just open it with AndroidStudio.
  * The only thing need to be set is switch Mojoc platform macro in [Platform.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Platform.h). 
    ```c
    #define PLATFORM_TARGET PLATFORM_IOS
    #define PLATFORM_TARGET PLATFORM_ANDROID
    ```
  * [Code Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Code) is sample code shared between android and ios.

And the AndroidStudio and Xcode already config sample source code reference [engine code](https://github.com/scottcgi/Mojoc/tree/master/Engine). So any engine code changes will present in sample game.

#### Step 3.
Understanding source code.

* [Code Style](./CodeStyle.md) is unified code writing style, all source code following.
* [OOC (Object Oriented C)](./OOC.md)) is the OOP (Object Oriented Programming) model of C language. 
  * Mojoc uses extremely lightweight OOC model that very easy to understand and use. 
  * Following the OOC rules, the C language can build complex projects, and can organize and reuse large amounts of code.
* [Component Architecture](./ComponentArchitecture.md) is code architecture support gameplay logic.
* [Code Details](./CodeDetails.md) are introduced the engine modules and functions of each code file. Through this can understand all the code of Mojoc.

The sample [SuperLittleRed](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed) give all aspects of using Mojoc, including all the above features. So modify and debug the source code helps understand all about Mojoc.

## Android Studio Bug

The `.C` source code of Mojoc in Android Studio may have a type of error msg — `"Field 'xxx' must be static"`. But this will not affect compile and run.

Obviously, this is an AS bug — that the AS error references the function defined in `.h` file, rather than the static function implemented in the `.c` file.

So, in Xcode everything is fine.

**Finally, this bug was fixed in AS 4.0.0.**

##
:door: