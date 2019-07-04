###############################################################################
#    Model Element   : CMakeLists
#    Component       : SomeIP library
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
find_package(persistency REQUIRED)
find_package(vac REQUIRED)
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)

file(GLOB_RECURSE SOMEIP_POSIX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(SomeIP-posix STATIC ${SOMEIP_POSIX_SRCS})
target_link_libraries(SomeIP-posix PRIVATE
  ${VAC_LIBRARIES}
  ${OSABSTRACTION_LIBRARIES}
  ${ARA_LOGGING_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
  SomeIP-posix-common
)
target_link_libraries(SomeIP-posix PUBLIC
  SomeIP-posix-common
)

target_include_directories(SomeIP-posix PUBLIC
  "$<BUILD_INTERFACE:${PERSISTENCY_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${ARA_LOGGING_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${OSABSTRACTION_INCLUDE_DIRS}>"
)

target_include_directories(SomeIP-posix PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(
  TARGETS SomeIP-posix
  EXPORT SomeIP-posixTargets
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets

export(TARGETS SomeIP-posix FILE "${PROJECT_BINARY_DIR}/SomeIP-posixTargets.cmake")
install(EXPORT SomeIP-posixTargets DESTINATION ${INSTALL_CMAKE_DIR}/SomeIP-posix)

foreach(p LIB INCLUDE CMAKE)
set(var INSTALL_${p}_DIR)
if(NOT IS_ABSOLUTE "${${var}}")
  set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/libSomeIP-posix/include")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/SomeIP-posixConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SomeIP-posixConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${SOMEIP_POSIX_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/SomeIP-posixConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SomeIP-posixConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SomeIP-posixConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/SomeIP-posix
  RENAME SomeIP-posixConfig.cmake
)
