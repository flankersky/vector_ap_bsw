###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-em-executionmanager
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-em-executionmanager
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -Wshadow -pedantic -Wl,--no-as-needed")
message(STATUS "-------------------------------------------------------------")
message(STATUS "CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")
message(STATUS "C Compiler used: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler used: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard used: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler options: ${CMAKE_CXX_FLAGS}")
message(STATUS "-DCMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#find external packages like librarys

#User Options
message(STATUS "-------------------------------------------------------------")
message(STATUS "Option Status")
option(ENABLE_DOXYGEN "Create and install the HTML based API documentation." OFF)
message(STATUS "option -DENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})

option(ENABLE_ADDON "Build the demo application." OFF)
message(STATUS "option -DENABLE_ADDON=" ${ENABLE_ADDON})

option(BUILD_TESTS "Build the project's unit tests." OFF)
message(STATUS "option -DBUILD_TESTS=" ${BUILD_TESTS})

option(ENABLE_STATIC_ANALYSIS "Static code analysis (clang-tidy)" OFF)
message(STATUS "option -DENABLE_STATIC_ANALYSIS=" ${ENABLE_STATIC_ANALYSIS})

message(STATUS "-------------------------------------------------------------")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_ON)
endif()

# Documentation settings
if(ENABLE_DOXYGEN)
  add_subdirectory(doc)
endif()

message(STATUS "-------------------------------------------------------------")

# Clang tidy for static code analysis
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

add_definitions(-DVIRTUALMOCK=)

if (BUILD_TESTS)
# Add tests to the build
  enable_testing()
  add_subdirectory(test)
  set(ENABLE_ADDON ON)
  message(STATUS "because of option BUILD_TEST=ON the option ENABLE_ADDON=" ${ENABLE_ADDON})
endif()

# Add subdirectories to the build.
add_subdirectory(src)
add_subdirectory(lib/ApplicationClient)
add_subdirectory(lib/StateClient)

# Add subdirectories for the applications in addon to the build.
if (ENABLE_ADDON)
  add_subdirectory(addon/amsr-vector-fs-em-executionmanager-demo-application)
  add_subdirectory(addon/amsr-vector-fs-em-executionmanager-restart-application)
  add_subdirectory(addon/amsr-vector-fs-em-executionmanager-shutdown-application)
endif()

