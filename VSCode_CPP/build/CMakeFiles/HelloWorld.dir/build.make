# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\workspace\git_TBSEQ\VSCode_CPP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\workspace\git_TBSEQ\VSCode_CPP\build

# Include any dependencies generated for this target.
include CMakeFiles/HelloWorld.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HelloWorld.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HelloWorld.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HelloWorld.dir/flags.make

CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj: CMakeFiles/HelloWorld.dir/flags.make
CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj: C:/workspace/git_TBSEQ/VSCode_CPP/HelloWorld.cpp
CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj: CMakeFiles/HelloWorld.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\workspace\git_TBSEQ\VSCode_CPP\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj -MF CMakeFiles\HelloWorld.dir\HelloWorld.cpp.obj.d -o CMakeFiles\HelloWorld.dir\HelloWorld.cpp.obj -c C:\workspace\git_TBSEQ\VSCode_CPP\HelloWorld.cpp

CMakeFiles/HelloWorld.dir/HelloWorld.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HelloWorld.dir/HelloWorld.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\workspace\git_TBSEQ\VSCode_CPP\HelloWorld.cpp > CMakeFiles\HelloWorld.dir\HelloWorld.cpp.i

CMakeFiles/HelloWorld.dir/HelloWorld.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HelloWorld.dir/HelloWorld.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\workspace\git_TBSEQ\VSCode_CPP\HelloWorld.cpp -o CMakeFiles\HelloWorld.dir\HelloWorld.cpp.s

# Object files for target HelloWorld
HelloWorld_OBJECTS = \
"CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj"

# External object files for target HelloWorld
HelloWorld_EXTERNAL_OBJECTS =

HelloWorld.exe: CMakeFiles/HelloWorld.dir/HelloWorld.cpp.obj
HelloWorld.exe: CMakeFiles/HelloWorld.dir/build.make
HelloWorld.exe: CMakeFiles/HelloWorld.dir/linkLibs.rsp
HelloWorld.exe: CMakeFiles/HelloWorld.dir/objects1.rsp
HelloWorld.exe: CMakeFiles/HelloWorld.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\workspace\git_TBSEQ\VSCode_CPP\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HelloWorld.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\HelloWorld.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HelloWorld.dir/build: HelloWorld.exe
.PHONY : CMakeFiles/HelloWorld.dir/build

CMakeFiles/HelloWorld.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\HelloWorld.dir\cmake_clean.cmake
.PHONY : CMakeFiles/HelloWorld.dir/clean

CMakeFiles/HelloWorld.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\workspace\git_TBSEQ\VSCode_CPP C:\workspace\git_TBSEQ\VSCode_CPP C:\workspace\git_TBSEQ\VSCode_CPP\build C:\workspace\git_TBSEQ\VSCode_CPP\build C:\workspace\git_TBSEQ\VSCode_CPP\build\CMakeFiles\HelloWorld.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/HelloWorld.dir/depend
