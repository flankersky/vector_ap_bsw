# Install script for directory: /home/user/AdaptiveEvalbundle/BSW/app-example-calculator/src/client

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
  if(EXISTS "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsrVectorAppExampleCalculatorClient/bin" TYPE EXECUTABLE FILES "/home/user/AdaptiveEvalbundle/BSW/app-example-calculator/build/src/client/amsrVectorAppExampleCalculatorClient")
  if(EXISTS "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/opt/amsrVectorAppExampleCalculatorClient/bin/amsrVectorAppExampleCalculatorClient")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsrVectorAppExampleCalculatorClient/etc/application.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsrVectorAppExampleCalculatorClient/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES "/home/user/AdaptiveEvalbundle/BSW/app-example-calculator/src-gen/etc/adaptiveCalculatorClient/application.json")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsrVectorAppExampleCalculatorClient/etc/application-gen.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsrVectorAppExampleCalculatorClient/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES "/home/user/AdaptiveEvalbundle/BSW/app-example-calculator/src-gen/adaptiveCalculatorClient/manifest/someip/application-gen.json")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/amsrVectorAppExampleCalculatorClient/etc/manifest.json")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/amsrVectorAppExampleCalculatorClient/etc" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME "manifest.json" FILES "/home/user/AdaptiveEvalbundle/BSW/app-example-calculator/src-gen/adaptiveCalculatorClient/manifest/em/amsrVectorAppExampleCalculatorClient/manifest.json")
endif()

