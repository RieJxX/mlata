cmake_minimum_required(VERSION 3.10)

project(mlata)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    src/Aksiomes.cpp
    src/Constructor.cpp
    src/Modes_ponens.cpp
    src/New_aks.cpp
    task1.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(include)

add_executable(mlata ${SOURCE_FILES})