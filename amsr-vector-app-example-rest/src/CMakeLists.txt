option(ENABLE_CLIENT "Build the client example application." ON)
option(ENABLE_SERVER "Build the server example application." ON)
option(ENABLE_CLIENTEVENT "Build the clientevent example application." ON)
option(ENABLE_SERVEREVENT "Build the serverevent example application." ON)

message(STATUS "option -DENABLE_CLIENT=" ${ENABLE_CLIENT})
message(STATUS "option -DENABLE_SERVER=" ${ENABLE_SERVER})
message(STATUS "option -DENABLE_CLIENTEVENT=" ${ENABLE_CLIENTEVENT})
message(STATUS "option -DENABLE_SERVEREVENT=" ${ENABLE_SERVEREVENT})
message(STATUS "-------------------------------------------------------------")

# find external packages
find_package(ara-logging REQUIRED)
find_package(Threads REQUIRED)
find_package(AraRest REQUIRED)

include_directories(
  "${CMAKE_CURRENT_LIST_DIR}/include"
)

if(ENABLE_CLIENT)
  add_subdirectory(client)
endif()

if(ENABLE_SERVER)
  add_subdirectory(server)
endif()
if(ENABLE_CLIENTEVENT)
  add_subdirectory(clientevent)
endif()

if(ENABLE_SERVEREVENT)
  add_subdirectory(serverevent)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  add_subdirectory(qnxdummylib)
endif()
message(STATUS "-------------------------------------------------------------")
