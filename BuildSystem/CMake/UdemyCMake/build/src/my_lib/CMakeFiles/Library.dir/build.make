# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build

# Include any dependencies generated for this target.
include src/my_lib/CMakeFiles/Library.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/my_lib/CMakeFiles/Library.dir/compiler_depend.make

# Include the progress variables for this target.
include src/my_lib/CMakeFiles/Library.dir/progress.make

# Include the compile flags for this target's objects.
include src/my_lib/CMakeFiles/Library.dir/flags.make

src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj: src/my_lib/CMakeFiles/Library.dir/flags.make
src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj: src/my_lib/CMakeFiles/Library.dir/includes_CXX.rsp
src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj: D:/Coding/CodingStudyStuff/gameStudy/Codes/BuildSystem/CMake/UdemyCMake/src/my_lib/my_lib.cc
src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj: src/my_lib/CMakeFiles/Library.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj"
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && C:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj -MF CMakeFiles\Library.dir\my_lib.cc.obj.d -o CMakeFiles\Library.dir\my_lib.cc.obj -c D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\src\my_lib\my_lib.cc

src/my_lib/CMakeFiles/Library.dir/my_lib.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Library.dir/my_lib.cc.i"
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && C:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\src\my_lib\my_lib.cc > CMakeFiles\Library.dir\my_lib.cc.i

src/my_lib/CMakeFiles/Library.dir/my_lib.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Library.dir/my_lib.cc.s"
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && C:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\src\my_lib\my_lib.cc -o CMakeFiles\Library.dir\my_lib.cc.s

# Object files for target Library
Library_OBJECTS = \
"CMakeFiles/Library.dir/my_lib.cc.obj"

# External object files for target Library
Library_EXTERNAL_OBJECTS =

src/my_lib/libLibrary.a: src/my_lib/CMakeFiles/Library.dir/my_lib.cc.obj
src/my_lib/libLibrary.a: src/my_lib/CMakeFiles/Library.dir/build.make
src/my_lib/libLibrary.a: src/my_lib/CMakeFiles/Library.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLibrary.a"
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && $(CMAKE_COMMAND) -P CMakeFiles\Library.dir\cmake_clean_target.cmake
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Library.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/my_lib/CMakeFiles/Library.dir/build: src/my_lib/libLibrary.a
.PHONY : src/my_lib/CMakeFiles/Library.dir/build

src/my_lib/CMakeFiles/Library.dir/clean:
	cd /d D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib && $(CMAKE_COMMAND) -P CMakeFiles\Library.dir\cmake_clean.cmake
.PHONY : src/my_lib/CMakeFiles/Library.dir/clean

src/my_lib/CMakeFiles/Library.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\src\my_lib D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib D:\Coding\CodingStudyStuff\gameStudy\Codes\BuildSystem\CMake\UdemyCMake\build\src\my_lib\CMakeFiles\Library.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/my_lib/CMakeFiles/Library.dir/depend

