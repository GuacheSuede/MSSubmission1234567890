cmake_minimum_required(VERSION 3.13)
project(MSCVProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")

find_package(Boost 1.67 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.5.0 REQUIRED)


add_subdirectory(cpr)

include_directories( ${Boost_INCLUDE_DIR} ${CPR_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIRS} )

add_executable(${PROJECT_NAME} main.cc)



target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CPR_LIBRARIES} pthread r3 nlohmann_json::nlohmann_json ssl)


