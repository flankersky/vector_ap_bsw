#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SomeIP-posix-common" for configuration ""
set_property(TARGET SomeIP-posix-common APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SomeIP-posix-common PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libSomeIP-posix-common.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SomeIP-posix-common )
list(APPEND _IMPORT_CHECK_FILES_FOR_SomeIP-posix-common "${_IMPORT_PREFIX}/lib/libSomeIP-posix-common.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
