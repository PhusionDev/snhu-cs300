cmake_minimum_required(VERSION 3.0.0)
# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
cmake_policy(SET CMP0025 NEW)
endif ()
set (CMAKE_CXX_STANDARD 17)
project(CS300-Hodgkins_J-Project2 VERSION 0.1.0)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY})
add_subdirectory(src)
