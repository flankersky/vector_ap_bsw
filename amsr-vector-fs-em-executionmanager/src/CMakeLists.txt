###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-em-executionmanager
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(TARGET_NAME amsr-vector-fs-em-executionmanager)

# Automatically add the current source- and build directories to the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Collect source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src *.cc)

add_executable(${TARGET_NAME} ${SRCS})
install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR}
  )

install(
  FILES ${PROJECT_SOURCE_DIR}/etc/machine_manifest.json ${PROJECT_SOURCE_DIR}/etc/machine_manifest_schema.json  ${PROJECT_SOURCE_DIR}/etc/application_manifest_schema.json
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION /etc/
  )

message(STATUS "-------------------------------------------------------------")

find_package(persistency REQUIRED)
if (persistency_FOUND)
  target_link_libraries(${TARGET_NAME} ${PERSISTENCY_LIBRARIES})
endif ()

find_package(vac REQUIRED)
if (vac_FOUND)
  target_link_libraries(${TARGET_NAME} ${VAC_LIBRARIES})
endif ()

message(STATUS "Package vac found: ${vac_FOUND}")
message(STATUS "VAC_INCLUDE_DIRS: ${VAC_INCLUDE_DIRS}")
message(STATUS "VAC_LIBRARIES: ${VAC_LIBRARIES}")

find_package(ara-logging REQUIRED)
if (ara-logging_FOUND)
  target_link_libraries(${TARGET_NAME} ${ARA_LOGGING_LIBRARIES})
endif ()

find_package(osabstraction REQUIRED)
if (osabstraction_FOUND)
  target_link_libraries(${TARGET_NAME} ${OSABSTRACTION_LIBRARIES})
endif ()


# rt (real time library) needed for posix message queues
target_link_libraries(${TARGET_NAME} amsr-vector-fs-em-executionmanagement_state-client)

include_directories(
  ${PROJECT_SOURCE_DIR}/lib/ApplicationClient/inc
  ${PROJECT_SOURCE_DIR}/lib/StateClient/inc
  ${PERSISTENCY_INCLUDE_DIRS}
  ${VAC_INCLUDE_DIRS}
  ${ARA_LOGGING_INCLUDE_DIRS}
  ${OSABSTRACTION_INCLUDE_DIRS}
)
