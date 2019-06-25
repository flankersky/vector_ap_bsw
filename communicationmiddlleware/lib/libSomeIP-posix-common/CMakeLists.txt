###############################################################################
#    Model Element   : CMakeLists
#    Component       : SomeIP Posix common parts library
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
find_package(ara-logging REQUIRED)
find_package(vac REQUIRED)
find_package(osabstraction REQUIRED)

file(GLOB_RECURSE SOMEIP_POSIX_COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(SomeIP-posix-common STATIC ${SOMEIP_POSIX_COMMON_SRCS})

target_link_libraries(SomeIP-posix-common PRIVATE
  ${ARA_LOGGING_LIBRARIES}
  ${VAC_LIBRARIES}
  ${OSABSTRACTION_LIBRARIES}
)

target_include_directories(SomeIP-posix-common PUBLIC
  "$<BUILD_INTERFACE:${ARA_LOGGING_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${OSABSTRACTION_INCLUDE_DIRS}>"
)

target_include_directories(SomeIP-posix-common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(
  TARGETS SomeIP-posix-common
  EXPORT SomeIP-posix-commonTargets
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets

export(TARGETS SomeIP-posix-common FILE "${PROJECT_BINARY_DIR}/SomeIP-posix-commonTargets.cmake")
install(EXPORT SomeIP-posix-commonTargets DESTINATION ${INSTALL_CMAKE_DIR}/SomeIP-posix-common)

foreach(p LIB INCLUDE CMAKE)
set(var INSTALL_${p}_DIR)
if(NOT IS_ABSOLUTE "${${var}}")
  set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/libSomeIP-posix-common/include")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/SomeIP-posix-commonConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SomeIP-posix-commonConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${SOMEIP_POSIX_COMMON_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/SomeIP-posix-commonConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SomeIP-posix-commonConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SomeIP-posix-commonConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/SomeIP-posix-common
  RENAME SomeIP-posix-commonConfig.cmake
)
