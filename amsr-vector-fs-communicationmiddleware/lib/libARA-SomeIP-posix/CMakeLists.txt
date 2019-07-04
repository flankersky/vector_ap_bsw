###############################################################################
#    Model Element   : CMakeLists
#    Component       : ARA-SomeIP-posix library
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

file(GLOB_RECURSE ARA_SOMEIP_POSIX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(ARA-SomeIP-posix STATIC ${ARA_SOMEIP_POSIX_SRCS})

target_link_libraries(ARA-SomeIP-posix PRIVATE
  ${VAC_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ARA_LOGGING_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
  SomeIP-posix
  ARA
)

target_include_directories(ARA-SomeIP-posix PUBLIC
  "$<BUILD_INTERFACE:${PERSISTENCY_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
)

target_include_directories(ARA-SomeIP-posix PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libARA/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libSomeIP-posix/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libSomeIP-posix-common/include>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(
  TARGETS ARA-SomeIP-posix
  EXPORT ARA-SomeIP-posixTargets
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets

export(TARGETS ARA-SomeIP-posix FILE "${PROJECT_BINARY_DIR}/ARA-SomeIP-posixTargets.cmake")
install(EXPORT ARA-SomeIP-posixTargets DESTINATION ${INSTALL_CMAKE_DIR}/ARA-SomeIP-posix)

foreach(p LIB INCLUDE CMAKE)
set(var INSTALL_${p}_DIR)
if(NOT IS_ABSOLUTE "${${var}}")
  set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/libARA-SomeIP-posix/include")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/ARA-SomeIP-posixConfig.cmake.in
  "${PROJECT_BINARY_DIR}/ARA-SomeIP-posixConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${ARA_SOMEIP_POSIX_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/ARA-SomeIP-posixConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ARA-SomeIP-posixConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ARA-SomeIP-posixConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/ARA-SomeIP-posix
  RENAME ARA-SomeIP-posixConfig.cmake
)
