
<img src="https://github.com/scottcgi/Mojoc/raw/master/Docs/Pic/Architecture.png" width="790" height="560" alt="Mojoc Engine Architecture" title="Mojoc Engine Architecture" />

## 

Mojoc modules have dependency hierarchy, the upper layer can reference the lower layer, but the lower layer cannot reference the upper layer, and parallel modules cannot be referenced to each other. This ensures minimal dependency compilation of modules.

* [Toolkit Module](#toolkit-module)
* [ThirdParty Module](#thirdparty-module)
* [Graphics Module](#graphics-module)
* [Physics Module](#physics-module)
* [Audio Module](#audio-module)
* [Extension Module](#extension-module)
* [Application Module](#application-module)


## Toolkit Module

The [Toolkit](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit) module is development kit of C language. It provides basic data structure and algorithm support.

* [Math](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Math) provides mathematical related algorithm implementation.
* [Utils](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Utils) provides basic data storage tools and algorithm structure.
* [HeaderUtils](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/HeaderUtils) provides independent structure definitions or functions (no `.c` file implementation).
* [Platform](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Platform) provides platform-related tools implementation.


## ThirdParty Module

The [ThirdParty](https://github.com/scottcgi/Mojoc/tree/master/Engine/ThirdParty) module is prebuild third party libraries, that can direct called by Mojoc engine. So the third party need to provide different prebuild lib files for native platforms of Mojoc support.

* Currently only contains [PNG](https://github.com/scottcgi/Mojoc/tree/master/Engine/ThirdParty/PNG) library.
* [ThirdPartyBuild](https://github.com/scottcgi/Mojoc/tree/master/ThirdPartyBuild) is multi-platform build project of ThirdParty.

## Graphics Module

[Graphics](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics) module is draw layer implemented with OpenGLES3, and provide drawing tools and implementations.

* [Draw](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics/Draw) provides basic abstract drawing layer.
* [OpenGL](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics/OpenGL) provides OpenGLES rendering functions.
  * [Platform](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics/OpenGL/Platform) provides platform-related EGL implementation.
* [Tween](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics/Tween) provides tween animation implementation.
* [Utils](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics/Utils) provides drawing support tools.

## Physics Module

[Physics](https://github.com/scottcgi/Mojoc/tree/master/Engine/Physics) module is simple physical simulation.

## Audio Module

* [Audio.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Audio/Platform/Audio.h) is interface of audio API.
* [Platform](https://github.com/scottcgi/Mojoc/tree/master/Engine/Audio/Platform) is cross-platform audio implementation
  * [Android/Audio.c](https://github.com/scottcgi/Mojoc/blob/master/Engine/Audio/Platform/Android/Audio.c) is OpenSLES implementation in Android.
  * [IOS/Audio.m](https://github.com/scottcgi/Mojoc/blob/master/Engine/Audio/Platform/IOS/Audio.m) is OpenAL implementation in IOS.

## Extension Module

[Extension](https://github.com/scottcgi/Mojoc/tree/master/Engine/Extension) module is editor-related implementation.
  * [Spine](https://github.com/scottcgi/Mojoc/tree/master/Engine/Extension/Spine) is runtime implementation of Spine 2D skeleton animation.

## Application Module

[Application](https://github.com/scottcgi/Mojoc/tree/master/Engine/Application) module is app control and interaction layer. It provides app lifecycle callback, input event, native platform functions, component architecture and so on.

* [Platform](https://github.com/scottcgi/Mojoc/tree/master/Engine/Application/Platform) are platform-related implementations.
  * [Platform/Android](https://github.com/scottcgi/Mojoc/tree/master/Engine/Application/Platform/Android) are native function implementations in Android.
  * [Platform/IOS](https://github.com/scottcgi/Mojoc/tree/master/Engine/Application/Platform/IOS) are native function implementations in IOS.

## 
:book: