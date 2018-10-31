<img src="https://github.com/scottcgi/Mojoc/raw/master/Docs/Pic/Logo.png" width="176" height="228" alt="Mojoc Logo" title="Mojoc Logo" />

## Mojoc v0.4.7

Mojoc is an open-source, cross-platform, pure C game engine. It is based on OpenGLES3 and written in C99. It currently works on IOS and Android, but can easily be extended to other platforms, and will support more platforms in the future.

* Released version in [releases](https://github.com/scottcgi/Mojoc/releases).
* Release changes in [changelog](https://github.com/scottcgi/Mojoc/blob/master/ChangeLog.md).
* Documents in [wiki](https://github.com/scottcgi/Mojoc/wiki).

## Architecture
![Mojoc Engine Architecture](https://github.com/scottcgi/Mojoc/raw/master/Docs/Pic/Architecture.png "Mojoc Engine Architecture")

## License
Mojoc is licensed under the [MIT License](https://github.com/scottcgi/Mojoc/blob/master/LICENSE "Mojoc Under MIT License").

## Core Idea
Less is more, simple is better, simplify complex ideas.

## Features

* The only programming skill needed is the C99 language.
* In platform-independent code, Mojoc uses only the standard C library, no dependence on third-party libraries.
* Mojoc has its own unique coding style. 
  * [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style)
* Mojoc uses an extremely lightweight OOC (Object Oriented C) programming model. 
  * [Mojoc OOC Model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C))
* Mojoc application architecture is based on components, state-machines and is message-driven. 
  * [Mojoc Component Architecture](https://github.com/scottcgi/Mojoc/wiki/Component-Architecture)
* For more feature details see 
  * [Mojoc Code Details](https://github.com/scottcgi/Mojoc/wiki/Code-Details)
    
## Published Games
Mojoc has been used for a cross-platform Android and IOS game on the App Store and Google Play. I will be using Mojoc to make more games.
* Super Little Red is a bow and arrow shooting game, very challenging for your coordination.
  * [App Store](https://itunes.apple.com/cn/app/id1242353775)
  * [Google Play](https://play.google.com/store/apps/details?id=com.SuperLittleRed)
  * [Tap Tap](https://www.taptap.com/app/45524)

## Samples
The samples are complete, can be run, and show the Mojoc features in a game demo. Each platform provides its own platform editor for building projects. For example, Android uses AndroidStudio, IOS uses XCode.

Currently there is only one sample, which is a simplfied version of the published game [Super Little Red](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed).

* #### How to run?

  * [IOS Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/IOS) is an XCode project that is all set up, just open it with XCode. The only thing needed to be set is the Mojoc platform macro in [Platform.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Platform.h).
    ```c
    #define PLATFORM_TARGET PLATFORM_IOS
    ```
  
  * [Android Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Android) is an AndroidStudio project that is all set up, just open it with AndroidStudio. The only thing needed to be set is the Mojoc platform macro in [Platform.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Platform.h).
    ```c
    #define PLATFORM_TARGET PLATFORM_ANDROID
    ```

  * [Code Folder](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed/Code) is the sample code shared between Android and IOS.


## FAQ

* #### Why not C++?
  * There are plenty of C++ game engines, but pure C game engines are much rarer. I don't want to reinvent wheels, I hope to be different.
  * The core idea of the game engine I want to create is to be smaller and simple, which is consistent with the C language.
  * At the beginning, I had an idea of how to build a complex project with pure C language. I want to test the feasibility of the idea, that is Mojoc.
  * The C language is simple yet powerful, it can do a lot of interesting things. **Why not C**?
    
* #### What is the meaning of the name Mojoc? 
  ```
  Mojoc = Mojo + c
  ```
  The c stand for the C language.
    
* #### Who designed the logo and what is its meaning?
  I designed it. The logo is an unknown creature made up of 'Mojoc' letters.
  ```
  M : ears  
  OO: eyes  
  J : body  
  C : tail
  ```
* #### How was Mojoc was born?

  Mojoc started in 2012 as a spare time project. At first, Mojoc used Eclipse + CDT + ADT + NDK for development, and was focused on Android platform. 
    
  During the development I experienced a lot of repetition and thinking. For a long time I was not very sure and clear about how to use the C language to simulate OOP and keep it simple and uniform. So I tried a lot of ideas, repeated the process, wrote code, modified, refactored, changed the ideas, deleted some code, rewritten it again and so on.
  
  But in the end, I developed a unique rule set, the [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style), and [Mojoc OOC Model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C)) that felt right and nice, then I followed the rules to develop Mojoc.


* #### What kind of games can Mojoc do?
  For now, it is not recommended to use Mojoc to make games. To be honest, Mojoc is not yet quite perfect. There are many improvements and refinements left to be made, that I'm currently working on. But Mojoc's code architecture and programming model have already been established. Mojoc is therefore very suitable for learning and understanding of how to use the C language to build game engine.
  
## Roadmap
The [Mojoc Roadmap](https://github.com/scottcgi/Mojoc/wiki/Roadmap) gives the direction of the Mojoc development.


## Support
  * :video_game:Play Mojoc [Published Games](#published-games).
  * :coffee:Buy me a coffee via [Paypal](https://www.paypal.me/PayScottcgi/0.99).
  
