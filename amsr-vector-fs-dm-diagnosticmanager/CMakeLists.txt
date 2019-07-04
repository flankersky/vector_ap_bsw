###############################################################################
#    Model Element   : CMakeLists
#    Component       : DiagnosticManager
#    Copyright       : Copyright (C) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-fs-dm-diagnosticmanager
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CXX_WARNING_OPTIONS "-Wall -Werror -Wnon-virtual-dtor -Wconversion -Wold-style-cast -Wshadow -pedantic -Wl,--no-as-needed -Werror=maybe-uninitialized")
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
    OFF
)
message(STATUS "option -DENABLE_STATIC_ANALYSIS=" ${ENABLE_STATIC_ANALYSIS})

message(STATUS "option -DENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})

option (ENABLE_EXEC_MANAGER_AUTOSAR "Enable AUTOSAR Execution Manager support" OFF)
message(STATUS "option -DENABLE_EXEC_MANAGER_AUTOSAR=" ${ENABLE_EXEC_MANAGER_AUTOSAR})

option (ENABLE_EXEC_MANAGER_VECTOR "Enable Vector Execution Manager support" OFF)
message(STATUS "option -DENABLE_EXEC_MANAGER_VECTOR=" ${ENABLE_EXEC_MANAGER_VECTOR})

if (ENABLE_EXEC_MANAGER_VECTOR AND ENABLE_EXEC_MANAGER_AUTOSAR)
  message(FATAL_ERROR "Use Vector or AUTOSAR executionmanager support")
endif()

option(
    BUILD_TESTS
    "Build GTest-based unit tests."
    OFF
)
message(STATUS "option -DBUILD_TESTS=" ${BUILD_TESTS})

option(
    BUILD_DIAG_TEST_APP
    "Build amsr-vector-fs-dm-diagnosticmanager-diag-test-app"
    ON
)
message(STATUS "option -DBUILD_DIAG_TEST_APP=" ${BUILD_DIAG_TEST_APP})

option(
    BUILD_MEDIATOR
    "Build Diag Mediator."
    OFF
)
message(STATUS "option -DBUILD_MEDIATOR=" ${BUILD_MEDIATOR})
message(STATUS "-------------------------------------------------------------")

set(TRANSPORT_PROTOCOL_PATH "${PROJECT_SOURCE_DIR}/lib/libDoIP" CACHE PATH "Location to transport protocol sources")
message(STATUS "Location to transport protocol sources: TRANSPORT_PROTOCOL_PATH = ${TRANSPORT_PROTOCOL_PATH}")
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

# Add new preprocessors definition
if (BUILD_TESTS)
    add_definitions(-DVIRTUALMOCK=virtual)
else()
    add_definitions(-DVIRTUALMOCK=)
endif()

# Add subdirectories to the build.
add_subdirectory(lib/libDM)
add_subdirectory(${TRANSPORT_PROTOCOL_PATH})
add_subdirectory(src)

if(BUILD_DIAG_TEST_APP)
  message(STATUS "DiagTestApp is enabled")
  add_subdirectory(addon/DiagTestApp/src)
endif()

if(BUILD_MEDIATOR)
  message(STATUS "DiagTestMediator is enabled")
  add_subdirectory(addon/DiagTestMediator/src)
endif()

if (BUILD_TESTS)
  message(STATUS "Tests are enabled")
  enable_testing()
  if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message(WARNING "CMAKE_BUILD_TYPE (=${CMAKE_BUILD_TYPE}) set to 'Debug' for proper code coverage test.")
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  add_subdirectory(test)
  add_subdirectory(test/addon/DiagTestApp)
  add_subdirectory(test/addon/DiagTestMediator)
  add_subdirectory(test/lib/libDoIP)
endif()
