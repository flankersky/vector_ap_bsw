# Install script for directory: /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application/bin" TYPE EXECUTABLE FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/build/amsr-vector-fs-em-executionmanager-demo-application")
  if(EXISTS "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/opt/amsr-vector-fs-em-executionmanager-demo-application/bin/amsr-vector-fs-em-executionmanager-demo-application")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application/etc/manifest.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/etc/manifest.json")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application2/bin/amsr-vector-fs-em-executionmanager-demo-application2")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application2/bin" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ OWNER_EXECUTE GROUP_EXECUTE RENAME "amsr-vector-fs-em-executionmanager-demo-application2" FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/build/addon/amsr-vector-fs-em-executionmanager-demo-application/amsr-vector-fs-em-executionmanager-demo-application")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application2/etc/manifest.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application2/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME "manifest.json" FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/etc/manifest2.json")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application3/bin/amsr-vector-fs-em-executionmanager-demo-application3")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application3/bin" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ OWNER_EXECUTE GROUP_EXECUTE RENAME "amsr-vector-fs-em-executionmanager-demo-application3" FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/build/addon/amsr-vector-fs-em-executionmanager-demo-application/amsr-vector-fs-em-executionmanager-demo-application")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsr-vector-fs-em-executionmanager-demo-application3/etc/manifest.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsr-vector-fs-em-executionmanager-demo-application3/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME "manifest.json" FILES "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/etc/manifest3.json")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-demo-application/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
