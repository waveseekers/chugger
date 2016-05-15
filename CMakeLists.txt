cmake_minimum_required(VERSION 3.3)

# Definitions
set(PROJECT_NAME_STR chugger)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Project title and define versioning
project(${PROJECT_NAME_STR} C CXX)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)


# Include paths
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")
set(CHUGGER_INCLUDES ${CHUGGER_INCLUDES} ${PROJECT_SOURCE_DIR}/src)
set(CHUGGER_INCLUDES ${CHUGGER_INCLUDES} ${PROJECT_SOURCE_DIR}/shared)
set(CHUGGER_INCLUDES ${CHUGGER_INCLUDES} ${PROJECT_SOURCE_DIR}/vendor)


# Gather the header and source files
file(GLOB_RECURSE INC_FILES ${PROJECT_SOURCE_DIR}/src/shared/*.hpp)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/shared/*.cpp)


##########################################################################
# Find "Boost" => "boost"
# => Boost_INCLUDE_DIRS		: include directories for Boost
# => Boost_LIBRARIES	    : Boost libraries
# => Boost_LIBRARY_DIRS	    : Boost libray directories
find_package(Boost COMPONENTS date_time REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if(NOT Boost_INCLUDE_DIR)
    message(FATAL_ERROR "Boost headers required to build project")
endif()


##########################################################################
# Find "Cassandra" => "cassandra"
# => CASSANDRA_INCLUDE_DIR		: include directory for Cassandra.h
# => CASSANDRA_LIBRARIES	    : Cassandra libraries
find_package(Cassandra)

if(NOT Cassandra_INCLUDE_DIR)
    message(FATAL_ERROR "Cassandra headers required to build project")
endif()


# Assign the include directories
include_directories(${CHUGGER_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})


# Build
set(SOURCE_FILES src/main.cpp ${INC_FILES} ${SRC_FILES})
add_executable(chugger ${SOURCE_FILES})
target_link_libraries(chugger ${Boost_LIBRARIES} ${Cassandra_LIBRARIES})