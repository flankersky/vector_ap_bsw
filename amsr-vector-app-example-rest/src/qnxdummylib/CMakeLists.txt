###############################################################################
#    Model Element   : CMakeLists
#    Component       :
#    Copyright       : Copyright (C) 2017, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

# The libraries are already included in libc of QNX but are needed by Poco.
# Therefore we create empty dummy libraries to satisfy linker dependencies.

message(STATUS "Creating dummy libraries for QNX")
set(DUMMY_PTHREAD pthread)
set(DUMMY_RT rt)
set(DUMMY_DL dl)

add_library(${DUMMY_PTHREAD} STATIC ${CMAKE_CURRENT_LIST_DIR}/dummy.cc)
add_library(${DUMMY_RT} STATIC ${CMAKE_CURRENT_LIST_DIR}/dummy.cc)
add_library(${DUMMY_DL} STATIC ${CMAKE_CURRENT_LIST_DIR}/dummy.cc)
