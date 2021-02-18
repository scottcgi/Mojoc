Mojoc strictly follow this code style to ensure the C code **readability** and **uniformity**. This code style is important to understanding how Mojoc works.

#### Code Naming
* [Basic Naming](#basic-naming)
* [Variable Naming](#variable-naming)
* [Abbreviated Naming](#abbreviated-naming)
* [Goto Label Naming](#goto-label-naming)
* [Enum Naming](#enum-naming)
* [Function Naming](#function-naming)
* [Struct Naming](#struct-naming)
* [Macro Naming](#macro-naming)
* [Array Naming](#array-naming)
* [Others Naming](#others-naming)

#### Code Format
* [Basic   Format](#basic-format)
* [Space   Format](#space-format)
* [Indent  Format](#indent-format)
* [Parentheses Format](#parentheses-format)
* [Lines   Format](#lines-format)
* [Comment format](#code-comment)

#### Others
* [Other Rules](#other-rules)


## Basic Naming

The basic use CamelCased, this is `BigCamelCased`, this is `smallCamelCased`.

## Variable Naming

* The common variable use `smallCamelCased`.
    ```c
    int               keyLength;
    int               valueTypeSize;
    SkeletonBone*     bone;
    SkeletonBoneData* boneData;
    Drawable*         drawable;
    ```

* The const use `smallCamelCased`.
  ```c
  static const char*  ids[AudioId_Length];
  static const char*  saveDataFileName = "MojocSaveDataFile";
  static const int    bezierSize       = (BEZIER_SEGMENTS + 1) * 2 - 1;
  static const float  subDivPre        = subDivStep3           / pre5;
  ```

* The bool variable (including member variables) begin with prefix `is`, then the whole is `smallCamelCased`.
  ```c
  SLboolean isLoopEnabled;
  bool      isFound;
  bool      isRemoved;
  particle->isActive
  ```

* The singleton variable begin with prefix `A`, then the whole is `BigCamelCased`.
  ```c
  extern struct AComponent   AComponent  [1];
  extern struct ADrawable    ADrawable   [1];
  extern struct AParticle    AParticle   [1];
  extern struct AApplication AApplication[1];
  ```

* The variable of function get out argument begin with prefix `out`, then the whole is `smallCamelCased`.
  ```c
  void (*OnSaveData)(void**   outSaveData,   int*         outLength);
  void (*Inverse)   (Matrix4* matrix4,       Matrix4*     outInverse);
  void Init         (int      valueTypeSize, ArrayIntMap* outArrayIntMap);
  ```

* The variable of function pointer use `BigCamelCased`.
  ```c
  typedef struct
  {
      void (*OnPause)  (void);
      void (*OnResume) (void);
      void (*OnDestroy)(void);
  }
  ApplicationCallbacks;


  typedef float (*TweenActionValueOnGet)(void* target);
  typedef void  (*TweenActionValueOnSet)(void* target, float value);


  typedef struct
  {
      TweenActionValueOnGet OnGet;
      TweenActionValueOnSet OnSet;
  }
  TweenActionValueGetSet;
  ```

## Abbreviated Naming

* The abbreviated words use `uppercase` words or `lowercase` words.
  ```c
  typedef struct
  {
  }
  RGB;


  RGB rgb;
  RGB myRGB;
  RGB rgbDatas[10];
  RGB myRGBData;
  

  void SetRGB    (RGB* rgb);
  void RGBSet    (RGB* rgb);
  void SetRGBData(RGB* rgb);

  ```

## Goto Label Naming

* The goto label tag use `BigCamelCased`. But goto label address use common variable rule. 

  ```c
  goto ParseArrayEnd;
  goto ParseObjectEnd;
  goto UseVBO;
  goto UseVAO;
  goto *coroutine->step;
  ```

## Enum Naming

* The enum use `BigCamelCased`, and must have prefix separated by `_`.
  ```c
  enum
  {
      HeroState_Stand,
      HeroState_DieOver,
  };


  enum
  {
      CollisionGroup_HeroBody   = 1,
      CollisionGroup_HeroAttack = 1 << 1,
  };
  ```

* The typedef enum use `BigCamelCased`, and must have prefix separated by `_`.
  ```c
  typedef enum
  {
      FontTextAlignment_HorizontalLeft,
      FontTextAlignment_HorizontalRight,
      FontTextAlignment_VerticalTop,
      FontTextAlignment_VerticalBottom,
  }
  FontTextAlignment;


  typedef enum
  {
      InputTouchType_Up     = 1,
      InputTouchType_Down   = 2,
      InputTouchType_Move   = 3,
      InputTouchType_Cancel = 4,
  }
  InputTouchType;
  ```

## Function Naming

* The global function and global inline function use `BigCamelCased`, and must have prefix separated by `_`.
  ```c
  extern void         Application_Main        (void);
  static inline void  AApplication_AppendChild(Component* child);
  static inline float AGLTool_ToGLWidth       (float      screenWidth);
  static inline float AMath_Random            (void);
  ```

* The local function and local inline function use `BigCamelCased`;
  ```c
  static void         LoadingRun    (Coroutine* coroutine);
  static inline float GetWorldScaleY(Drawable*  drawable);
  ```

* The typedef function use `BigCamelCased`, and must have prefix.
  ```c
  typedef float (*TweenActionValueOnGet)(void*      target);
  typedef void  (*TweenActionValueOnSet)(void*      target, float value);
  typedef void  (*CoroutineRun)         (Coroutine* coroutine);
  ```

* The function operation may not succeed, begin with prefix `Try`, then the whole is `BigCamelCased`.
  ```c
  void* (*TryPut)   (ArrayIntMap* arrayIntMap, intptr_t key, void* valuePtr);
  bool  (*TryRemove)(ArrayIntMap* arrayIntMap, intptr_t key);
  ```

* The function return boolean which is operation purpose, begin with prefix `Is` or `Test` or `Check` , then the whole is `BigCamelCased`.
  ```c
  bool (*IsContains)         (ArrayIntSet*  arrayIntSet, intptr_t element);
  bool (*TestPolygonPoint)   (Array(float)* vertexArr,   float x, float y);
  bool ADrawable_CheckVisible(Drawable*     drawable);
  ```

* The function begin with prefix `Release`, means free the struct members memory, after then the pointer needs to be freed manually.
  ```c
  void (*Release)    (Mesh* mesh);
  void (*ReleaseQuad)(DrawAtlas* drawAtlas, SubMesh* subMesh);
  ```

* The function begin with the prefix `Destroy`, means free the struct members and self memory, after then the pointer cannot be used.
  ```c
  void (*Destroy)(void);
  void (*Destroy)(Texture* texture);
  ```

* The function begin with prefix `Create`, means return the malloc pointer, so use `Release` and `free` or `Destroy` for delete.
  ```c
  ArrayList* (*Create)            (int elementTypeSize);
  ArrayList* (*CreateWithSize)    (int elementTypeSize, int size);
  ArrayList* (*CreateWithCapacity)(int elementTypeSize, int capacity);
  ```

* The function begin with prefix `Init`, means the pointer members memory on stack, so use `Release` or `Destroy` for delete memory.
  ```c
  void (*Init)            (int elementTypeSize, ArrayList*               outArrayList);
  void (*InitWithSize)    (int elementTypeSize, int size,     ArrayList* outArrayList);
  void (*InitWithCapacity)(int elementTypeSize, int capacity, ArrayList* outArrayList);
  ```

* The function if no args then type `void` arg.
  ```c
  Drawable*          (*Create)      (void);
  int                (*GetDrawCalls)(void);
  struct ATweenTool* (*AddAction)   (void);
  ```

* The function have suffix `Impl`, means the function must be implemented in `.c` file.
  ```c
  extern void Application_MainImpl(void);


  struct ATweenActionValueGetSetImpl
  {
      TweenActionValueGetSet moveX  [1];
      TweenActionValueGetSet moveY  [1];

      TweenActionValueGetSet scaleX [1];
      TweenActionValueGetSet scaleY [1];

      TweenActionValueGetSet rotateZ[1];
      TweenActionValueGetSet fadeTo [1];
  };

  extern struct ATweenActionValueGetSetImpl ATweenActionValueGetSetImpl[1];
  ```

## Struct Naming

* The struct (union) or typedef struct (union), use `BigCamelCased`.

## Macro Naming

* The head define macro, use `uppercase` words separated by `_` and have suffix `H`.
  ```c
  #ifndef STYLE_GUIDE_H
  #define STYLE_GUIDE_H
  ```

* The macro without parameters, use `uppercase` words separated by `_`.
  ```c
  #define MATH_PI  3.141592653589793
  #define MATH_2PI 6.283185307179586
  #define MATH_PI2 1.570796326794897
  ```

* The macro with parameters, use `BigCamelCased`, and must have prefix separated by `_`.
  ```c
  #define AMath_Min(x, y) 
  #define AStruct_GetParent2(memberPtr, structType)
  #define ACoroutine_YieldFrames(waitFrames)
  ```

* The local macro in `.c` file, use `BigCamelCased`, no prefix required, and must `#undef` in same `.c` file.
  ```c
  #define CheckIndex(tag, index)
  #define CheckCallback(callback)
  #define CheckFingerId(tag, fingerId)

  #undef CheckIndex
  #undef CheckCallback
  #undef CheckFingerId
  ```

* The macro type parameter must have suffix `Type`.
  ```c
  #define ArrayList(ElementType)
  #define AArrayIntSet_Init(ElementType, increase)
  #define AArrayList_Pop(arrayList, ElementType)
  ```

## Array Naming

* Array as parameter, use `[]` form, not `*` form.
  ```c
  void (*SetTimeToBuff)(char        buff  [], int seconds);
  int  (*BinarySearch) (const float values[], int valuesLength, float target);
  ```

* Array as parameter, use `[length]` form, if the length is clear.
  ```c
  void (*GetLanguageCode)(char outLanguageCode[2]);
  ```

* Array variable use `[length]` form, if the length is clear.
  ```c
  extern struct AMesh AMesh[1];


  struct AGLInfo
  {
      GLfloat pointSizeRange[2];
      GLfloat lineWidthRange[2];
  }


  ArrayIntSet set[1] = AArrayIntSet_Init(ElementType, increase);
  ```

* Array composite literal use `[length]` form, if the length is clear.
  ```c
  #define AColor_Make(r, g, b, a) (Color[1])   {{r, g, b, a }}
  #define AMatrix_Make(...)       (Matrix4[1]) {{__VA_ARGS__}}
  InitSprite(outSprite, texture, (Array(Quad)[1]) {quad, 1}); 
  ```

## Others Naming

* The following cases use `BigCamelCased`. 
  * File Name
  * Folder Name
  * Resource Name


## Basic Format

* Use `4 spaces` key indent, not `tab` key.
* One line `120 chars`.
* The pointer `*` on type name side.
  ```c
  int*  p1;
  int** p2 = &p1;
  void* Function(char* str);
  ```
* In code block try `vertical` alignment.
  ```c
  See all the examples above.
  ```


## Space Format

* The operator at least `one space` on both sides.
  ```c
  vertexX + (y - vertexY) / (preY - vertexY) * (preX - vertexX)
  ```

* The `if, while, for, switch`, must have `{}`, and `one space` with `()`.
  ```c
  if (...)
  {
  }
  

  while (...)
  {
  }


  for (...)
  {
  }


  switch (...)
  {
  }
  ```

## Indent Format

* The case `4 spaces` indent, and the break `4 spaces` indent in switch case.
  ```c
  switch (...)
  {
      case 0:
          break;

      case 1:
      {
          break;
      }
  }
  ```

* The goto label indent with the current line.
  ```c
  static void Function()
  { 
       goto Label:

       Label1:
       int a;

       Label2:
       int b;

       Label3:
       int c;
  }
  ``` 
  
* The conditional compile, indent with the current line.
  ```c
  typedef struct
  {
       Sprite       sprite[1];
       PhysicsBody* body;
       Enemy*       enemy;
       ArrowHitType hitType;

       #ifdef APP_DEBUG
       Drawable     debugDrawable[1];
       #endif
  }
  Arrow;

  void Function()
  {
      int a;
        
      #ifdef APP_DEBUG
      int b;
      #endif
  }


  #ifdef APP_DEBUG
  Drawable debugDrawable[1];
  #endif
  ```

## Parentheses Format

* The `{}` and `{{}}` vertical alignment or on the same line.
  ```c
  {
    ...
  }  // vertical alignment
  
    
  {{
    ...
  }} // vertical alignment 


  {...} // same line
  

  {{...}} // same line
  ```

* The `{{}}` used for array init.
  ```c
  struct AGameMap AGameMap[1] =
  {{
      .Init      = Init,
      .Run       = Run,
      .RandomMap = RandomMap,
  }};

  Vector3 vector[1] =
  {{
      drawable->modelMatrix->m0,
      drawable->modelMatrix->m1,
      drawable->modelMatrix->m2,
  }};

  #define AMatrix_Make(...) (Matrix4[1]) {{__VA_ARGS__}}
  ```

* The `()` if need to wrap, then vertical alignment.
  ```c
  AMath_Max
  (
      animationData->duration,
      AArray_Get
      (
          deformTimeline->frameArr,
          deformTimeline->frameArr->length - 1,
          float
      )
  );

  static void ReadAnimationDeform
  (
      SkeletonData*          skeletonData,
      JsonObject*            jsonDeform,
      SkeletonAnimationData* animationData,
      ArrayList*             skeletonTimelineArr
  )
  {
  }
  ```

## Lines Format

* Empty `two lines` around functions.
  ```c
  static int a = 100;


  static void Function1()
  {
  }


  static inline void Function2()
  {
  }

 
  struct A
  {
  }
  ```

* Empty `two lines` between different contents.
  ```c
  #include "AAA.h"
  #include "BBB.h"


  typedef float (*Function1)(void* target);
  typedef float (*Function2)();


  struct A
  {
  }


  struct B
  {
  }


  extern struct B B[1];
  ```

## Code Comment

* Outside function body use `/***/`.
  ```c
  /**
   * Comment struct
   */  
  struct A
  {   
      /**
       * Comment property
       */ 
      int a;
      
      /**
       * Comment function
       */
      void (*Function)();
  }
  ```

* In function body use `//`.
  ```c
  void Function()
  {
    // comment in function body.
  } 
  ```

* Comment block or multiline code.
  ```c
  /* This is means comment block or multiline code. */

  /* 
  This is means comment block or multiline code. 
  */
  ```

* Use comment line to separate different logical related contents.
  ```c
  void Function1();


  //--------------------
  

  void Function2();


  void Function3()
  {
      int a;

      //---------------- 

      int b;
  }
  ```

* Use comment line to mark macros `#ifdef Platform_X` or multiple `#endif`.
  ```c
  #ifndef PLATFORM_H
  #define PLATFORM_H


  //------------------------
  #ifdef IS_PLATFORM_ANDROID
  //------------------------


  // platform codes.


  //---------------------------
  #endif // IS_PLATFORM_ANDROID
  //---------------------------


  //------------------
  #endif // PLATFORM_H
  //------------------
  ```


## Other Rules
* The parameter macro, used only when `inline` function not working.
  * such as shortcut parameters of function call.
    ```c
    /**
     * Shortcut of AArrayList->GetAdd.
     * return element.
     */
     #define AArrayList_GetAdd(arrayList, ElementType) \ 
     (*(ElementType*) AArrayList->GetAdd(arrayList))
    ```
  * such as macro specific functionality.
    ```c
    #define CheckIndex(tag, index)                                      \
    ALog_A                                                              \
    (                                                                   \
        ((index) < arrayList->size && (index) >= 0),                    \
        "AArrayList " tag " failed index error, index = %d, size = %d", \
        index,                                                          \
        arrayList->size                                                 \
    )     
    ```

  * such as generic parameter.
    ```c
    /**
     * Marked ArrayList element type.
     * ElementType: element data type
     */
    #define ArrayList(ElementType) ArrayList
    ```

  * such as varargs.
    ```c
    #define ALog_A(e, ...) e ? (void) 0 : ALog_E(__VA_ARGS__),  assert(e);
    ```

* Do not use number as `boolean` value.
* The `coefficient` number uses `int` unless it is a `float`.
  ```c
  float f1 = sprite->width / 2;
  float f2 = sprite->width / 2.5f;
  ```
* Use `++i` whenever possible.
  ```c
  for (int i = 0; i < 10; ++i)
  {
  }
  ```
* Assignment appears in the `if` or `while` statement, adding brackets.
  ```c
  int intVal;
  if ((intVal = GetInt())) 
  {
      int tmp = intVal * 100;
  }

  int intVal1;
  int intVal2 = GetInt();
  while ((intVal1 = intVal2)) 
  {
      int tmp = intVal1 * 100;
  }
  ```
* The `const` variable cannot be modified.
* Use `enum` instead of macro `#define` whenever possible.
* Use a monospaced font.

#
:smiling_imp: _**Follow the Style, Feel the Power !**_
