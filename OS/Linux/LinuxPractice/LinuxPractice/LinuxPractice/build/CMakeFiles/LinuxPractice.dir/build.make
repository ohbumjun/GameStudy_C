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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build

# Include any dependencies generated for this target.
include CMakeFiles/LinuxPractice.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/LinuxPractice.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/LinuxPractice.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LinuxPractice.dir/flags.make

CMakeFiles/LinuxPractice.dir/LinuxPractice.obj: CMakeFiles/LinuxPractice.dir/flags.make
CMakeFiles/LinuxPractice.dir/LinuxPractice.obj: D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/LinuxPractice.cpp
CMakeFiles/LinuxPractice.dir/LinuxPractice.obj: CMakeFiles/LinuxPractice.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LinuxPractice.dir/LinuxPractice.obj"
	C:/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LinuxPractice.dir/LinuxPractice.obj -MF CMakeFiles/LinuxPractice.dir/LinuxPractice.obj.d -o CMakeFiles/LinuxPractice.dir/LinuxPractice.obj -c D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/LinuxPractice.cpp

CMakeFiles/LinuxPractice.dir/LinuxPractice.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinuxPractice.dir/LinuxPractice.i"
	C:/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/LinuxPractice.cpp > CMakeFiles/LinuxPractice.dir/LinuxPractice.i

CMakeFiles/LinuxPractice.dir/LinuxPractice.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinuxPractice.dir/LinuxPractice.s"
	C:/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/LinuxPractice.cpp -o CMakeFiles/LinuxPractice.dir/LinuxPractice.s

# Object files for target LinuxPractice
LinuxPractice_OBJECTS = \
"CMakeFiles/LinuxPractice.dir/LinuxPractice.obj"

# External object files for target LinuxPractice
LinuxPractice_EXTERNAL_OBJECTS =

LinuxPractice.exe: CMakeFiles/LinuxPractice.dir/LinuxPractice.obj
LinuxPractice.exe: CMakeFiles/LinuxPractice.dir/build.make
LinuxPractice.exe: CMakeFiles/LinuxPractice.dir/linkLibs.rsp
LinuxPractice.exe: CMakeFiles/LinuxPractice.dir/objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LinuxPractice.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/LinuxPractice.dir/objects.a
	C:/mingw64/bin/ar.exe qc CMakeFiles/LinuxPractice.dir/objects.a @CMakeFiles/LinuxPractice.dir/objects1
	C:/mingw64/bin/c++.exe -g -Wl,--whole-archive CMakeFiles/LinuxPractice.dir/objects.a -Wl,--no-whole-archive -o LinuxPractice.exe -Wl,--out-implib,libLinuxPractice.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/LinuxPractice.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/LinuxPractice.dir/build: LinuxPractice.exe
.PHONY : CMakeFiles/LinuxPractice.dir/build

CMakeFiles/LinuxPractice.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LinuxPractice.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LinuxPractice.dir/clean

CMakeFiles/LinuxPractice.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build D:/Coding/CodingStudyStuff/gameStudy/Codes/OS/Linux/LinuxPractice/LinuxPractice/LinuxPractice/build/CMakeFiles/LinuxPractice.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LinuxPractice.dir/depend

