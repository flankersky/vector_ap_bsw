get_filename_component(OSABSTRACTION_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
# CONF_INCLUDE_DIRS must match the ${PROJECT_SOURCE_DIR}/lib/include of template
set(OSABSTRACTION_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/libosa/lib/libosabstraction-interface/include;/home/wsj/workspace/vector_ap/DATA/BSW/libosa/lib/libosabstraction-linux/include;/home/wsj/workspace/vector_ap/DATA/BSW/libosa/lib/libosabstraction-posix/include")
# LIBRARY_NAME must match the name from varTargets.cmake
set(OSABSTRACTION_LIBRARIES "osabstraction")
# Variable to determine used OS abstraction in other modules
set(OSABSTRACTION_NAME "linux")

include("${OSABSTRACTION_CMAKE_DIR}/osabstractionTargets.cmake")
