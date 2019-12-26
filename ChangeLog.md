## Developing

* Remove `RotateM` function in `Matrix.c`.
* Optimize matrix `ratation`.
* Add `rotatioinX` and `rotationY` in `Drawable`. 
* Rename `Interpolates` to `Easing` in `ATweenEase`.
* Add `EasingTimeFns` in `ATweenEase`. 
* Fix `Bounce` function error when easing complete in `TweenEase`.
* Fix bug `off_t` convert to `off_t*` in function `OpenFileDescriptor` of `File.c`.
* Optimize easing functions that delete all redundant `else` keywords.
* Optimize easing functions `ElasticIn, Out, IntOut`.


## v0.5.0
_`2019.9.5 UTC+8 15:42`_

* **Features**

  * Add `Vibrator`implementation.
  * Add `JniTool` functions: `CallNativeActivityMethod`, `CallActivityMethod`, `GetField`, `GetArrayLength`, `GetArrayAt`, `GetNativeActivityField`, `GetEnvPtr`.
  * Add Audio `Stop` function.
  * Add `Create` and `Init` functions with filePath.
  * Add `AUserData_AddSlotInt` and `AUserData_AddSlotFloat`.
  * Add macro `AArray_MakeWithData()`.
  * Add Mesh drawMode.
  * Add Vector and Color macro define for construct composite literal.
  * Add Mesh fucntions: `Deform, DeformByIndex`.
  * Add `GetDrawCalls` function of ADrawable.
  * Add `Color RGB` macros.
  * Add `Deform` function of Sprite.
  * Add `TweenEase` functions: `TweenEaseType_BackInExponentialOut, TweenEaseType_BackInElasticOut`.
  * Add `Thread` implementation.
  * Add `SystemInfo` implementation.
  * Add AMath inline functions: `AMath_InvSqrtf, AMath_Sqrtf in Math`.
  * Add `MultiplyMZ` function of AMatrix.
  * Add `GetWorldScaleV2` function of ADrawable.
  * Add macro `AMatrix_Make(...)` for construct Matrix composite literal.


* **Refactor Code (Break Compatibility)**

  * Refactor "JniTool" implementation.
  * Refactor `Primitive` implementation.
  * Refactor `Quad` implementation.
  * Refactor `Mesh` and `SubMesh` implementation.
  * Refactor `Physics` implementation.
  * Refactor `File` and `FileTool` implementation.
  * Refactor `UserData` implementation.
  * Refactor `Drawable and ADrawable` implementation.
  * Refactor `ABitwise` implementation.
  * Refactor `PhysicsBody` state and collisionGroup `set/add` functions.
  * Refactor `NativeGlue.c` context data object.
  * Refactor the `GetKey` function return value type from `char*` to `const char*`.
  * Refactor `ACoroutine` implementation from `labels as values` to `switch case`.
  * Refactor const constraints of `Json` implementation.


* **Refactor Name (Break Compatibility)**

  * Rename `random()`to `rand()` and `srandom()` to `srand()`.
  * Rename Id to ID.
  * Rename `SetPlay` to `Play`, `SetPause` to `Pause`.
  * Rename enum variable names of `MeshDefine`.
  * Rename JSON `Release` to `Delete`.
  * Rename Toolkit `Head` to `HeaderUtils`.
  * Rename `DrawableState_UpdateInverse` to `DrawableState_UpdateInverseMatrix`, `DrawableState_IsUpdateMVP` to `DrawableState_IsUpdateMVPMatrix`.
  * Rename `elementArray` to `elementArr`.
  * Rename `Json` and `Tween` functions name.
  * Rename `File` and `FileTool` functions name.
  * Rename `SetWithQuad` to `SetUVWithQuad`.
  * Rename `ADrawable` `RenderQueue` to `Render`.
  * Rename `BufferReader` functions name and macros.
  * Rename consts of `Sprite, Mesh, Quad` that make them legible.


* **Removed Code (Break Compatibility)**

  * Remove `AArray_GetData` of Array.
  * Remove `Particle` System.


* **Optimizations**

  * Optimize IOS swift calls `Mojoc` code.
  * Optimize `i--` to `--i` as can do as possible.
  * Optimize `i++` to `++i` as can do as possible.
  * Optimize `Mesh, SubMesh, Quad` data calculation.
  * Optimize `Skeleton` `Update` function implementation.
  * Optimize `Physics` implementation
  * Optimize `BufferReader` implementation.
  * Optimize `File` and `FileTool `implementation.
  * Optimize `InputTouch` implementation.
  * Optimize `Application` make it more clear and stable.
  * Optimize `composite literal` have clear length.
  * Optimize binary search.
  * Optimize `BufferReader` `TryFindString` function.
  * Optimize Json `ParseNumber` function.
  * Optimize functions: `MultiplyMX, MultiplyMY, MultiplyMV2`.
  * Optimize audio play callback error check.
  * Optimize Json function `SkipWhiteSpace` call in `ParseArray` and `ParseObject`.
  * Optimize `Mesh` and `SubMesh` funcations: `SetUVWithQuad, CreateWithData, CreateWithQuad.
  * Optimize Sprite `Deform` that move update data implementation to render function by if use VBO.


* **Fix Bugs**

  * Fix `FontText` alignment bug.
  * Fix rendering bug when app goes into the background and not changing the content size.
  * Fix Android destroy callback bug when app in background.
  * Fix Audio `Release` bug in Android.
  * Fix `RenderLines` bug.
  * Fix `ANativeActivity`, `OnContentRectChanged` callback bug.
  * Fix `NativeGlue` bug when window resized.
  * Fix `EGL` error setting when screen lock/unlock.
  * Fix E/libEGL: validate_display:99 error 3008 (EGL_BAD_DISPLAY).
  * Fix OpenSLES "E/libOpenSLES: Error after prepare: 1" in some devices.
  * Fix point and polygon collision algorithm error.


## v0.4.7
_`2018.1.16 UTC+8 17:12`_

* Update copyright.
* Optimize Json ParseNumber and code format.
* Json optimize if else to switch case.
* Refactor Json ParseObject, ParseArray and SkipString fucntions.
* Rename TweenData to Tween.
* Fix tween variable name error and float value missing 'f'.
* Fix code warning and modify log msg.
* Add LibPng 1.6.25 XCode and AndroidStudio project.
* Modify java and swift code.


## v0.0.0
_`2017.7.2 UTC+8 09:58`_

* Initial Release.
