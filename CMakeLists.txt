cmake_minimum_required(VERSION 4.2)
project(untitled1)

set(CMAKE_CXX_STANDARD 26)

add_executable(untitled1 main.cpp
        cppfeatures/structuredbinding.h
        CommonIncludes.h
        cppfeatures/tuple.h
        cppfeatures/invoke.h
        cppfeatures/constexpr.h)
