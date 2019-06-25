# Install script for directory: /home/wsj/workspace/vector_ap/DATA/BSW/em/lib/StateClient

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/wsj/workspace/vector_ap/DATA/BSW/em/build/lib/StateClient/libamsr-vector-fs-em-executionmanagement_state-client.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/wsj/workspace/vector_ap/DATA/BSW/em/lib/StateClient/inc/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets.cmake"
         "/home/wsj/workspace/vector_ap/DATA/BSW/em/build/lib/StateClient/CMakeFiles/Export/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client" TYPE FILE FILES "/home/wsj/workspace/vector_ap/DATA/BSW/em/build/lib/StateClient/CMakeFiles/Export/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client" TYPE FILE FILES "/home/wsj/workspace/vector_ap/DATA/BSW/em/build/lib/StateClient/CMakeFiles/Export/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client/amsr-vector-fs-em-executionmanagement_state-clientTargets-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/amsr-vector-fs-em-executionmanagement_state-client" TYPE FILE RENAME "amsr-vector-fs-em-executionmanagement_state-clientConfig.cmake" FILES "/home/wsj/workspace/vector_ap/DATA/BSW/em/build/CMakeFiles/amsr-vector-fs-em-executionmanagement_state-clientConfig.toinstall.cmake")
endif()
