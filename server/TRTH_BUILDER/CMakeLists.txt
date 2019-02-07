cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")

find_package(Boost 1.67 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.5.0 REQUIRED)
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

add_subdirectory(cpr)

include_directories( ${Boost_INCLUDE_DIR} ${CPR_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR} ${LIBBSONCXX_INCLUDE_DIR})
link_directories( ${Boost_LIBRARY_DIRS} )

# Due to bug of generated directories not being included
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")

add_executable(${PROJECT_NAME} main.cc)



target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CPR_LIBRARIES} pthread r3 nlohmann_json::nlohmann_json ssl dill ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})


