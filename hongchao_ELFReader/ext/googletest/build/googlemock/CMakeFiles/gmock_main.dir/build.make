# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/hongchao/Desktop/ELFReader

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hongchao/Desktop/ELFReader

# Include any dependencies generated for this target.
include ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/depend.make

# Include the progress variables for this target.
include ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/progress.make

# Include the compile flags for this target's objects.
include ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/flags.make

ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o: ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/flags.make
ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o: ext/googletest/src/googlemock/src/gmock_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongchao/Desktop/ELFReader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o"
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmock_main.dir/src/gmock_main.cc.o -c /home/hongchao/Desktop/ELFReader/ext/googletest/src/googlemock/src/gmock_main.cc

ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmock_main.dir/src/gmock_main.cc.i"
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hongchao/Desktop/ELFReader/ext/googletest/src/googlemock/src/gmock_main.cc > CMakeFiles/gmock_main.dir/src/gmock_main.cc.i

ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmock_main.dir/src/gmock_main.cc.s"
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hongchao/Desktop/ELFReader/ext/googletest/src/googlemock/src/gmock_main.cc -o CMakeFiles/gmock_main.dir/src/gmock_main.cc.s

# Object files for target gmock_main
gmock_main_OBJECTS = \
"CMakeFiles/gmock_main.dir/src/gmock_main.cc.o"

# External object files for target gmock_main
gmock_main_EXTERNAL_OBJECTS =

ext/googletest/build/googlemock/libgmock_maind.a: ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
ext/googletest/build/googlemock/libgmock_maind.a: ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/build.make
ext/googletest/build/googlemock/libgmock_maind.a: ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hongchao/Desktop/ELFReader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgmock_maind.a"
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock_main.dir/cmake_clean_target.cmake
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmock_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/build: ext/googletest/build/googlemock/libgmock_maind.a

.PHONY : ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/build

ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/clean:
	cd /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock_main.dir/cmake_clean.cmake
.PHONY : ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/clean

ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/depend:
	cd /home/hongchao/Desktop/ELFReader && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader/ext/googletest/src/googlemock /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock /home/hongchao/Desktop/ELFReader/ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ext/googletest/build/googlemock/CMakeFiles/gmock_main.dir/depend

