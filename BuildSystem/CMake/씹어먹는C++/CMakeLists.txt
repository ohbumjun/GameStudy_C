# CMake 프로그램의 최소 버전
cmake_minimum_required(VERSION 3.25.2)

# 프로젝트 정보
project(
  ModooCode
  VERSION 0.1
  DESCRIPTION "예제 프로젝트"
  LANGUAGES CXX)

add_executable (program main.cc)