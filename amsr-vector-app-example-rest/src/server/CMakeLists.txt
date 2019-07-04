
set(TARGET_NAME amsr-vector-app-example-rest-server)
set(MANIFEST ${CMAKE_CURRENT_LIST_DIR}/${MANIFEST_FILE})
set(CONFIGFILE ${CMAKE_CURRENT_LIST_DIR}/etc/configuration.json)

IF (NOT CMAKE_INSTALL_BINDIR_SERVER)
  set(CMAKE_INSTALL_BINDIR_SERVER ${CMAKE_INSTALL_BINDIR})
ENDIF()

IF (NOT CMAKE_INSTALL_SYSCONFDIR_SERVER)
  set(CMAKE_INSTALL_SYSCONFDIR_SERVER ${CMAKE_INSTALL_SYSCONFDIR})
ENDIF()


message(STATUS "Configuring target: ${TARGET_NAME}")

# Automatically add the current source- and build directories to the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Collect source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/server *.cc)

add_executable(${TARGET_NAME} ${SRCS})

if(ENABLE_EXEC_MANAGER)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARA_EXEC_FLAGS}")
  target_link_libraries(${TARGET_NAME} ${ARA_EXEC_LIBRARIES})
  target_include_directories(${TARGET_NAME} PRIVATE
    "$<BUILD_INTERFACE:${ARA_EXEC_INCLUDE_DIRS}>"
  )
endif()

target_link_libraries(${TARGET_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ARA_REST_LIBRARIES}
  ${ARA_LOGGING_LIBRARIES}
  ${VAC_LIBRARIES}
  )

target_include_directories(${TARGET_NAME} PRIVATE
  "$<BUILD_INTERFACE:${VAC_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${ARA_REST_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${ARA_LOGGING_INCLUDE_DIRS}>"
)

install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR_SERVER}
  )

if (ENABLE_EXEC_MANAGER)
    install(
      FILES ${MANIFEST}
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      DESTINATION ${CMAKE_INSTALL_SYSCONFDIR_SERVER}
      )
endif()

install(
  FILES ${CONFIGFILE}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR_SERVER}
  )

  # Create a symlink in the build directory to the configuration file
add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMENT "Creating symlink for ${TARGET_NAME} to configuration file."
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/etc/
  COMMAND ln -sf ${CONFIGFILE} ${CMAKE_CURRENT_BINARY_DIR}/etc/configuration.json
)

message(STATUS "Configuring target: ${TARGET_NAME} - Done")
message(STATUS "-------------------------------------------------------------")
