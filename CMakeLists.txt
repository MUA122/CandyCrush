cmake_minimum_required(VERSION 3.30)
project(CandyCrush)

set(CMAKE_CXX_STANDARD 20)

add_executable(CandyCrush main.cpp
        Node.h
        Node.cpp
        Config.h)
