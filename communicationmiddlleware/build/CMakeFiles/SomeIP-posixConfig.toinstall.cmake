
get_filename_component(SOMEIP_POSIX_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOMEIP_POSIX_INCLUDE_DIRS "${SOMEIP_POSIX_CMAKE_DIR}/../../../include")
set(SOMEIP_POSIX_LIBRARIES "SomeIP-posix")

find_package(SomeIP-posix-common REQUIRED)
find_package(ara-logging REQUIRED)
find_package(osabstraction REQUIRED)

include("${SOMEIP_POSIX_CMAKE_DIR}/SomeIP-posixTargets.cmake")
