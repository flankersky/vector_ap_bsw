###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-libvac
#    Author          : vismbn <michael.baumann@vector.com>
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-libvac
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CXX_WARNING_OPTIONS "-Wall -Werror -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_OPTIONS}")
message(STATUS "-------------------------------------------------------------")
message(STATUS "C Compiler used: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler used: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard used: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler options: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "CMAKE_VERSION : ${CMAKE_VERSION}")
message(STATUS "-DCMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "-------------------------------------------------------------")

# User Options
message(STATUS "-------------------------------------------------------------")
message(STATUS "Option Status")
option(ENABLE_DOXYGEN "Create and install the HTML based API documentation " ON)
message(STATUS "option -DENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})
option(BUILD_TESTS "Build GTest-based unit tests" OFF)
message(STATUS "option -DBUILD_TESTS=" ${BUILD_TESTS})
option (ENABLE_STATIC_ANALYSIS "Enable static code analysis" OFF)
message(STATUS "option -DENABLE_STATIC_ANALYSIS=" ${ENABLE_STATIC_ANALYSIS})

message(STATUS "-------------------------------------------------------------")

# Documentation settings
if(ENABLE_DOXYGEN)
  add_subdirectory(doc)
endif()

# Clang tidy settings for static code analysis
# Refer to: https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/
if (ENABLE_STATIC_ANALYSIS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CLANG_TIDY_EXE run-clang-tidy.py)

  add_custom_target(hicpp-clang-tidy
    ALL
    COMMAND ${CLANG_TIDY_EXE} -checks="-*,cppcoreguidelines-*,hicpp-*" -header-filter=.*
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Executing static code analysis tool clang-tidy to be compliant with HIC++ coding guideline."
    VERBATIM
  )
endif ()

message(STATUS "-------------------------------------------------------------")

# Add subdirectories to the build.
add_subdirectory(lib)

if (BUILD_TESTS)
  message(STATUS "Tests are enabled")
  enable_testing()
  if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message(WARNING "CMAKE_BUILD_TYPE (=${CMAKE_BUILD_TYPE}) set to 'Debug' for proper code coverage test.")
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  add_subdirectory(test)
endif()
