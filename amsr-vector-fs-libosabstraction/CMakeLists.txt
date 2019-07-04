###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-libosabstraction
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-libosabstraction
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_SYSTEM_NAME MATCHES "PikeOS")
  # Some options were to restrictive for c code parts of lwip and libc
  set(CXX_WARNING_OPTIONS "${CXX_WARNING_OPTIONS} -Wall -Wextra -Wnon-virtual-dtor -Wno-error=conversion -Wno-error=shadow -pedantic -Werror -Wno-error=old-style-cast")
  # '-isystem' wraps system headers into 'extern "C"'. This leads to compilation issues. Disable it.
  set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-I")
else()
  set(CXX_WARNING_OPTIONS "${CXX_WARNING_OPTIONS} -Wall -Wextra -Wnon-virtual-dtor -Wno-error=conversion -Wshadow -pedantic -Werror -Wold-style-cast")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_OPTIONS}")


message(STATUS "-------------------------------------------------------------")
message(STATUS "C Compiler used: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler used: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard used: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler options: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_VERSION : ${CMAKE_VERSION}")
message(STATUS "-DCMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "-------------------------------------------------------------")

# Default to target OS
if(NOT DEFINED OS_NAME)
  string(TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)
endif()

if(OS_NAME MATCHES "linux" OR OS_NAME MATCHES "qnx")
  set(POSIX ON)
else()
  set(POSIX OFF)
endif()

# User Options
message(STATUS "-------------------------------------------------------------")
message(STATUS "Option Status")
message(STATUS "option -DOS_NAME=" ${OS_NAME})
option(ENABLE_DOXYGEN "Create and install the HTML based API documentation " OFF)
message(STATUS "option -DENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})
option(BUILD_TESTS "Build GTest-based unit tests" OFF)
message(STATUS "option -DBUILD_TESTS=" ${BUILD_TESTS})
option(ENABLE_STATIC_ANALYSIS "Static code analysis (clang-tidy)" OFF)
message(STATUS "option -DENABLE_STATIC_ANALYSIS=" ${ENABLE_STATIC_ANALYSIS})
message(STATUS "-------------------------------------------------------------")

# Documentation settings
if(ENABLE_DOXYGEN)
  add_subdirectory(doc)
endif()

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

# Add subdirectories to the build.
add_subdirectory(lib)

if (BUILD_TESTS)
  message(STATUS "Tests are enabled")
  add_subdirectory(test)
endif()
