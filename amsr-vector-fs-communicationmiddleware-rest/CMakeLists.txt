###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-communicationmiddleware-rest
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-communicationmiddleware-rest
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CXX_WARNING_FLAGS "-Wall -Werror -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -pedantic")

if("${CMAKE_CXX_STANDARD}"  STREQUAL "11")
# Set preprocessor macro for QNX.
add_definitions(-D__STDC_VERSION__=201112L )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang, do not add specific flags
else()
  set(CXX_WARNING_OPTIONS "${CXX_WARNING_OPTIONS} -Wl,--no-as-needed")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")

message(STATUS "-------------------------------------------------------------")
message(STATUS "C Compiler used: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler used: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard used: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler options: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "-------------------------------------------------------------")

# User Options
message(STATUS "-------------------------------------------------------------")
message(STATUS "Option Status")
option(
    ENABLE_DOXYGEN
    "Create and install the HTML based API documentation "
    OFF
)
message(STATUS "option ENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})
option(
    BUILD_TESTS
    "Build GTest-based unit tests"
    OFF
)
message(STATUS "option BUILD_TESTS=" ${BUILD_TESTS})
option(
    SERVER_TESTS
    "Build the Server Tests which are non functional in the CI"
    OFF
)
message(STATUS "option SERVER_TESTS=" ${SERVER_TESTS})


message(STATUS "-------------------------------------------------------------")

# Documentation settings
if(ENABLE_DOXYGEN)
  add_subdirectory(doc)
endif()

message(STATUS "-------------------------------------------------------------")

# Find cmake packages.
find_package(ara-logging REQUIRED)
find_package(vac REQUIRED)
find_package(persistency REQUIRED)
find_package(Threads REQUIRED)
find_package(Poco REQUIRED COMPONENTS XML JSON Util Net Foundation)

# Add subdirectories to the build.
add_subdirectory(lib/AraRest)

if (BUILD_TESTS)
  message(STATUS "Tests are enabled")
  enable_testing()
  if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message(WARNING "CMAKE_BUILD_TYPE (=${CMAKE_BUILD_TYPE}) set to 'Debug' for proper code coverage test.")
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  add_subdirectory(test)
endif()
