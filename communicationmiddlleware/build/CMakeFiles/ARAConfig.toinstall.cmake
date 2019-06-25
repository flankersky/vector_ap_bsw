
get_filename_component(ARA_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(ARA_INCLUDE_DIRS "${ARA_CMAKE_DIR}/../../../include")
set(ARA_LIBRARIES "ARA")

find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)

include("${ARA_CMAKE_DIR}/ARATargets.cmake")
