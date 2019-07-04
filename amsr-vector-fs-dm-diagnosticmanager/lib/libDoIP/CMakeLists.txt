###############################################################################
#    Model Element   : CMakeLists
#    Component       : DiagnosticManager
#    Copyright       : Copyright (C) 2017, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################
## Collect sources
file(GLOB_RECURSE TRANSPORT_PROTOCOL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
set(TRANSPORT_PROTOCOL_SRCS ${TRANSPORT_PROTOCOL_SRCS} PARENT_SCOPE)
set(TRANSPORT_PROTOCOL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)