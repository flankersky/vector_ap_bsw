###############################################################################
#    Model Element   : CMakeLists
#    Component       : amsr-vector-fs-osabstraction
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : CMakeLists.txt
###############################################################################

set(OSABSTRACTION_LIBRARIES "${OSABSTRACTION_LIBRARIES}" ${GTEST_BOTH_LIBRARIES} ${add_gtest_LIBS} gmock ${CMAKE_THREAD_LIBS_INIT} PARENT_SCOPE)
