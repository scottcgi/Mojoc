![Mojoc](https://github.com/scottcgi/Mojoc/raw/master/Docs/Pic/Logo.png "Pure C game engine")

## Mojoc v0.1

Mojoc is **`open-source`**, **`cross-platform`**, **`pure C`** game engine. It is based on **OpenGLES3** and written in **`C99`**. It works on **`IOS`** and **`Android`** currently, but easily extended to **`other platforms`**, and will support **`more platforms`** in the future.

## Architecture
![Mojoc Engine Architecture](https://github.com/scottcgi/Mojoc/raw/master/Docs/Pic/Architecture.png "Mojoc Engine Architecture")

## License
Mojoc is **licensed** under the [MIT License](https://github.com/scottcgi/Mojoc/blob/master/LICENSE "Mojoc Under MIT License").

## Core Idea
**`Less`** is more, **`Simple`** is best, **`More complex`** thinking keep less and simple.

## Features

* The only need programming skill is **`C language`** with **`C99`**.
* In platform-independent code, the **`C Standard Library`** is only tool that Mojoc used, in others Mojoc build everything.
* Mojoc has own unique **`code style`**. 
    * [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style)
* Mojoc use extremely lightweight **`OOC (Object Oriented C)`** programing model. 
    * [Mojoc OOC model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C))
* Mojoc application architecture based on **`Component`**, **`State-Machine`**, **`Message-Driven`**. 
    * [Mojoc Component Architecture](https://github.com/scottcgi/Mojoc/wiki/Component-Architecture)
* More detail **`features`** see 
    * [Mojoc Main Features](https://github.com/scottcgi/Mojoc/wiki/Main-Features)
    
## Published Games
Mojoc has made a cross-platform **`android`** and **`ios`** game on the **`App Store`** and **`Google Play`**. And I will use Mojoc to make more games.
* **`Super Little Red`** is bow and arrow shooting game, very **`challenging`** your operation.
    * [App Store](https://itunes.apple.com/us/app/id1242353775)
    * [Google Play](https://play.google.com/store/apps/details?id=com.SuperLittleRed)
    * [Tap Tap](https://www.taptap.com/app/45524)

## Samples
The samples are **`complete`**, **`can play`**, **`present the Mojoc features`** game demo. Each platform provides its own **`platform editor`** for project build. For example, android use **`AndroidStudio`**, ios use **`XCode`**, windows use **`Visual Studio`**.

Now there is only one sample, which is the **`simplfied version`** of published game [Super Little Red](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed).

* #### How to running ?

    * [IOS Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/IOS) is **`XCode project`** that all set up well, just open it with XCode. The only thing need to be set is switch Mojoc **`platform macro`** in [Platform.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Platform.h).
      ```c
      #define PLATFORM_TARGET PLATFORM_IOS
      ```
  
    * [Android Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Android) is **`AndroidStudio project`** that all set up well, just open it with AndroidStudio. The only thing need to be set is switch Mojoc **`platform macro`** in [Platform.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Platform.h).
      ```c
      #define PLATFORM_TARGET PLATFORM_ANDROID
      ```

    * [Code Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Code) is sample code **`shared`** between android and ios.


## FAQ

* #### Why not C++ ?
    * There are **`plenty`** of C++ game engines, but pure C game engines are much **`rarer`**. I don't want to **`repeat`**, I hope to be **`differently`**.
    * The **`core idea`** of game engine I want to create is **`less`** and **`simple`**, which is consistent with the **`C`** language.
    * At the **`beginning`**, I had a **`idea`** of how to build a **`complex`** project with pure **`C`** language. I want to test the **`feasibility`** of the **`idea`**, that is **`Mojoc`**.
    * The C language is **`simple`** but **`powerful`**, it can implement a lot of **`interesting`** things. **Why not C** ?
    
* #### What is the meaning of Mojoc name ? 
   ```
   Mojoc = Mojo + c
   ```
    The **`c`** means the **`C language`**.
    
 * #### Who designed the logo and what is the meaning of it ?
    Yes, I **`designed`**. The logo is a **`creature`** made up of **`Mojoc`** letters.
    ```
    M : ears  
    OO: eyes  
    J : body  
    C : tail
    ```
