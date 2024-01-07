
# create target, but not executable == library
add_library(${LIBRARY_NAME} STATIC my_lib.cc)

# where to find header files
# cpp file can include header files from "./~~"
target_include_directories(${LIBRARY_NAME} PUBLIC 

"./"

# also include files from other directories
"${CMAKE_BINARY_DIR}/configured_files/include"
)