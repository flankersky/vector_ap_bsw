# Install script for directory: /home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/libSomeIP-posix-common.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/include/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets.cmake"
         "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/CMakeFiles/Export/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common" TYPE FILE FILES "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/CMakeFiles/Export/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common" TYPE FILE FILES "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/CMakeFiles/Export/lib/cmake/SomeIP-posix-common/SomeIP-posix-commonTargets-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SomeIP-posix-common" TYPE FILE RENAME "SomeIP-posix-commonConfig.cmake" FILES "/home/user/AdaptiveEvalbundle/BSW/communicationmiddlleware/CMakeFiles/SomeIP-posix-commonConfig.toinstall.cmake")
endif()
