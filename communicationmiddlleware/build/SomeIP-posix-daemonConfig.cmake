
get_filename_component(SOMEIPD_POSIX_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOMEIPD_POSIX_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/communicationmiddlleware/src")
set(SOMEIPD_POSIX_LIBRARIES "SomeIP-posix-daemon")

include("${SOMEIPD_POSIX_CMAKE_DIR}/SomeIP-posix-daemonTargets.cmake")
