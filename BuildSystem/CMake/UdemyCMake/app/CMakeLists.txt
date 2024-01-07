# create target , executable \\ make to exe
add_executable(${EXECUTABLE_NAME} main.cc)

# added library should be linked to exeutable
# ex) when file in executable use function in library, executable should be linked to library
# exectable depends on library. when build executable, library should be built first
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME})