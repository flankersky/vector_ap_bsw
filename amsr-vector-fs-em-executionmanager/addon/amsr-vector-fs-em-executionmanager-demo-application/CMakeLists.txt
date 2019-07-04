###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-em-executionmanager-demo-application
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(TARGET_NAME amsr-vector-fs-em-executionmanager-demo-application)
set(TARGET_NAME2 ${TARGET_NAME}2)
set(TARGET_NAME3 ${TARGET_NAME}3)
set(MANIFEST1 etc/manifest.json)
set(MANIFEST2 etc/manifest2.json)
set(MANIFEST3 etc/manifest3.json)
set(MANIFEST_NAME manifest.json)

find_package(Threads REQUIRED)
message(STATUS "Package Threads found = ${Threads_FOUND}")

# Automatically add the current source- and build directories to the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Collect source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/addon/amsr-vector-fs-em-executionmanager-demo-application *.cc)

set(APPLICATION_CLIENT amsr-vector-fs-em-executionmanagement_application-client)
# when moving this demo application into an own cmake projekt, we have to use find_package.
#find_package(APPLICATION_CLIENT REQUIRED)
#include_directories(${${APPLICATION_CLIENT}_INCLUDE_DIRS})
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)
find_package(vac REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}/addon/amsr-vector-fs-em-executionmanager-demo-application/
  ${PROJECT_SOURCE_DIR}/lib/ApplicationClient/inc/
  ${PROJECT_SOURCE_DIR}/lib/StateClient/inc/
  ${ARA_LOGGING_INCLUDE_DIRS}
  ${OSABSTRACTION_INCLUDE_DIRS}
  ${VAC_INCLUDE_DIRS}
  )

add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} ${APPLICATION_CLIENT}) # use ${${APPLICATION_CLIENT}_LIBRARIES} when build in other cmake projects
target_link_libraries(${TARGET_NAME} amsr-vector-fs-em-executionmanagement_state-client)
target_link_libraries(${TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${TARGET_NAME} ${ARA_LOGGING_LIBRARIES})
target_link_libraries(${TARGET_NAME} ${OSABSTRACTION_LIBRARIES})
target_link_libraries(${TARGET_NAME} ${VAC_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(${TARGET_NAME} rt)
endif()

# Install first instance of application
install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION /opt/${TARGET_NAME}/bin
  )
install(
  FILES ${MANIFEST1}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION /opt/${TARGET_NAME}/etc
  )

# Install second instance of application
install(
  FILES ${PROJECT_BINARY_DIR}/addon/amsr-vector-fs-em-executionmanager-demo-application/${TARGET_NAME}
  PERMISSIONS OWNER_READ GROUP_READ OWNER_EXECUTE GROUP_EXECUTE
  DESTINATION /opt/${TARGET_NAME2}/bin
  RENAME ${TARGET_NAME2}
  )
install(
  FILES ${MANIFEST2}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION /opt/${TARGET_NAME2}/etc
  RENAME ${MANIFEST_NAME}
  )

# Install third instance of application
install(
  FILES ${PROJECT_BINARY_DIR}/addon/amsr-vector-fs-em-executionmanager-demo-application/${TARGET_NAME}
  PERMISSIONS OWNER_READ GROUP_READ OWNER_EXECUTE GROUP_EXECUTE
  DESTINATION /opt/${TARGET_NAME3}/bin
  RENAME ${TARGET_NAME3}
  )
install(
  FILES ${MANIFEST3}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION /opt/${TARGET_NAME3}/etc
  RENAME ${MANIFEST_NAME}
  )

message(STATUS "-------------------------------------------------------------")
