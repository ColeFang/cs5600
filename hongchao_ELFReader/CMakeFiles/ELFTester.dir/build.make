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
include CMakeFiles/ELFTester.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ELFTester.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ELFTester.dir/flags.make

CMakeFiles/ELFTester.dir/src/main.cpp.o: CMakeFiles/ELFTester.dir/flags.make
CMakeFiles/ELFTester.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongchao/Desktop/ELFReader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ELFTester.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ELFTester.dir/src/main.cpp.o -c /home/hongchao/Desktop/ELFReader/src/main.cpp

CMakeFiles/ELFTester.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ELFTester.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hongchao/Desktop/ELFReader/src/main.cpp > CMakeFiles/ELFTester.dir/src/main.cpp.i

CMakeFiles/ELFTester.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ELFTester.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hongchao/Desktop/ELFReader/src/main.cpp -o CMakeFiles/ELFTester.dir/src/main.cpp.s

CMakeFiles/ELFTester.dir/answer/answer.c.o: CMakeFiles/ELFTester.dir/flags.make
CMakeFiles/ELFTester.dir/answer/answer.c.o: answer/answer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongchao/Desktop/ELFReader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ELFTester.dir/answer/answer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ELFTester.dir/answer/answer.c.o   -c /home/hongchao/Desktop/ELFReader/answer/answer.c

CMakeFiles/ELFTester.dir/answer/answer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ELFTester.dir/answer/answer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongchao/Desktop/ELFReader/answer/answer.c > CMakeFiles/ELFTester.dir/answer/answer.c.i

CMakeFiles/ELFTester.dir/answer/answer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ELFTester.dir/answer/answer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongchao/Desktop/ELFReader/answer/answer.c -o CMakeFiles/ELFTester.dir/answer/answer.c.s

# Object files for target ELFTester
ELFTester_OBJECTS = \
"CMakeFiles/ELFTester.dir/src/main.cpp.o" \
"CMakeFiles/ELFTester.dir/answer/answer.c.o"

# External object files for target ELFTester
ELFTester_EXTERNAL_OBJECTS =

ELFTester: CMakeFiles/ELFTester.dir/src/main.cpp.o
ELFTester: CMakeFiles/ELFTester.dir/answer/answer.c.o
ELFTester: CMakeFiles/ELFTester.dir/build.make
ELFTester: ext/googletest/build/googlemock/gtest/libgtestd.a
ELFTester: CMakeFiles/ELFTester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hongchao/Desktop/ELFReader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ELFTester"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ELFTester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ELFTester.dir/build: ELFTester

.PHONY : CMakeFiles/ELFTester.dir/build

CMakeFiles/ELFTester.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ELFTester.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ELFTester.dir/clean

CMakeFiles/ELFTester.dir/depend:
	cd /home/hongchao/Desktop/ELFReader && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader /home/hongchao/Desktop/ELFReader/CMakeFiles/ELFTester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ELFTester.dir/depend

