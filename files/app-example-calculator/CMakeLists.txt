cmake_minimum_required(VERSION 3.4)

project(calculator
  LANGUAGES C CXX
  VERSION "1.0.0"
)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable C++14
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D_GLIBCXX_USE_NANOSLEEP -DLINUX")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler options: ${CMAKE_CXX_FLAGS}")

# Additionnal compiler flags debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVERBOSE_LEVEL=2")
message(STATUS "Debug compiler options: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")

# Import persistency library
message(STATUS "Importing persistency")
find_package(persistency REQUIRED)
message(STATUS "    Package persistency found: ${persistency_FOUND}")

# Import ARA-SomeIP-posix
message(STATUS "Importing ARA-SomeIP-posix")
find_package(ARA-SomeIP-posix REQUIRED)
message(STATUS "    Package ARA-SomeIP-posix found: ${ARA-SomeIP-posix_FOUND}")

# Import Threads
message(STATUS "Importing Threads")
find_package(Threads REQUIRED)
message(STATUS "    Package Threads found: ${Threads_FOUND}")

# Logging API
message(STATUS "Importing ara-logging")
find_package(ara-logging REQUIRED)
message(STATUS "    Package ara-logging found: ${ara-logging_FOUND}")
#find_package(amsr-vector-fs-log-api REQUIRED)
#message(STATUS "    Package amsr-vector-fs-log-api: ${amsr-vector-fs-log-api_FOUND}")

# Import ApplicationClient
message(STATUS "Importing EM ApplicationClient")

set(VectorApplicationClient amsr-vector-fs-em-executionmanagement_application-client)
find_package(${VectorApplicationClient} QUIET)

set(ARA_EXEC_LIBRARIES ${${VectorApplicationClient}_LIBRARIES})
message(STATUS "    Package ApplicationClient (Vector) found with libraries: ${ARA_EXEC_LIBRARIES}")
set(ARA_EXEC_INCLUDE_DIRS ${${VectorApplicationClient}_INCLUDE_DIRS})


# Import vac
message(STATUS "Importing vac")
find_package(vac REQUIRED)
message(STATUS "    Package vac found: ${vac_FOUND}")


# Additionnal compiler flags debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVERBOSE_LEVEL=2")
message(STATUS "Debug compiler options: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")

option(
    USE_PKGCONFIG
    "Use pkgconfig instead of CMake exported targets for dependencies"
    OFF
)
message(STATUS "option USE_PKGCONFIG=" ${USE_PKGCONFIG})

option(
    USE_ARA_PER
    "Enable ara/per example code"
    OFF
)

message(STATUS "option USE_ARA_PER=" ${USE_ARA_PER})

if (USE_ARA_PER)
    add_definitions(-DPERSISTENCY_ON)
endif()

add_subdirectory(src)

# Do not generate code until the aragen generator is able to do read the released scheme.
# Generate interfaces and vSOME/IP configuration
# set_source_files_properties(generate-interfaces PROPERTIES GENERATED true)
# add_custom_target(generate-interfaces
#     COMMAND sh "${PROJECT_SOURCE_DIR}/model/generate-interfaces.sh"
#     WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/model"
# )
# set_source_files_properties(generate-vsomeip-config PROPERTIES GENERATED true)
# add_custom_target(generate-vsomeip-config
#     COMMAND sh "${PROJECT_SOURCE_DIR}/model/generate-vsomeip-config.sh"
#     WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/model"
# )

# Deploy
install(FILES src-gen/etc/someip-posix/someipd-posix.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/someipd-posix/etc/)
install(FILES src-gen/etc/someip-posix/someipd-posix-gen.json PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION /opt/someipd-posix/etc/)
