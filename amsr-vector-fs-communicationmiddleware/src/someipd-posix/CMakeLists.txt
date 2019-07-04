###############################################################################
#    Model Element   : CMakeLists
#    Component       : SOME/IP daemon for POSIX OS
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

file(GLOB_RECURSE SOMEIPD_POSIX_SRCS ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB SOMEIPD_POSIX_LIB_SRCS
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/application/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/configuration/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/packet_router/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/service_discovery/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/service_discovery/connection_manager/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/service_discovery/state_machine/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/service_discovery/state_machine/server/*.cc
  ${PROJECT_SOURCE_DIR}/src/someipd-posix/service_discovery/state_machine/server/eventgroup/*.cc
)

set(INSTALL_CMAKE_DIR lib/cmake)
set(INSTALL_LIB_DIR src)
set(INSTALL_INCLUDE_DIR include/SomeIPd-posix)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(persistency REQUIRED)
find_package(vac REQUIRED)
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)


if (ENABLE_EXEC_MANAGER)
  set(VectorApplicationClient amsr-vector-fs-em-executionmanagement_application-client)
  find_package(${VectorApplicationClient} REQUIRED)
  set(ARA_EXEC_LIBRARIES ${${VectorApplicationClient}_LIBRARIES})
  message(STATUS "    Package ApplicationClient (Vector) found with libraries: ${ARA_EXEC_LIBRARIES}")
  set(ARA_EXEC_INCLUDE_DIRS ${${VectorApplicationClient}_INCLUDE_DIRS})
  set(ARA_EXEC_FLAGS "-DENABLE_EXEC_MANAGER_SUPPORT")
  set(MANIFEST_FILE "${PROJECT_SOURCE_DIR}/etc/manifest.json")
  set(MANIFEST_TARGET_FILENAME "manifest.json")
  set(EXEC_MANAGER_ENABLED true)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/
  ${PERSISTENCY_INCLUDE_DIRS})

set(SOMEIPD_POSIX_TARGET someipd-posix)
set(SOMEIPD_POSIX_CONFIGS ${PROJECT_SOURCE_DIR}/etc/local/someipd-posix.json
                          ${PROJECT_SOURCE_DIR}/etc/local/someipd-posix-gen.json)
set(SOMEIPD_POSIX_INSTALL_PREFIX /opt/${SOMEIPD_POSIX_TARGET})
if(NOT DEFINED SOMEIPD_POSIX_INSTALL_PREFIX)
  set(SOMEIPD_POSIX_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
  set(SOMEIPD_POSIX_INSTALL_SYSCONFDIR ${CMAKE_INSTALL_SYSCONFDIR})
  message(STATUS "someipd-posix install path: ${CMAKE_INSTALL_PREFIX}")
else()
  set(SOMEIPD_POSIX_INSTALL_BINDIR ${SOMEIPD_POSIX_INSTALL_PREFIX}/bin)
  set(SOMEIPD_POSIX_INSTALL_SYSCONFDIR ${SOMEIPD_POSIX_INSTALL_PREFIX}/etc)
  message(STATUS "someipd-posix install path: ${SOMEIPD_POSIX_INSTALL_PREFIX}")
endif()

add_executable(${SOMEIPD_POSIX_TARGET} ${SOMEIPD_POSIX_SRCS})
target_link_libraries(${SOMEIPD_POSIX_TARGET} PRIVATE
  SomeIP-posix-common
  ${VAC_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ARA_LOGGING_LIBRARIES}
  ${OSABSTRACTION_LIBRARIES}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  target_link_libraries(${SOMEIPD_POSIX_TARGET} PRIVATE socket)
endif()

if(EXEC_MANAGER_ENABLED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARA_EXEC_FLAGS}")
  target_link_libraries(${SOMEIPD_POSIX_TARGET} PRIVATE ${ARA_EXEC_LIBRARIES})
endif()

set(SOMEIPD_POSIX_LIB_TARGET SomeIP-posix-daemon)

add_library(${SOMEIPD_POSIX_LIB_TARGET} STATIC ${SOMEIPD_POSIX_LIB_SRCS})
target_link_libraries(${SOMEIPD_POSIX_LIB_TARGET} PRIVATE
  SomeIP-posix-common
  ${VAC_LIBRARIES}
  ${PERSISTENCY_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ARA_LOGGING_LIBRARIES}
)

# Export targets

install(
  TARGETS ${SOMEIPD_POSIX_TARGET}
  RUNTIME DESTINATION ${SOMEIPD_POSIX_INSTALL_BINDIR}
)
install(FILES ${MANIFEST_FILE} PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${SOMEIPD_POSIX_INSTALL_PREFIX}/etc/ RENAME ${MANIFEST_TARGET_FILENAME})

export(TARGETS ${SOMEIPD_POSIX_LIB_TARGET} FILE "${PROJECT_BINARY_DIR}/${SOMEIPD_POSIX_LIB_TARGET}Targets.cmake")


# The following line is important, other cmake packages can then add the includes with
# include_directories("${SOMEIPD_POSIX_INCLUDE_DIRS}")
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${SOMEIPD_POSIX_LIB_TARGET}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${SOMEIPD_POSIX_LIB_TARGET}Config.cmake" @ONLY
)

set(CONF_INCLUDE_DIRS "\${SOMEIPD_POSIX_CMAKE_DIR}/../${REL_INCLUDE_DIR}")
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/${SOMEIPD_POSIX_LIB_TARGET}Config.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${SOMEIPD_POSIX_LIB_TARGET}Config.toinstall.cmake" @ONLY)

install(
  FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${SOMEIPD_POSIX_LIB_TARGET}Config.toinstall.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/${SOMEIPD_POSIX_LIB_TARGET}
  RENAME ${SOMEIPD_POSIX_LIB_TARGET}Config.cmake
)
