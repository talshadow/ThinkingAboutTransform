cmake_minimum_required(VERSION 2.8)

project(DoNotUseGuidelineWithoutBrain)
add_executable(${PROJECT_NAME} "main.cpp")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
