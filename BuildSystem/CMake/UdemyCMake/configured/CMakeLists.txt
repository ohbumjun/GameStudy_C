# CMAKE_BINARY_DIR : absoulute path build directory

configure_file(
    # input file for configure_file function
    "config.hpp.in" 
    # output : regular header file for c++
    #          regular c++ file is built after build cmake command
    "${CMAKE_BINARY_DIR}/configured_files/include/config.hpp" 
    ESCAPE_QUOTES
)