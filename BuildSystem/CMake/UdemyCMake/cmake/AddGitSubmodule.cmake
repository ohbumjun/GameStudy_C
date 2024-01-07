# dir : current directory

function(add_git_submodule dir)

    # if git not installed on computer. this function will error out
    # find git in computer. set variable
    find_package(Git REQUIRED)

    # if cmakelist.txt not exist in dir 
    #    - recursivley add submodule we listed in gitmodules file
    # ex) find cmake file in external/json & add that as sub_directory 
    #     & find very last cmakelist.txt
    if (NOT EXISTS ${dir}/CMakeLists.txt)
        # GIT_EXECUTABLE : set by find_package(GIT REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE} 
                        submodule update --init --recursive -- ${dir}
                        # PROJECT_SOURCE_DIR : absolute path to root directory
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()

    add_subdirectory(${dir})

endfunction(add_git_submodule)