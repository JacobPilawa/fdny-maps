cmake_minimum_required(VERSION 3.20)
project(fdny_training LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(JSON REQUIRED IMPORTED_TARGET json-c)
find_package(OpenGL REQUIRED)

add_executable(fdny-training src/main.cpp)
target_link_libraries(fdny-training PRIVATE PkgConfig::SDL PkgConfig::JSON OpenGL::GL)
target_compile_options(fdny-training PRIVATE -ffp-contract=off -Wno-deprecated-declarations)
target_compile_definitions(fdny-training PRIVATE FDNY_ASSETS="${CMAKE_CURRENT_SOURCE_DIR}/assets/training")

if(APPLE)
  find_library(CORETEXT_FRAMEWORK CoreText REQUIRED)
  find_library(COREGRAPHICS_FRAMEWORK CoreGraphics REQUIRED)
  target_link_libraries(fdny-training PRIVATE ${CORETEXT_FRAMEWORK} ${COREGRAPHICS_FRAMEWORK})
endif()
