cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0054 NEW)

project (clone)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify bin path
set (EXECUTABLE_OUTPUT_PATH bin/)

# Create the executable from sources
add_executable(clone ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
