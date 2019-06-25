
get_filename_component(SOMEIP_POSIX_COMMON_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOMEIP_POSIX_COMMON_INCLUDE_DIRS "${SOMEIP_POSIX_COMMON_CMAKE_DIR}/../../../include")
set(SOMEIP_POSIX_COMMON_LIBRARIES "SomeIP-posix-common")

find_package(osabstraction REQUIRED)

include("${SOMEIP_POSIX_COMMON_CMAKE_DIR}/SomeIP-posix-commonTargets.cmake")
