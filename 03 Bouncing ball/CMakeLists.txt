cmake_minimum_required(VERSION 3.21)
project(03_Bouncing_ball)

set(CMAKE_CXX_STANDARD 14)

add_executable(03_Bouncing_ball main.cpp)
target_include_directories(03_Bouncing_ball PUBLIC glm)