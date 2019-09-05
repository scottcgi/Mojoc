# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

#####################################################################

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Mojoc/Engine/Build/Android/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Mojoc/Engine/Build/Android/Bin/
)

#####################################################################

include_directories(
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Mojoc/
     ../../
)

#####################################################################

add_library(
    SuperLittleRed SHARED

    ../../AppInit.c
    ../../Tool.c
    ../../GameMap.c
    ../../Hero.c
    ../../Enemy.c
    ../../EnemyAI.c
    ../../GameActor.c
    ../../HUD.c
    ../../UI.c
    ../../GameData.c
    ../../AudioTool.c
    ../../ADTool.c
)


target_link_libraries(
    SuperLittleRed
    Mojoc
)