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

* Mojoc has own unique **`code style`**.
    * See Details: [Mojoc Code Style](https://github.com/scottcgi/Mojoc/wiki/Code-Style)
    
* Mojoc use extremely lightweight **`OOC (Object Oriented C)`** programing model.
    * See Details: [Mojoc OOC model](https://github.com/scottcgi/Mojoc/wiki/OOC-(Object-Oriented-C))
    
* Mojoc application architecture based on **`Component`**, **`State-Machine`**, **`Message-Driven`**.
    * See Details: [Mojoc Component](https://github.com/scottcgi/Mojoc/wiki/Component-Based,-State-Machine,-Message-Driven-Architecture)

#### `Mojoc Toolkit Module`

[Toolkit](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit) module is **`development kit`** of C language. Provide basic **`data structure`** and **`algorithm`** support.

* [Utils](https://github.com/scottcgi/Mojoc/tree/master/Engine/Toolkit/Utils) provide basic **`data storage tools`** and **`algorithm structure`**.
    * [Generic Array](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Array.h)
    * [ArrayIntMap](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayIntMap.h), implement the **`int key`** dictionary.
    * [ArrayIntSet](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayIntSet.h), implement the **`int key`** set.
    * [Generic ArrayList](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayList.h)
    * [Generic ArrayQueue](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayQueue.h), implement the **`generic queue`** by ArrayList.
    * [ArrayStrMap](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/ArrayStrMap.h), implement the **`string key`** dictionary.
    * [BufferReader](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/BufferReader.h), implement the **`byte`** stream read.
    * [Coroutine](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Coroutine.h), implement the **`simple`** and **`effective`** coroutine for C.
    * [FileTool](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/FileTool.h), impement the cross-platfom **`file read and write`**.
    * [Json](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Json.h), implement the independent **`json parser`**.
    * [Tween](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/Tween.h), implement the independent **`tween animation`**.
    * [TweenTool](https://github.com/scottcgi/Mojoc/blob/master/Engine/Toolkit/Utils/TweenTool.h), implement the **`interface`** of tween animation **`chain calls`**.
