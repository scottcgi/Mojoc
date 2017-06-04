# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99")

#####################################################################

add_library(
    PNG
    STATIC
    IMPORTED
)

set_target_properties(
    PNG
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Mojoc/Engine/ThirdParty/PNG/Prebuilt/Android/${ANDROID_ABI}/libpng.a
)

#####################################################################

include_directories(
    ../../../
    ../../ThirdParty/PNG/Include/Android/
)

add_library(
    Mojoc STATIC

################ MojocToolkit ################

    ../../Toolkit/Toolkit.c

    ../../Toolkit/Math/Math.c
    ../../Toolkit/Math/Matrix.c
    ../../Toolkit/Math/TweenEase.c

    ../../Toolkit/Utils/Array.c
    ../../Toolkit/Utils/ArrayList.c
    ../../Toolkit/Utils/ArrayStrMap.c
    ../../Toolkit/Utils/ArrayIntMap.c
    ../../Toolkit/Utils/ArrayIntSet.c
    ../../Toolkit/Utils/ArrayQueue.c
    ../../Toolkit/Utils/BufferReader.c
    ../../Toolkit/Utils/Json.c
    ../../Toolkit/Utils/Tween.c
    ../../Toolkit/Utils/TweenTool.c
    ../../Toolkit/Utils/Coroutine.c
    ../../Toolkit/Utils/FileTool.c

    ../../Toolkit/Platform/Android/File.c

################ MojocGraphics ################

	../../Graphics/Graphics.c

	../../Graphics/OpenGL/Shader/ShaderSprite.c
	../../Graphics/OpenGL/Shader/ShaderPrimitive.c
	../../Graphics/OpenGL/Shader/ShaderMesh.c
	../../Graphics/OpenGL/Shader/Shader.c

	../../Graphics/OpenGL/Sprite.c

	../../Graphics/OpenGL/Camera.c
	../../Graphics/OpenGL/GLPrimitive.c
	../../Graphics/OpenGL/GLInfo.c
	../../Graphics/OpenGL/GLTool.c
	../../Graphics/OpenGL/Texture.c
	../../Graphics/OpenGL/Mesh.c
	../../Graphics/OpenGL/SubMesh.c
    ../../Graphics/OpenGL/Platform/EGLTool.c

	../../Graphics/Draw/Quad.c
	../../Graphics/Draw/Drawable.c
	../../Graphics/Draw/Color.c

	../../Graphics/Utils/Image.c
	../../Graphics/Tween/TweenDrawable.c

################ MojocPhysics ################

	../../Physics/PhysicsWorld.c
	../../Physics/PhysicsBody.c
	../../Physics/Physics.c
	../../Physics/PhysicsCollision.c

################ MojocApplication ################

	../../Application/Application.c
	../../Application/Scheduler.c
	../../Application/Component.c
    ../../Application/Input.c
	../../Application/Platform/Android/NativeGlue.c
    ../../Application/Platform/Android/JniTool.c

################ MojocExtension ################

	../../Extension/Extension.c
	../../Extension/Particle/Particle.c
	../../Extension/Particle/ParticleEmitterData.c
	../../Extension/Particle/ParticleEmitter.c

	../../Extension/Spine/SkeletonData.c
	../../Extension/Spine/SkeletonBone.c
	../../Extension/Spine/Skeleton.c
	../../Extension/Spine/SkeletonTimeline.c
	../../Extension/Spine/SkeletonSlot.c
	../../Extension/Spine/SkeletonAnimationPlayer.c

	../../Extension/TextureAtlas.c
	../../Extension/Font.c
	../../Extension/DrawAtlas.c

################ MojocAudio ################
    ../../Audio/Platform/Android/Audio.c
)


target_link_libraries(
    Mojoc

    PNG

    android
    EGL
    GLESv3
    log
    z
    OpenSLES
)


