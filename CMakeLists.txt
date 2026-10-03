cmake_minimum_required(VERSION 3.19)

# Options
option(ENABLE_DEBUG_UTILS "Enable debug_utils.hpp" ON)

project(minesweeper3d)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   # We need to use clang's linker, as it threw many 'duplicate section' errors on Windows + MSYS2 and 'print' header
   add_link_options("-fuse-ld=lld")
endif()


add_library(
   minesweeper3d-lib

   ./src/application.cpp
   ./src/profiling.hpp
   ./src/profiling.cpp

   ./src/game/board.cpp
   ./src/game/board.hpp
   ./src/game/crosshair.cpp
   ./src/game/crosshair.hpp

   ./src/resource_manager.cpp
   ./src/resource_manager.hpp

   ./src/input.cpp
   ./src/input.hpp

   ./src/error.hpp

   ./src/math/math.hpp
   ./src/math/matrix.hpp

   ./src/render/camera.hpp
   ./src/render/camera.cpp
   ./src/render/mesh.hpp
   ./src/render/program.cpp
   ./src/render/program.hpp
   ./src/render/shader.cpp
   ./src/render/shader.hpp
   ./src/render/texture.cpp
   ./src/render/texture.hpp

   ./src/ui/components/core.hpp
   ./src/ui/components/core.cpp
   ./src/ui/components/modifier.hpp
   ./src/ui/components/components.cpp
   ./src/ui/components/components.hpp

   ./src/render/window/window.hpp
   ./src/render/window/window.cpp
   ./src/render/window/window_system.hpp
   ./src/render/window/window_system.cpp

   ./src/ui/scene.cpp
   ./src/ui/scene.hpp
   ./src/ui/main_menu_scene.cpp
   ./src/ui/main_menu_scene.hpp
   ./src/ui/game_scene.cpp
   ./src/ui/game_scene.hpp

)

add_executable(minesweeper3d
   ./src/main.cpp
   ./vendor/glad/glad.c
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
   # Enable standard-compliant preprocessor
   target_compile_options(minesweeper3d-lib PUBLIC /Zc:preprocessor)
   # make sure value of __cplusplus macro is correct
   target_compile_options(minesweeper3d-lib PUBLIC /Zc:__cplusplus)
   # Enable standard c++ stack unwinding, extern "c" never throw c++ exceptions
   target_compile_options(minesweeper3d-lib PUBLIC /EHsc)
endif()

if(ENABLE_DEBUG_UTILS)
   target_compile_definitions(minesweeper3d-lib PUBLIC ENABLE_DEBUG_UTILS)
   target_compile_definitions(minesweeper3d PUBLIC ENABLE_DEBUG_UTILS)
endif()

# Enable warnings
target_compile_options(minesweeper3d-lib PRIVATE
  "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Wconversion>")

# This caused issues when includign glad
target_compile_definitions(minesweeper3d-lib PUBLIC GLFW_INCLUDE_NONE)

target_compile_features(minesweeper3d-lib PUBLIC cxx_std_23)
target_compile_features(minesweeper3d PUBLIC cxx_std_23)

include_directories(./src)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# TODO Turn off building examples once base is set up
set(GLFW_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)

add_subdirectory(vendor/tasty SYSTEM)
add_subdirectory(vendor/logzy SYSTEM)
add_subdirectory(vendor/glfw SYSTEM)
include_directories(SYSTEM vendor/glad)
include_directories(SYSTEM vendor/stb)
include(./cmake/imgui-lib.cmake)

find_package(OpenGL REQUIRED)

target_link_libraries(minesweeper3d-lib 
    OpenGL::GL logzy::logzy glfw imgui
    $<$<CXX_COMPILER_ID:Clang>:stdc++exp>
)
target_link_libraries(minesweeper3d minesweeper3d-lib)

# Testing
include(CTest)
if(BUILD_TESTING)
   add_subdirectory(./tests)
endif()
