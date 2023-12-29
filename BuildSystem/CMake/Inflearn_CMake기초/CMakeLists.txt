# CMake 프로그램의 최소 버전
cmake_minimum_required(VERSION 3.11)

# 프로젝트 정보
project(
  ModooCode
  VERSION 0.1
  DESCRIPTION "예제 프로젝트"
  LANGUAGES CXX)

add_executable (program main.cpp)cmake_minimum_required(VERSION 3.11)

set(PACKAGE_NAME READABLE_CODE)
set(PACKAGE_VERSION 1.0.1)

project(
    ${PACKAGE_NAME} 
    VERSION ${PACKAGE_VERSION} 
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

set(${PACKAGE_NAME}_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(${PACKAGE_NAME}_SOURCE_PATH "${${PACKAGE_NAME}_PATH}/src")

message(STATUS "PACKAGE_NAME : ${PACKAGE_NAME}")
message(STATUS "PACKAGE_VERSION : ${PACKAGE_VERSION}")
message(STATUS "${PACKAGE_NAME}_PATH : ${${PACKAGE_NAME}_PATH}")
message(STATUS "${PACKAGE_NAME}_SOURCE_PATH : ${${PACKAGE_NAME}_SOURCE_PATH}")

# # src/main.cpp 라는 파일을
# ${PACKAGE_NAME}_MAIN 이라는 이름의 executable 파일로 만들겠다
add_executable(${PACKAGE_NAME}_MAIN src/main.cpp)

target_compile_options(${PACKAGE_NAME}_MAIN PRIVATE
    -Wall -Wextra -Wpendantic -Werror
)