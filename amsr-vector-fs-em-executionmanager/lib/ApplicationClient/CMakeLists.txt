###############################################################################
#    Model Element   : CMakeLists
#    Component       : ApplicationClient
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(PACKAGE_VERSION 0.0.1)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR lib)
set(INSTALL_INCLUDE_DIR include)
set(LIBRARY_NAME amsr-vector-fs-em-executionmanagement_application-client)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc")

file(GLOB_RECURSE ${LIBRARY_NAME}_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src *.cc)

#create a static library *.a
add_library(${LIBRARY_NAME} STATIC ${${LIBRARY_NAME}_SRCS})

find_package(ara-logging REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE ${ARA_LOGGING_LIBRARIES})
target_include_directories(${LIBRARY_NAME} PUBLIC
  "$<BUILD_INTERFACE:${ARA_LOGGING_INCLUDE_DIRS}>"
)

find_package(vac REQUIRED)
target_include_directories(${LIBRARY_NAME} PUBLIC
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
)

find_package(osabstraction REQUIRED)
if (osabstraction_FOUND)
  message(STATUS "Package osabstraction found: ${osabstraction_FOUND}")
  message(STATUS "OSABSTRACTION_INCLUDE_DIRS: ${OSABSTRACTION_INCLUDE_DIRS}")
  message(STATUS "OSABSTRACTION_LIBRARIES: ${OSABSTRACTION_LIBRARIES}")
  target_link_libraries(${LIBRARY_NAME} PRIVATE ${OSABSTRACTION_LIBRARIES})
  target_include_directories(${LIBRARY_NAME} PUBLIC
    "$<BUILD_INTERFACE:${OSABSTRACTION_INCLUDE_DIRS}>"
  )
endif ()

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

install(
  TARGETS ${LIBRARY_NAME}
  EXPORT "${LIBRARY_NAME}Targets"
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  COMPONENT dev
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/
  DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
)

# Export targets
# see https://cmake.org/Wiki/CMake:ExportInterface for details
export(TARGETS ${LIBRARY_NAME} FILE "${PROJECT_BINARY_DIR}/lib/ApplicationClient/${LIBRARY_NAME}Targets.cmake")
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

set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/inc")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/lib/ApplicationClient/${LIBRARY_NAME}Config.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${${LIBRARY_NAME}_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${LIBRARY_NAME}Config.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${LIBRARY_NAME}Config.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/${LIBRARY_NAME}
  RENAME ${LIBRARY_NAME}Config.cmake
)
