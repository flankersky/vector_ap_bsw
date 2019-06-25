###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-per-libpersistency
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
set(LIBRARY_NAME persistency)

file(GLOB_RECURSE LIB_PERSISTENCY_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src *.cc)

# create a static library *.a
add_library(${LIBRARY_NAME} STATIC ${LIB_PERSISTENCY_SRCS})

find_package(vac REQUIRED)
if (vac_FOUND)
  target_link_libraries(${LIBRARY_NAME} ${VAC_LIBRARIES})
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC
  $<BUILD_INTERFACE:${RapidJSON_INCLUDE_DIRS}>
  $<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>
)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

install(
  TARGETS ${LIBRARY_NAME}
  EXPORT "${LIBRARY_NAME}Targets"
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets

export(TARGETS ${LIBRARY_NAME} FILE "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}/persistencyTargets.cmake")
# Store the current build directory in the CMake user package registry for package ${LIBRARY_NAME}
# The find_package command may consider the directory while searching for package ${LIBRARY_NAME}
export(PACKAGE ${LIBRARY_NAME})
install(EXPORT "${LIBRARY_NAME}Targets" DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME})

foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/lib/${LIBRARY_NAME}/include"
  ${RapidJSON_INCLUDE_DIRS}
)
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/persistencyConfig.cmake.in
  "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}/persistencyConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${PERSISTENCY_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/persistencyConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/persistencyConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/persistencyConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME}
  RENAME persistencyConfig.cmake
)
