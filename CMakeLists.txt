cmake_minimum_required(VERSION 3.3)

# Definitions
set(PROJECT_NAME_STR chugger)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


# Project title and define versioning
project(${PROJECT_NAME_STR} C CXX)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)


# Include paths
set(CRANK_INCLUDES ${CRANK_INCLUDES} ${PROJECT_SOURCE_DIR}/vendor)


# Build
set(SOURCE_FILES main.cpp csv.hpp)
add_executable(chugger ${SOURCE_FILES})