#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "persistency" for configuration ""
set_property(TARGET persistency APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(persistency PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpersistency.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS persistency )
list(APPEND _IMPORT_CHECK_FILES_FOR_persistency "${_IMPORT_PREFIX}/lib/libpersistency.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
