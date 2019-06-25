
get_filename_component(SOMEIP_POSIX_COMMON_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SOMEIP_POSIX_COMMON_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/communicationmiddlleware/lib/libSomeIP-posix-common/include")
set(SOMEIP_POSIX_COMMON_LIBRARIES "SomeIP-posix-common")

find_package(osabstraction REQUIRED)

include("${SOMEIP_POSIX_COMMON_CMAKE_DIR}/SomeIP-posix-commonTargets.cmake")
