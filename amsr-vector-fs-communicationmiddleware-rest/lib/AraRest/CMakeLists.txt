###############################################################################
#    Model Element   : CMakeLists
#    Component       : AraRest
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include/AraRest)
set(LIBRARY_NAME AraRest)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )

file(GLOB_RECURSE ARAREST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src *.cc *.h)

#create a static library *.a
add_library(${LIBRARY_NAME} STATIC ${ARAREST_SRCS})
target_link_libraries(${LIBRARY_NAME}
  ${VAC_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
  ${ARA_LOGGING_LIBRARIES}
  ${Poco_LIBRARIES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${PERSISTENCY_INCLUDE_DIRS}>"
)

include_directories(
  "${VAC_INCLUDE_DIRS}"
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

install(
  TARGETS ${LIBRARY_NAME}
  EXPORT "${LIBRARY_NAME}Targets"
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets
# see https://cmake.org/Wiki/CMake:ExportInterface for details
export(TARGETS ${LIBRARY_NAME} FILE "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}/AraRestTargets.cmake")
# Store the current build directory in the CMake user package registry for package ${LIBRARY_NAME}
# The find_package command may consider the directory while searching for package ${LIBRARY_NAME}
export(PACKAGE ${LIBRARY_NAME})
install(EXPORT "${LIBRARY_NAME}Targets" DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME})

# creates absolute paths for three varaibles: INSTALL_LIB_DIR, INSTALL_INCLUDE_DIR, INSTALL_CMAKE_DIR
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/${LIBRARY_NAME}/include")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/AraRestConfig.cmake.in
  "${PROJECT_BINARY_DIR}/lib/${LIBRARY_NAME}/AraRestConfig.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${ARA_REST_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/AraRestConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AraRestConfig.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AraRestConfig.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME}
  RENAME AraRestConfig.cmake
)
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

message(STATUS "-------------------------------------------------------------")

add_custom_command(
    TARGET ${LIBRARY_NAME} POST_BUILD
    COMMENT "create symlinks and directories for testing"
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/etc/
    COMMAND ln -sf ${PROJECT_SOURCE_DIR}/etc/configuration.json ${PROJECT_BINARY_DIR}/etc/configuration.json
)
