cmake_minimum_required(VERSION 3.24)
SET(CMAKE_CXX_STANDARD 23)

project(BMPWatermark LANGUAGES CXX)

add_executable(${PROJECT_NAME} ./main.cpp)