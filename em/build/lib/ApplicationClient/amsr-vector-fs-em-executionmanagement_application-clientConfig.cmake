set(PACKAGE_NAME amsr-vector-fs-em-executionmanagement_application-client)
get_filename_component(${PACKAGE_NAME}_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(CONF_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/em/lib/ApplicationClient/inc")

set(${PACKAGE_NAME}_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/em/lib/ApplicationClient/inc")
set(${PACKAGE_NAME}_LIBRARIES "amsr-vector-fs-em-executionmanagement_application-client")

find_package(osabstraction REQUIRED)
find_package(ara-logging REQUIRED)
find_package(vac REQUIRED)

include("${${PACKAGE_NAME}_CMAKE_DIR}/${PACKAGE_NAME}Targets.cmake")