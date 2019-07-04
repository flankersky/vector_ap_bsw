###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-app-example-rest
#    Copyright       : Copyright (C) 2017, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.4)

project(
  amsr-vector-app-example-rest
  LANGUAGES CXX
  VERSION "1.0.0"
)

include(GNUInstallDirs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CXX_WARNING_FLAGS "-Werror -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -pedantic -Wl,--no-as-needed")

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
option(
    ENABLE_DOXYGEN
    "Create and install the HTML based API documentation "
    ON
)
message(STATUS "option ENABLE_DOXYGEN=" ${ENABLE_DOXYGEN})

option(
    ENABLE_EXEC_MANAGER
    "Enable Vector Execution Manager support"
    OFF
)
message(STATUS "option ENABLE_EXEC_MANAGER=" ${ENABLE_EXEC_MANAGER})

# Documentation settings
if(ENABLE_DOXYGEN)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen not found. It is needed to build the documentation")
  endif()
  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/config/Doxyfile)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${doxyfile_in} ${doxyfile} @ONLY)
  # make target to generate API documentation with doxygen
  add_custom_target(documentation
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html2 DESTINATION share/doc)
endif()

message(STATUS "-------------------------------------------------------------")

if(ENABLE_EXEC_MANAGER)
  # Import ApplicationClient
  message(STATUS "Importing EM ApplicationClient")

  set(VectorApplicationClient amsr-vector-fs-em-executionmanagement_application-client)
  find_package(${VectorApplicationClient} REQUIRED)


  set(ARA_EXEC_LIBRARIES ${${VectorApplicationClient}_LIBRARIES})
  message(STATUS "    Package ApplicationClient (Vector) found with libraries: ${ARA_EXEC_LIBRARIES}")
  set(ARA_EXEC_INCLUDE_DIRS ${${VectorApplicationClient}_INCLUDE_DIRS})
  set(ARA_EXEC_FLAGS "-DENABLE_EXEC_MANAGER")
  set(MANIFEST_FILE "etc/manifest.json")
endif()

# Add subdirectories to the build.
add_subdirectory(src)
