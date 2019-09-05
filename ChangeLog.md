## v
_`2019.9.5 UTC+8 15:42`_

#### Features

1. Add Vector and Color macro define for construct composite literal.
1. Add Mesh fucntions: `Deform, DeformByIndex`.
1. Add `GetDrawCalls` function of ADrawable.
1. Add `Color RGB` macros.
1. Add `Deform` function of Sprite.
1. Add `TweenEase` functions: `TweenEaseType_BackInExponentialOut, TweenEaseType_BackInElasticOut`.
1. Add `Thread` implementation.
1. Add `SystemInfo` implementation.
1. Add AMath inline functions: `AMath_InvSqrtf, AMath_Sqrtf in Math`.
1. Add `MultiplyMZ` function of AMatrix.
1. Add `GetWorldScaleV2` function of ADrawable.
1. Add macro `AMatrix_Make(...)` for construct Matrix composite literal.


#### Refactor Code (Break Compatibility)

1. Refactor `Primitive` implementation.
1. Refactor `Quad` implementation.
1. Refactor `Mesh` and `SubMesh` implementation.
1. Refactor `Physics` implementation.
1. Refactor `File` and `FileTool` implementation.
1. Refactor `UserData` implementation.
1. Refactor `Drawable and ADrawable` implementation.
1. Refactor `ABitwise` implementation.
1. Refactor `PhysicsBody` state and collisionGroup `set/add` functions.
1. Refactor `NativeGlue.c` context data object.
1. Refactor the `GetKey` function return value type from `char*` to `const char*`.
1. Refactor `ACoroutine` implementation from `labels as values` to `switch case`.
1. Refactor const constraints of `Json` implementation.


#### Refactor Name (Break Compatibility)

1. Rename enum variable names of `MeshDefine`.
1. Rename JSON `Release` to `Delete`.
1. Rename Toolkit `Head` to `HeaderUtils`.
1. Rename `DrawableState_UpdateInverse` to `DrawableState_UpdateInverseMatrix`, `DrawableState_IsUpdateMVP` to `DrawableState_IsUpdateMVPMatrix`.
1. Rename `elementArray` to `elementArr`.
1. Rename `Json` and `Tween` functions name.
1. Rename `File` and `FileTool` functions name.
1. Rename `SetWithQuad` to `SetUVWithQuad`.
1. Rename `ADrawable` `RenderQueue` to `Render`.
1. Rename `BufferReader` functions name and macros.
1. Rename consts of `Sprite, Mesh, Quad` that make them legible.


#### Removed (Break Compatibility)

1. Remove `AArray_GetData` of Array.
1. Remove `Particle` System.


#### Update Comment/Log
1. Update `ArrayStrSet` and `Audio` error msg log.
1. Update code header comments.
1. Update `AStruct` comments.


#### Optimizations

1. Optimize `i--` to `--i` as can do as possible.
1. Optimize `i++` to `++i` as can do as possible.
1. Optimize `Mesh, SubMesh, Quad` data calculation.
1. Optimize `Skeleton` `Update` function implementation.
1. Optimize `Physics` implementation
1. Optimize `BufferReader` implementation.
1. Optimize `File` and `FileTool `implementation.
1. Optimize `InputTouch` implementation.
1. Optimize `Application` make it more clear and stable.
1. Optimize `composite literal` have clear length.
1. Optimize binary search.
1. Optimize `BufferReader` `TryFindString` function.
1. Optimize Json `ParseNumber` function.
1. Optimize functions: `MultiplyMX, MultiplyMY, MultiplyMV2`.
1. Optimize audio play callback error check.
1. Optimize Json function `SkipWhiteSpace` call in `ParseArray` and `ParseObject`.
1. Optimize `Mesh` and `SubMesh` funcations: `SetUVWithQuad, CreateWithData, CreateWithQuad.
1. Optimize Sprite `Deform` that move update data implementation to render function by if use VBO.

#### Fixed Bugs 

1. Fix` ANativeActivity` `OnContentRectChanged` callback bug.
1. Fix `NativeGlue` bug when window resized.
1. Fix `EGL` error setting when screen lock/unlock.
1. Fix E/libEGL: validate_display:99 error 3008 (EGL_BAD_DISPLAY).
1. Fix OpenSLES "E/libOpenSLES: Error after prepare: 1" in some devices.
1. Fix point and polygon collision algorithm error.


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
