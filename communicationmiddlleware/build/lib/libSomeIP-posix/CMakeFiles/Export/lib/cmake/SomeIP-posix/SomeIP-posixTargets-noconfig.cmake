#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SomeIP-posix" for configuration ""
set_property(TARGET SomeIP-posix APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SomeIP-posix PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libSomeIP-posix.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SomeIP-posix )
list(APPEND _IMPORT_CHECK_FILES_FOR_SomeIP-posix "${_IMPORT_PREFIX}/lib/libSomeIP-posix.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
