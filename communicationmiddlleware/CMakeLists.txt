###############################################################################
#    Model Element   : CMakeLists
#    Component       : Communication middleware
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-communicationmiddleware
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CXX_WARNING_OPTIONS "${CXX_WARNING_OPTIONS} -Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wshadow -pedantic -Werror -Wold-style-cast")

set(CXX_WARNING_OPTIONS "${CXX_WARNING_OPTIONS} -Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wshadow -pedantic -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_OPTIONS}")

message(STATUS "-------------------------------------------------------------")
message(STATUS "C Compiler used: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler used: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard used: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler options: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "-------------------------------------------------------------")

# User Options
message(STATUS "-------------------------------------------------------------")
message(STATUS "Option Status")
option(
    ENABLE_DOXYGEN
    "Create and install the HTML based API documentation "
    ON
)
message(STATUS "option -DENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})
option(
    BUILD_TESTS
    "Build unit and integration tests"
    OFF
)
message(STATUS "option -DBUILD_TESTS=" ${BUILD_TESTS})

option (ENABLE_PROFILING "Enable profiling" OFF)
if (ENABLE_PROFILING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif ()
message(STATUS "option -DENABLE_PROFILING=" ${ENABLE_PROFILING})

option (ENABLE_ASAN "Enable address sanitizer" OFF)
if (ENABLE_ASAN)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif ()
message(STATUS "option -DENABLE_ASAN=" ${ENABLE_ASAN})

option (ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)
message(STATUS "option -DENABLE_STATIC_ANALYSIS=" ${ENABLE_STATIC_ANALYSIS})

option (ENABLE_EXEC_MANAGER "Enable Vector Execution Manager support" OFF)
message(STATUS "option -DENABLE_EXEC_MANAGER=" ${ENABLE_EXEC_MANAGER})

if(EXISTS ${PROJECT_SOURCE_DIR}/generator)
  option(ENABLE_CODE_GENERATION "Enable code generation (includes SIP download, generator build and code generation)." ON)
else()
  # Disable code generation. Generator does not exists.
  set(ENABLE_CODE_GENERATION OFF)
endif()
message(STATUS "option -DENABLE_CODE_GENERATION=${ENABLE_CODE_GENERATION}")
if(ENABLE_CODE_GENERATION)
    # AraApp SIP version used for code generation
    set(GENERATOR_SIP_VERSION "20.00.60")
endif()

message(STATUS "-------------------------------------------------------------")

# Documentation settings
if(ENABLE_DOXYGEN)
  add_subdirectory(doc)
endif()

# Clang tidy settings for static code analysis
# Refer to: https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/
option (ENABLE_STATIC_ANALYSIS "Enable static code analysis" OFF)
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

message(STATUS "Importing Threads")
find_package(Threads REQUIRED)
message(STATUS "CMAKE_THREAD_LIBS_INIT=${CMAKE_THREAD_LIBS_INIT}")

add_subdirectory(src/someipd-posix)
add_subdirectory(lib/libSomeIP-posix-common)
add_subdirectory(lib/libARA)
add_subdirectory(lib/libARA-SomeIP-posix)
add_subdirectory(lib/libSomeIP-posix)

if(ENABLE_CODE_GENERATION)
    message(STATUS "Code generation enabled. SIP version: ${GENERATOR_SIP_VERSION}")
    add_subdirectory(generator)
endif()

if (BUILD_TESTS)
  message(STATUS "Tests are enabled")
  add_subdirectory(test)
endif()

export(PACKAGE SomeIP-posix-common)
export(PACKAGE SomeIP-posix)
export(PACKAGE ARA)
export(PACKAGE ARA-SomeIP-posix)
export(PACKAGE SomeIP-posix-daemon)

#Copy etc
file(COPY "etc/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/etc/")
