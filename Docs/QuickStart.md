## Three Steps To Quick Start

#### Step 1.
To get the source code, there are three options.

* Git clone from <https://github.com/scottcgi/Mojoc.git>
* Download Github master zip file from <https://github.com/scottcgi/Mojoc/archive/master.zip>
* Download released version from <https://github.com/scottcgi/Mojoc/releases>

#### Step 2.

Running sample [SuperLittleRed](../Samples/SuperLittleRed), debugging on phone.

  * [IOS Folder](../Samples/SuperLittleRed/IOS) is XCode project that all set up well, just open it with XCode. 
  * [Android Folder](../Samples/SuperLittleRed/Android) is AndroidStudio project that all set up well, just open it with AndroidStudio.
  * The only thing that needs to be set is Mojoc platform macro in [Platform.h](../Engine/Toolkit/Platform/Platform.h). 
    ```c
    #define PLATFORM_TARGET PLATFORM_IOS
    #define PLATFORM_TARGET PLATFORM_ANDROID
    ```
  * [Code Folder](../Samples/SuperLittleRed/Code) is sample code shared between android and ios.

The AndroidStudio and Xcode sample source code already references [engine code](../Engine). So any engine code changes will be present in the sample game.

#### Step 3.
Understanding the source code.

* [Code Style](./CodeStyle.md) is unified code writing style that all the source code follows.
* [OOC (Object Oriented C)](./OOC.md)) is the OOP (Object Oriented Programming) model of C language. 
  * Mojoc uses extremely lightweight OOC model that is very easy to understand and use. 
  * Following the OOC rules, the C language can build complex projects, and can organize and reuse large amounts of code.
* [Component Architecture](./ComponentArchitecture.md) is code architecture that supports gameplay logic.
* [Code Details](./CodeDetails.md) to introduce the engine modules and functions of each source file. Through this can understand all the code of Mojoc.

The sample [SuperLittleRed](../Samples/SuperLittleRed) shows all aspects of using Mojoc, including all the above features. So modifying and debugging the source code helps to understand everything about Mojoc.

## Android Studio Bug

The `.C` source code of Mojoc in Android Studio may have a type of error msg — `"Field 'xxx' must be static"`. But this will not affect compile and run.

Obviously, this is an AS bug — that the AS error references the function defined in `.h` file, rather than the static function implemented in the `.c` file.

So, in Xcode everything is fine.

**Finally, this bug was fixed in AS 4.0.0.**

##
:door: