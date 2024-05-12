cmake_minimum_required(VERSION 3.16)
project(yall)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_YALL_TESTS "Build project tests" TRUE)
option(SANITIZE_YALL_APPS "Build apps with sanitizer flags" FALSE)

add_library(yall INTERFACE)
target_include_directories(yall INTERFACE include)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(yall
    INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_subdirectory(apps)

if(${BUILD_YALL_TESTS})
  enable_testing()
  include(FetchContent)
  add_subdirectory(test)
endif()