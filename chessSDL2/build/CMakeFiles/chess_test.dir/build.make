# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sardiness/Desktop/projects/chessSDL2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sardiness/Desktop/projects/chessSDL2/build

# Include any dependencies generated for this target.
include CMakeFiles/chess_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/chess_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/chess_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chess_test.dir/flags.make

CMakeFiles/chess_test.dir/src/test/initialize.cpp.o: CMakeFiles/chess_test.dir/flags.make
CMakeFiles/chess_test.dir/src/test/initialize.cpp.o: ../src/test/initialize.cpp
CMakeFiles/chess_test.dir/src/test/initialize.cpp.o: CMakeFiles/chess_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sardiness/Desktop/projects/chessSDL2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chess_test.dir/src/test/initialize.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chess_test.dir/src/test/initialize.cpp.o -MF CMakeFiles/chess_test.dir/src/test/initialize.cpp.o.d -o CMakeFiles/chess_test.dir/src/test/initialize.cpp.o -c /home/sardiness/Desktop/projects/chessSDL2/src/test/initialize.cpp

CMakeFiles/chess_test.dir/src/test/initialize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_test.dir/src/test/initialize.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sardiness/Desktop/projects/chessSDL2/src/test/initialize.cpp > CMakeFiles/chess_test.dir/src/test/initialize.cpp.i

CMakeFiles/chess_test.dir/src/test/initialize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_test.dir/src/test/initialize.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sardiness/Desktop/projects/chessSDL2/src/test/initialize.cpp -o CMakeFiles/chess_test.dir/src/test/initialize.cpp.s

# Object files for target chess_test
chess_test_OBJECTS = \
"CMakeFiles/chess_test.dir/src/test/initialize.cpp.o"

# External object files for target chess_test
chess_test_EXTERNAL_OBJECTS =

chess_test: CMakeFiles/chess_test.dir/src/test/initialize.cpp.o
chess_test: CMakeFiles/chess_test.dir/build.make
chess_test: libchess_lib.a
chess_test: lib/libgtest.a
chess_test: lib/libgtest_main.a
chess_test: lib/libgtest.a
chess_test: CMakeFiles/chess_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sardiness/Desktop/projects/chessSDL2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable chess_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chess_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chess_test.dir/build: chess_test
.PHONY : CMakeFiles/chess_test.dir/build

CMakeFiles/chess_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chess_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chess_test.dir/clean

CMakeFiles/chess_test.dir/depend:
	cd /home/sardiness/Desktop/projects/chessSDL2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sardiness/Desktop/projects/chessSDL2 /home/sardiness/Desktop/projects/chessSDL2 /home/sardiness/Desktop/projects/chessSDL2/build /home/sardiness/Desktop/projects/chessSDL2/build /home/sardiness/Desktop/projects/chessSDL2/build/CMakeFiles/chess_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chess_test.dir/depend

