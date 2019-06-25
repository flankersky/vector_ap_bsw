
get_filename_component(ARA_SOMEIP_POSIX_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(ARA_SOMEIP_POSIX_INCLUDE_DIRS "/home/wsj/workspace/vector_ap/DATA/BSW/communicationmiddlleware/lib/libARA-SomeIP-posix/include")
set(ARA_SOMEIP_POSIX_LIBRARIES "ARA-SomeIP-posix")

find_package(ara-logging REQUIRED)
find_package(vac REQUIRED)
find_package(ARA REQUIRED)
find_package(SomeIP-posix REQUIRED)

include("${ARA_SOMEIP_POSIX_CMAKE_DIR}/ARA-SomeIP-posixTargets.cmake")
