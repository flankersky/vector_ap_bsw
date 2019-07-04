###############################################################################
#  Model Element : CMakeLists
#  Component     : amsr-vector-fs-osabstraction
#  Copyright     : Copyright (c) 2018, Vector Informatik GmbH.
#  File Name     : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
set(LIBRARY_NAME osabstraction)
set(OSABSTRACTION_LIBRARIES ${LIBRARY_NAME})
set(OSABSTRACTION_NAME ${OS_NAME})

if(POSIX)
  set(POSIX_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/lib/libosabstraction-posix/include)
  message(STATUS "POSIX_INCLUDE_DIRS: ${POSIX_INCLUDE_DIRS}")
endif()

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-interface/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-${OS_NAME}/include"
  ${POSIX_INCLUDE_DIRS}
)

# TODO(visasl): Refactor CMakeLists.txt. Split this file into several OS specific CMakeLists files.
if(OS_NAME STREQUAL "pikeos")
  include_directories(
    # Allow using ipc driver headers
    "${PROJECT_SOURCE_DIR}/driver/${OS_NAME}/ipc_driver/include"
    # Allow using ddk driver headers
    "$(CUSTOM_POOL_DIR)/driver/include"
    "$(PIKEOS_POOL_DIR)/driver/include"
    # Allow using psp headers
    "$(CUSTOM_POOL_DIR)/psp/include"
    "$(PIKEOS_POOL_DIR)/psp/include"
    # Allow using kerneldriver headers
    "$(CUSTOM_POOL_DIR)/kerneldriver/include"
    "$(PIKEOS_POOL_DIR)/kerneldriver/include"
    # Allow using asp headers
    "$(CUSTOM_POOL_DIR)/include/asp"
    "$(PIKEOS_POOL_DIR)/include/asp"
    # Allow using kernel headers
    "$(CUSTOM_POOL_DIR)/include/kernel"
    "$(PIKEOS_POOL_DIR)/include/kernel"
    # Standard headers
    "$(CUSTOM_POOL_DIR)/include"
    "$(PIKEOS_POOL_DIR)/include"
    )
endif()


file(GLOB_RECURSE
  LIB_OSABSTRACTION_SRCS
  ${PROJECT_SOURCE_DIR}/lib/libosabstraction-interface/src/*.cc
  ${PROJECT_SOURCE_DIR}/lib/libosabstraction-${OS_NAME}/src/*.cc
)

if(POSIX)
  file(GLOB_RECURSE
    LIB_OSABSTRACTION_POSIX_SRCS
    ${PROJECT_SOURCE_DIR}/lib/libosabstraction-posix/src/*.cc
  )
  LIST(APPEND LIB_OSABSTRACTION_SRCS "${LIB_OSABSTRACTION_POSIX_SRCS}")
endif()

# Create a static library *.a
add_library(${LIBRARY_NAME} STATIC ${LIB_OSABSTRACTION_SRCS})

# find external packages
find_package(vac REQUIRED)
if (vac_FOUND)
  message(STATUS "Package vac found: ${vac_FOUND}")
  message(STATUS "VAC_INCLUDE_DIRS: ${VAC_INCLUDE_DIRS}")
  message(STATUS "VAC_LIBRARIES: ${VAC_LIBRARIES}")
  include_directories(${VAC_INCLUDE_DIRS})
  target_link_libraries(${LIBRARY_NAME} ${VAC_LIBRARIES})
endif ()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(${LIBRARY_NAME} "rt")
endif()

if(OS_NAME STREQUAL "gmock")
  find_package(GTest REQUIRED)
  find_package(Threads REQUIRED)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-${OS_NAME}")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  target_link_libraries(${LIBRARY_NAME} mq)
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-interface/include/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-posix/include/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-${OS_NAME}/include/>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

install(
  TARGETS ${LIBRARY_NAME}
  EXPORT "${LIBRARY_NAME}Targets"
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-interface/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-${OS_NAME}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)
if(POSIX)
  install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libosabstraction-posix/include/
    DESTINATION ${INSTALL_INCLUDE_DIR}
    COMPONENT dev
  )
endif()

# Export targets to be able to use the library in other cmake projects
# see https://cmake.org/Wiki/CMake:ExportInterface for details
export(TARGETS ${LIBRARY_NAME} FILE "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}Targets.cmake")
# Store the current build directory in the CMake user package registry for package ${LIBRARY_NAME}
# The find_package command may consider the directory while searching for package ${LIBRARY_NAME}
# This creates an folder ${LIBRARY_NAME} in the ~/.cmake/packages directory with a file containing
# the path to the directory containing this CMakeLists.txt
export(PACKAGE ${LIBRARY_NAME})
install(EXPORT "${LIBRARY_NAME}Targets" DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME})

foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")
if(POSIX)
  set(CONF_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/lib/lib${LIBRARY_NAME}-interface/include"
    "${PROJECT_SOURCE_DIR}/lib/lib${LIBRARY_NAME}-${OS_NAME}/include"
    "${PROJECT_SOURCE_DIR}/lib/lib${LIBRARY_NAME}-posix/include"
  )
else()
  set(CONF_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/lib/lib${LIBRARY_NAME}-interface/include"
    "${PROJECT_SOURCE_DIR}/lib/lib${LIBRARY_NAME}-${OS_NAME}/include"
  )
endif()
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}Config.cmake" @ONLY
)
# ${OSABSTRACTION_CMAKE_DIR} is generated by osabstractionConfig.cmake.in
set(CONF_INCLUDE_DIRS "\${OSABSTRACTION_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${LIBRARY_NAME}Config.toinstall.cmake" @ONLY
)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${LIBRARY_NAME}Config.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME}
  RENAME ${LIBRARY_NAME}Config.cmake
)
