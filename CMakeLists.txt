cmake_minimum_required(VERSION 3.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(mtfind-project VERSION 0.1 LANGUAGES C CXX)

file(GLOB srcfiles
    Headers/*.h
    Source/*.cpp
)

add_executable(mtfind ${srcfiles})

target_include_directories(mtfind PUBLIC Headers)
