<img src="https://raw.githubusercontent.com/scottcgi/Mojoc/master/Docs/Pic/Logo.png" width="176" height="228" alt="Mojoc Logo" title="Mojoc Logo" />


## Mojoc v0.5.0

Mojoc is an open-source, cross-platform, pure C game engine. It is based on OpenGLES3 and written in C99. It currently works on IOS and Android, but can easily be extended to other platforms, and will support more platforms in the future.

* Released version in [releases](https://github.com/scottcgi/Mojoc/releases).
* Release changes in [ChangeLog](https://github.com/scottcgi/Mojoc/blob/master/ChangeLog.md).
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
* Mojoc application architecture is based on components, state-machines and message-driven. 
  * [Mojoc Component Architecture](https://github.com/scottcgi/Mojoc/wiki/Component-Architecture)
* More feature details. 
  * [Mojoc Code Details](https://github.com/scottcgi/Mojoc/wiki/Code-Details)
  
    
## Published Games
Mojoc has been used for a cross-platform Android and IOS game on the App Store and Google Play. 

* **SuperLittleRed** is a bow and arrow shooting game, very challenging for your operation.
  * [App Store](https://itunes.apple.com/cn/app/id1242353775)
  * [Google Play](https://play.google.com/store/apps/details?id=com.SuperLittleRed)
  

## Samples
The samples will show all aspects of the Mojoc features. Each platform will provide native building project for native platform editor. For example: Android provides AndroidStudio project, IOS provides XCode project.

Currently there is only one sample, which is a simplfied version of the published game [SuperLittleRed](https://github.com/scottcgi/Mojoc/tree/master/Samples/SuperLittleRed). Please check out the wiki [Quick Start](https://github.com/scottcgi/Mojoc/wiki) guide for quickly run this sample.

The compiled and playable apk is here:

* [SuperLittleRed-2.0.1.apk](https://github.com/scottcgi/Mojoc/blob/master/Samples/Apk/SuperLittleRed-2.0.1.apk?raw=true)

<img src="https://raw.githubusercontent.com/scottcgi/Mojoc/master/Docs/Pic/SuperLittleRed-Sample.gif" width="400" height="225" alt="SuperLittleRed-Sample" title="SuperLittleRed-Sample" />



## FAQ

#### Why not C++?
  * There are plenty of C++ game engines, but pure C game engines are much rarer. I don't want to reinvent wheels, I hope to be different.
  * The core idea of the game engine I want to create is to be smaller and simple, which is consistent with the C language.
  * At the beginning, I had an idea of how to build a complex project with pure C language. I want to test the feasibility of the idea, that is Mojoc.
  * The C language is simple but powerful, it can do a lot of interesting things. **Why not C**?
    
#### What is the meaning of the name Mojoc? 
  ```
  Mojoc = Mojo + c
  ```
  The c stand for the C language.
    
#### Who designed the logo and what is its meaning?
  I designed it. The logo is an unknown creature made up of `"Mojoc"` letters.
  ```
  M : ears  
  OO: eyes  
  J : body  
  C : tail
  ```
  
#### How was Mojoc was born?

  Mojoc started in 2012 as a spare time project. At first, Mojoc used Eclipse + CDT + ADT + NDK for development, and was focused on Android platform. 
    
  During the development I experienced a lot of repetition and thinking. For a long time I was not very sure and clear about how to use the C language to simulate OOP and keep it simple and uniform. So I tried a lot of ideas, repeated the proces: wrote code, modified, refactored, changed ideas, deleted code, rewritten it again and so on.
  
  But in the end, I developed a unique rule set, the [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style) and [Mojoc OOC Model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C)) that felt right and nice, then I followed the rules to develop Mojoc.


#### What kind of games can Mojoc do?

  For now, it is not recommended to use Mojoc to make games. To be honest, Mojoc is not yet quite perfect. There are many improvements and refinements left to be made, that I'm currently working on. But Mojoc's code architecture and programming model have already been established. 
  
  Therefore Mojoc is very suitable for learning and understanding of how to use the C language to build game engine.
  
  
## Roadmap
The roadmap shows the direction of the Mojoc development.

#### Graphics Module
  * Create graphics layer compatible with Vulkan, Metal and D3D.
  * Create material system control shaders.
  * Support Primitive Restart and DrawInstance.
  * Support Sampler Objects and Immutable Textures.
  * Support dynamic Multi-Texture Unit rendering.
  
#### Extension Module
  * Support bitmap fonts.
  * Support simple test unit for Mojoc features.
  
#### Physics Module
  * Support motion simulation after rigid body collision.
  * Support mutual restriction and linkage between rigid bodies.
  * Support more physical motion simulations.  
  
#### Toolkit Module  
  * Generic iteration interface, it's a step-by-step calculation that can be driven by coroutine.
  * Memory management strategy for many to one.
  * Plug-in function extensions.
  * More algorithm implementation.  
  
#### Other Modules  
  * Create UI module.
  * Create network module.
  
#### Others
  * Game editor implement by Mojoc self.
  * Game script language `"MojoScript"`, and implement by C language with Toolkit.  


## Support
  * :video_game: Play Mojoc [Published Games](#published-games).
  * :octocat: Star & Issue & Fork on GitHub, or share **Mojoc** project to anyone.
  * :coffee: If the source code is **useful** for you, maybe buy me a coffee via [Paypal-0.99](https://www.paypal.me/PayScottcgi/0.99).
  

##  
:loop:
