# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application

# Include any dependencies generated for this target.
include CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/flags.make

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/flags.make
CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o: run.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o -c /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/run.cc

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/run.cc > CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.i

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/run.cc -o CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.s

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.requires:

.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.requires

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.provides: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.requires
	$(MAKE) -f CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/build.make CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.provides.build
.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.provides

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.provides.build: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o


# Object files for target amsr-vector-fs-em-executionmanager-shutdown-application
amsr__vector__fs__em__executionmanager__shutdown__application_OBJECTS = \
"CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o"

# External object files for target amsr-vector-fs-em-executionmanager-shutdown-application
amsr__vector__fs__em__executionmanager__shutdown__application_EXTERNAL_OBJECTS =

amsr-vector-fs-em-executionmanager-shutdown-application: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o
amsr-vector-fs-em-executionmanager-shutdown-application: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/build.make
amsr-vector-fs-em-executionmanager-shutdown-application: /usr/local/lib/libosabstraction.a
amsr-vector-fs-em-executionmanager-shutdown-application: /home/user/AdaptiveEvalbundle/BSW/libvac/lib/libvac.a
amsr-vector-fs-em-executionmanager-shutdown-application: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable amsr-vector-fs-em-executionmanager-shutdown-application"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/build: amsr-vector-fs-em-executionmanager-shutdown-application

.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/build

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/requires: CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/run.o.requires

.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/requires

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/cmake_clean.cmake
.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/clean

CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/depend:
	cd /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application /home/user/AdaptiveEvalbundle/BSW/em/addon/amsr-vector-fs-em-executionmanager-shutdown-application/CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/amsr-vector-fs-em-executionmanager-shutdown-application.dir/depend
