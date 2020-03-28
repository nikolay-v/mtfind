cmake_minimum_required(VERSION 3.0)

project(mtfind-project VERSION 0.1 LANGUAGES C CXX)

aux_source_directory(Source srcfiles)

add_executable(mtfind ${srcfiles})

target_include_directories(mtfind PUBLIC Headers)
