Mojoc strictly follow this OOC (Object Oriented C) programing model. The model is customized by C unique features, so it supports C language to build large and complex projects, and still remains simple to use and easy to understand.

* [Singleton](#Singleton)
* [Encapsulation](#encapsulation)
* [Inheritance](#inheritance)
* [Composite](#composite)
* [Polymorphism](#polymorphism)
* [Override](#override)
* [Memory](#memory)
* [Examples](#examples)
* [Sample](#Sample)

## Singleton

The Singleton pattern is very important to Mojoc. In C language the data and behavior are separated and there is no namespace. The singleton can abstract a group of behavior and give it namespace. This model build a parallel data struct and behavior struct that correspond to the class of OOP model.

#### In `.h` file
  ```c
  struct ADrawable
  {
      Drawable* (*Create)();  
      void      (*Init)  (Drawable* outDrawable);
  };


  extern struct ADrawable ADrawable[1];
  ```
  
  * The `ADrawable` is singleton, global unique.
  * The struct `ADrawable` is type name, `ADrawable[1]` is object name.
  * The `ADrawable` object defined as an array, can let it use like a ptr.

#### In `.c` file
  ```c
  static Drawable* Create()
  {
      return (Drawable*) malloc(sizeof(Drawable));
  }

 
  static void Init(Drawable* outDrawable)
  {
      // init outDrawable
  }


  struct ADrawable ADrawable[1] =
  {
      Create,  
      Init,
  };
  ```
 
  * Just initialize the singleton `ADrawable` object.


## Encapsulation

#### In `.h` file
  ```c
  typedef struct Drawable Drawable;  
  struct  Drawable  
  {  
      float positionX;  
      float positionY;  
  };


  typedef struct 
  {  
      Drawable* (*Create)();  
      void      (*Init)  (Drawable* outDrawable);  
  }  
  ADrawable;  
  

  extern ADrawable ADrawable[1]; 
  ```
  
  * The `Drawable` object hold the data.
  * The `ADrawable` object hold the behavior.
  * The `Create` function malloc the `Drawable` memory like new keyword.
  * The Init function initialize `Drawable` which memory already exists, often in stack or in superclass memory.

## Inheritance

The inheritance is superclass struct all datas are embedded in subclass struct. Then one malloc can get all memory in inheritance chain, and one free on subclass object can release all memory in inheritance chain.

  ```c
  typedef struct Drawable Drawable;  
  struct  Drawable  
  {  
      int a;
  };


  typedef struct
  {
      Drawable drawable[1];
  }
  Sprite;


  struct ASprite
  {
      Sprite* (*Create)();  
      void    (*Init)  (Sprite* outSprite);  
  };
  ```

* The `Drawable` is superclass.
* The `Sprite` is subclass.
* The `drawable[1]` as an array, can let it use like a ptr.
* In `ASprite` function `Create` and `Init`, indirect call `ADrawable->Init` for initialize superclass memory.
* In C there is no limit of inheritance number, the subclass can inheritance any numbers of superclass — this is a new angle of abstract.

#### How to visit subclass from superclass ?

```c
/**
 * Get struct pointer from member pointer.
 * this for memberPtr same as memberName.
 * 
 * memberPtr: the pointer that point struct's member.
 *            it's the member address offset from struct address.
 * 
 */
#define AStruct_GetParent(memberPtr, ParentType) \
    ((ParentType*) ((char*) (memberPtr) - offsetof(ParentType, memberPtr)))

Sprite* sprite = AStruct_GetParent(drawable, Sprite);
```

* First, get the superclass offset in subclass struct.
* Second, get the subclass ptr from superclass ptr by superclass offset.
* With this ability, we can hold superclass ptr execute same interface, but different implementation of subclass — this is polymorphism.

## Composite

The composite is independent struct ptr are embedded in struct .

```c
typedef struct Drawable Drawable;  
struct  Drawable  
{  
    Drawable* parent;
}; 
```

* The parent composite into `Drawable`, the memory manage by independent `Create` and `Release` of parent.

## Polymorphism

```c
typedef struct Drawable Drawable;  
struct  Drawable  
{   
    void (*Draw)(Drawable* drawable);  
};  
```

The function `Draw` in struct, means different `Drawable` object can has own implementation of `Draw`.

  ```c
  typedef struct  
  {  
      Drawable drawable[1];  
  }  
  Hero;


  typedef struct  
  {  
      Drawable drawable[1];  
  }  
  Enemy; 


  Drawable drawables[] =   
  {  
      hero->drawable,  
      enemy->drawable,  
  };  


  for (int i = 0; i < 2; ++i)  
  {  
      Drawable* drawable = drawables[i];  
      drawable->Draw(drawable);  
  }
  ```

The `Hero` and `Enemy` implement own `Draw` behavior.

## Override

In inheritance chain, usually need to override superclass function and call superclass function.

  ```c
  typedef struct  
  {  
      Drawable drawable[1];  
  }  
  Sprite;  


  struct ASprite
  {  
      void (*Draw)(Drawable* drawable);  
  };  


  extern ASprite ASprite;  
  ```

If we want to override Sprite's `Draw` function: 
  * First, need to publish `Draw` to `ASprite`;
  * Second, override `Draw` function; 
  * Then, we can still call the original `Draw` by `ASprite`.

  ```c
  typedef struct  
  {  
      Sprite sprite[1];  
  }  
  SpriteBatch;


  // subclass implementation  
  static void SpriteBatchDraw(Drawable* drawable)  
  {  
        // call father  
        ASprite->Draw(drawable);
      
        // do extra things...
  }  


  // override
  spriteBatch->sprite->drawable->Draw = SpriteBatchDraw;
  ```

## Memory

* `Create` — malloc struct memory and delete by `free` and `Release` if needed.
* `Init` — initialize struct memory and delete by `Release` if needed.
* `Release` — delete struct member ptr memory which composite into struct and `Create` by struct self.
* `Destroy` — first `Release` then `free`.

## Examples

* [Drawable.h](../Engine/Graphics/Draw/Drawable.h)
* [Drawable.c](../Engine/Graphics/Draw/Drawable.c)
* [Sprite.h](../Engine/Graphics/OpenGL/Sprite.h)
* [Sprite.c](../Engine/Graphics/OpenGL/Sprite.c)
* [Struct.h](../Engine/Toolkit/HeaderUtils/Struct.h)

## Sample

Mojoc uses [OOC Spine](../Engine/Extension/Spine) to implement the Java [OOP Spine](https://github.com/EsotericSoftware/spine-runtimes/tree/3.5/spine-libgdx/spine-libgdx/src/com/esotericsoftware/spine).

##
:rocket: