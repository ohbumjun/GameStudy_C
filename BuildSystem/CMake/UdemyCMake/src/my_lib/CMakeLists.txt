set(LIBRARY_SOURCES
    "my_lib.cc")

set(LIBRARY_HEADERS
    "my_lib.h")

# create target, but not executable == library
add_library(${LIBRARY_NAME} STATIC 
${LIBRARY_SOURCES} 
${LIBRARY_HEADERS})

# where to find header files
# cpp file can include header files from "./~~"
target_include_directories(${LIBRARY_NAME} PUBLIC 

"./"

# also include files from other directories
"${CMAKE_BINARY_DIR}/configured_files/include"
)