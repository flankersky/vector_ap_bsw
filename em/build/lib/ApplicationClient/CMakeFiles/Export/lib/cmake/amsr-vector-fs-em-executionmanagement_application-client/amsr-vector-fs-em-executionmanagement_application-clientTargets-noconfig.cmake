#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "amsr-vector-fs-em-executionmanagement_application-client" for configuration ""
set_property(TARGET amsr-vector-fs-em-executionmanagement_application-client APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(amsr-vector-fs-em-executionmanagement_application-client PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libamsr-vector-fs-em-executionmanagement_application-client.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS amsr-vector-fs-em-executionmanagement_application-client )
list(APPEND _IMPORT_CHECK_FILES_FOR_amsr-vector-fs-em-executionmanagement_application-client "${_IMPORT_PREFIX}/lib/libamsr-vector-fs-em-executionmanagement_application-client.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
