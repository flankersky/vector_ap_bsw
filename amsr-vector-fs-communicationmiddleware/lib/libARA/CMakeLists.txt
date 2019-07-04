###############################################################################
#    Model Element   : CMakeLists
#    Component       : ARA library
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
find_package(vac REQUIRED)
find_package(persistency REQUIRED)
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)

file(GLOB_RECURSE ARA_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(ARA STATIC ${ARA_SRCS})

target_link_libraries(ARA PRIVATE
  ${VAC_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ARA_LOGGING_LIBRARIES}
  ${OSABSTRACTION_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
)

target_include_directories(ARA PUBLIC
  "$<BUILD_INTERFACE:${PERSISTENCY_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${OSABSTRACTION_INCLUDE_DIRS}>"
)

target_include_directories(ARA PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(
  TARGETS ARA
  EXPORT ARATargets
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets

export(TARGETS ARA FILE "${PROJECT_BINARY_DIR}/ARATargets.cmake")
install(EXPORT ARATargets DESTINATION ${INSTALL_CMAKE_DIR}/ARA)

foreach(p LIB INCLUDE CMAKE)
set(var INSTALL_${p}_DIR)
if(NOT IS_ABSOLUTE "${${var}}")
  set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/libARA/include")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/ARAConfig.cmake.in
  "${PROJECT_BINARY_DIR}/ARAConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${ARA_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/ARAConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ARAConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ARAConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/ARA
  RENAME ARAConfig.cmake
)
