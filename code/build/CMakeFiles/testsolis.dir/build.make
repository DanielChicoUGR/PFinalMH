# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /home/daniel/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/daniel/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/231.8109.222/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniel/Proyectos/p4mh/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniel/Proyectos/p4mh/code/build

# Include any dependencies generated for this target.
include CMakeFiles/testsolis.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/testsolis.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/testsolis.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testsolis.dir/flags.make

CMakeFiles/testsolis.dir/src/testsolis.cc.o: CMakeFiles/testsolis.dir/flags.make
CMakeFiles/testsolis.dir/src/testsolis.cc.o: /home/daniel/Proyectos/p4mh/code/src/testsolis.cc
CMakeFiles/testsolis.dir/src/testsolis.cc.o: CMakeFiles/testsolis.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/Proyectos/p4mh/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testsolis.dir/src/testsolis.cc.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/testsolis.dir/src/testsolis.cc.o -MF CMakeFiles/testsolis.dir/src/testsolis.cc.o.d -o CMakeFiles/testsolis.dir/src/testsolis.cc.o -c /home/daniel/Proyectos/p4mh/code/src/testsolis.cc

CMakeFiles/testsolis.dir/src/testsolis.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testsolis.dir/src/testsolis.cc.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/Proyectos/p4mh/code/src/testsolis.cc > CMakeFiles/testsolis.dir/src/testsolis.cc.i

CMakeFiles/testsolis.dir/src/testsolis.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testsolis.dir/src/testsolis.cc.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/Proyectos/p4mh/code/src/testsolis.cc -o CMakeFiles/testsolis.dir/src/testsolis.cc.s

# Object files for target testsolis
testsolis_OBJECTS = \
"CMakeFiles/testsolis.dir/src/testsolis.cc.o"

# External object files for target testsolis
testsolis_EXTERNAL_OBJECTS =

/home/daniel/Proyectos/p4mh/code/bin/testsolis: CMakeFiles/testsolis.dir/src/testsolis.cc.o
/home/daniel/Proyectos/p4mh/code/bin/testsolis: CMakeFiles/testsolis.dir/build.make
/home/daniel/Proyectos/p4mh/code/bin/testsolis: libcec17_test_func.so
/home/daniel/Proyectos/p4mh/code/bin/testsolis: CMakeFiles/testsolis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/Proyectos/p4mh/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/daniel/Proyectos/p4mh/code/bin/testsolis"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testsolis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testsolis.dir/build: /home/daniel/Proyectos/p4mh/code/bin/testsolis
.PHONY : CMakeFiles/testsolis.dir/build

CMakeFiles/testsolis.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testsolis.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testsolis.dir/clean

CMakeFiles/testsolis.dir/depend:
	cd /home/daniel/Proyectos/p4mh/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/Proyectos/p4mh/code /home/daniel/Proyectos/p4mh/code /home/daniel/Proyectos/p4mh/code/build /home/daniel/Proyectos/p4mh/code/build /home/daniel/Proyectos/p4mh/code/build/CMakeFiles/testsolis.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testsolis.dir/depend

