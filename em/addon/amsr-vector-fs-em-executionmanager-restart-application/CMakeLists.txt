###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-em-executionmanager-restart-application
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(TARGET_NAME amsr-vector-fs-em-executionmanager-restart-application)

# Automatically add the current source- and build directories to the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Collect source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/addon *.cc)

# when moving this demo application into an own cmake projekt, we have to use find_package.
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)
find_package(vac REQUIRED)

include_directories(
  ${OSABSTRACTION_INCLUDE_DIRS}
)

add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} ${OSABSTRACTION_LIBRARIES})

# Install first instance of application
install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION /opt/${TARGET_NAME}/bin
  )
install(
  FILES etc/manifest.json
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION /opt/${TARGET_NAME}/etc
  )
message(STATUS "-------------------------------------------------------------")
