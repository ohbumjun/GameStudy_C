# 0. create source and cmakefile
# 1. create build folder
# 2. cd build
# 3. cmake .. (Generating the build files / configure the project)
# 4. cmake --build . (Compile the project, make final exe file)
# 5. ./Executable

cmake_minimum_required(VERSION 3.22)

# set the project name
project(CPPProjectCMake 
VERSION 
1.0.0 
LANGUAGES C CXX)

# every target in our project set to use C++17
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

# ex) some c variable not in C++ standard. but can be used in gcc
#     disable these kinds of examples 
set(CMAKE_CXX_EXTENSIONS        OFF)

# can pass variable to set 'COMPILE_EXECUTABLE' value in command line
# ex) cmake .. -DCOMPILE_EXECUTABLE=ON
option(COMPILE_EXECUTABLE "Whether to compile the executable" ON)

# define variable. all used in subdirectories cmakefiles
set(LIBRARY_NAME "Library")
set(EXECUTABLE_NAME "Executable")

# search for other CMakeLists.txt files in subdirectories
add_subdirectory(src) # library
add_subdirectory(configured)

if (COMPILE_EXECUTABLE)
    add_subdirectory(app)
else ()
    # message when configure. not when we build
    # ex) cmake .. -> '..' means configure
    message("Without exe. compiling")
endif()

# which generator should be used
# generator is the build system that cmake uses under the hood

# build type
# - Debug : no optimization
# - Release : high optimization