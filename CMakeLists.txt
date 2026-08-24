cmake_minimum_required(VERSION 3.16)
project(QtCalculator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(QtCalculator
    main.cpp
    calculator.cpp
    calculator.h
)

target_link_libraries(QtCalculator PRIVATE Qt6::Widgets)