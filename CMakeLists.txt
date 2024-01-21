cmake_minimum_required(VERSION 3.6.0)

project(hstring LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "clang++")

add_executable(main 
    main.cpp
    hstring.h
    hstring.cpp
)