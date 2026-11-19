cmake_minimum_required(VERSION 3.10)
project(CampusActivityLab04 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CampusActivity
    src/Input.cpp
    src/User.cpp
    src/Activity.cpp
    src/Lecture.cpp
    src/Competition.cpp
    src/Sport.cpp
    src/ActivityManager.cpp
    src/main.cpp
)

target_include_directories(CampusActivity PRIVATE src)
