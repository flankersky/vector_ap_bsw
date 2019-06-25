
get_filename_component(SOMEIPD_POSIX_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOMEIPD_POSIX_INCLUDE_DIRS "${SOMEIPD_POSIX_CMAKE_DIR}/../")
set(SOMEIPD_POSIX_LIBRARIES "SomeIP-posix-daemon")

include("${SOMEIPD_POSIX_CMAKE_DIR}/SomeIP-posix-daemonTargets.cmake")
