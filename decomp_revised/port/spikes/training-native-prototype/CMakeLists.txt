cmake_minimum_required(VERSION 3.20)
project(fdny_training LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use pkg-config where available, while keeping CMake package and ordinary
# include/library discovery available on systems without pkg-config.
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(SDL2 QUIET IMPORTED_TARGET sdl2)
  pkg_check_modules(JSONC QUIET IMPORTED_TARGET json-c)
endif()

if(TARGET PkgConfig::SDL2)
  set(FDNY_SDL_TARGET PkgConfig::SDL2)
else()
  find_package(SDL2 CONFIG QUIET)
  if(TARGET SDL2::SDL2)
    set(FDNY_SDL_TARGET SDL2::SDL2)
  elseif(TARGET SDL2::SDL2-static)
    set(FDNY_SDL_TARGET SDL2::SDL2-static)
  else()
    find_path(FDNY_SDL_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)
    find_library(FDNY_SDL_LIBRARY NAMES SDL2 SDL2-static)
    if(NOT FDNY_SDL_INCLUDE_DIR OR NOT FDNY_SDL_LIBRARY)
      message(FATAL_ERROR "SDL2 development files were not found by pkg-config, CMake, or standard paths")
    endif()
    add_library(fdny_sdl2 INTERFACE)
    target_include_directories(fdny_sdl2 INTERFACE "${FDNY_SDL_INCLUDE_DIR}")
    target_link_libraries(fdny_sdl2 INTERFACE "${FDNY_SDL_LIBRARY}")
    set(FDNY_SDL_TARGET fdny_sdl2)
  endif()
endif()

if(TARGET PkgConfig::JSONC)
  set(FDNY_JSONC_TARGET PkgConfig::JSONC)
else()
  find_package(json-c CONFIG QUIET)
  if(TARGET json-c::json-c)
    set(FDNY_JSONC_TARGET json-c::json-c)
  elseif(TARGET json-c)
    set(FDNY_JSONC_TARGET json-c)
  else()
    find_path(FDNY_JSONC_INCLUDE_DIR NAMES json-c/json.h)
    find_library(FDNY_JSONC_LIBRARY NAMES json-c json_c)
    if(NOT FDNY_JSONC_INCLUDE_DIR OR NOT FDNY_JSONC_LIBRARY)
      message(FATAL_ERROR "json-c development files were not found by pkg-config, CMake, or standard paths")
    endif()
    add_library(fdny_jsonc INTERFACE)
    target_include_directories(fdny_jsonc INTERFACE "${FDNY_JSONC_INCLUDE_DIR}")
    target_link_libraries(fdny_jsonc INTERFACE "${FDNY_JSONC_LIBRARY}")
    set(FDNY_JSONC_TARGET fdny_jsonc)
  endif()
endif()

find_package(OpenGL REQUIRED)

function(fdny_configure_native_target target)
  if(MSVC)
    # Keep floating-point operations separate like -ffp-contract=off.
    target_compile_options(${target} PRIVATE /fp:strict /W3)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(${target} PRIVATE -ffp-contract=off -Wno-deprecated-declarations)
  endif()
endfunction()

# The Training workbench's title UI uses Apple CoreText/CoreGraphics. The
# map-neutral viewer is the shared executable and includes the Training
# package through the same SceneData path as every rescue map.
if(APPLE)
  find_library(CORETEXT_FRAMEWORK CoreText REQUIRED)
  find_library(COREGRAPHICS_FRAMEWORK CoreGraphics REQUIRED)

  add_executable(fdny-training src/main.cpp)
  target_link_libraries(fdny-training PRIVATE
    ${FDNY_SDL_TARGET} ${FDNY_JSONC_TARGET} OpenGL::GL
    ${CORETEXT_FRAMEWORK} ${COREGRAPHICS_FRAMEWORK})
  target_compile_definitions(fdny-training PRIVATE
    FDNY_ASSETS="${CMAKE_CURRENT_SOURCE_DIR}/assets/training")
  fdny_configure_native_target(fdny-training)
endif()

# Level-neutral BSP host for Training, rescue missions, and linked stages.
add_executable(fdny-scene-viewer src/scene_viewer.cpp)
target_link_libraries(fdny-scene-viewer PRIVATE
  ${FDNY_SDL_TARGET} ${FDNY_JSONC_TARGET} OpenGL::GL)
if(WIN32)
  # Use a regular console main without requiring the SDL2main shim.
  target_compile_definitions(fdny-scene-viewer PRIVATE SDL_MAIN_HANDLED=1)
endif()
fdny_configure_native_target(fdny-scene-viewer)
