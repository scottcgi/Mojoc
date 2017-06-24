![Mojoc](https://github.com/scottcgi/Mojoc/raw/master/Logo.png "Pure C game engine")

## Mojoc v0.1

Mojoc is **`open-source`**, **`cross-platform`**, **`pure C`** game engine. It is based on **OpenGLES3** and written in **`C99`**. It works on **`IOS`** and **`Android`** currently, but easily extended to **`other platforms`**, and will support **`more platforms`** in the future.

## Architecture
![Mojoc Engine Architecture](https://github.com/scottcgi/Mojoc/raw/master/Architecture.png "Mojoc Engine Architecture")

## License
Mojoc is **licensed** under the [MIT License](https://github.com/scottcgi/Mojoc/blob/master/LICENSE "Mojoc Under MIT License").

## Core Idea
**`Less`** is more, **`Simple`** is best, **`More complex`** thinking keep less and simple.

## Features

* The only need programming skill is **`C language`** with **`C99`**.
* In platform-independent code, the **`C Standard Library`** is only tool that Mojoc used, in others Mojoc build everything.
* Mojoc has own unique **`code style`**. Details: [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style)
* Mojoc use extremely lightweight **`OOC (Object Oriented C)`** programing model. Details: [Mojoc OOC model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C))
* Mojoc application architecture based on **`Component`**, **`State-Machine`**, **`Message-Driven`**. Details: [Mojoc Component](https://github.com/scottcgi/Mojoc/wiki/Component-Based,-State-Machine,-Message-Driven-Architecture)

#### `Mojoc Toolkit Module`

[Toolkit](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit) module is **`development kit`** of C language. Provide basic **`data structure`** and **`algorithm`** support.

* [Utils](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Utils) provide basic **`data storage tools`** and **`algorithm structure`**.
    * [Array.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Array.h), implement **`generic`** array.
    * [ArrayIntMap.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayIntMap.h), implement the **`int key`** dictionary.
    * [ArrayIntSet.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayIntSet.h), implement the **`int key`** set.
    * [ArrayList.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayList.h), implement **`generic`** array list.
    * [ArrayQueue.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayQueue.h), implement the **`generic queue`** by ArrayList.
    * [ArrayStrMap.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayStrMap.h), implement the **`string key`** dictionary.
    * [BufferReader.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/BufferReader.h), implement the **`byte`** stream read.
    * [Coroutine.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Coroutine.h), implement the **`simple`** and **`effective`** **`coroutine`** for C.
    * [FileTool.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/FileTool.h), impement the cross-platfom **`file read and write`**.
    * [Json.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Json.h), implement the independent **`json parser`**.
    * [Tween.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Tween.h), implement the independent **`tween animation`**.
    * [TweenTool.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/TweenTool.h), implement the **`interface`** of tween animation **`chain calls`**.
    
* [Platform](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Platform) provide platform related tools implementation.
    * [File.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/File.h), **`interface`** of native file operation.
        * [Android File.c](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Platform/Android/File.c), implement **`android`** file operation.
        * [IOS File.c](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Platform/IOS), implement **`ios`** file operation.

* [Math](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Math) provide **`mathematical`** related **`algorithm`** implementation.
    * [Math.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Math/Math.h), implement math **`formulas`** and define math **`constants`**.
    * [Matrix.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Math/Matrix.h), implement all the **`matrix operations`** .
    * [TweenEase.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Math/TweenEase.h), implement the standard **`tween algorithm`**.
    * [Vector.h](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Math/Vector.h), implement **`vector operations`**.

* [Head](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Head) provide **`simple structure`** definition and operation.


#### `Mojoc ThirdParty Module`

[ThirdParty](https://github.com/scottcgi/Mojoc/tree/master/Engine/ThirdParty) module is **`prebuild`** third party **`libraries`**, that can **`direct called`** by Mojoc engine. So the third party need to provide different **`prebuild lib files`** for native platforms of Mojoc support. Currently only contains **`PNG`** library.

#### `Mojoc Graphics Module`

[Graphics](https://github.com/scottcgi/Mojoc/tree/master/Engine/Graphics) module is draw layer implemented by **`OpenGLES3`**, and provide **`drawing tools`** and **`implementations`**.
