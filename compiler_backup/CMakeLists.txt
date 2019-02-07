cmake_minimum_required(VERSION 3.13)
project(MSCVProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.67 REQUIRED)

add_executable(MSCVProject main.cc)

