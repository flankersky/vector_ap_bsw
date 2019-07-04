cmake_minimum_required(VERSION 3.2)

find_package(Doxygen REQUIRED)

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/../config/Doxyfile)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(${doxyfile_in} ${doxyfile} @ONLY)

# make target to generate API documentation with doxygen
add_custom_target(documentation
  ALL
  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/doxygen)
install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/doxygen/html DESTINATION share/doc/${CMAKE_PROJECT_NAME})