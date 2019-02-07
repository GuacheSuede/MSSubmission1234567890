cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.67 REQUIRED COMPONENTS system regex)
find_package(nlohmann_json 3.5.0 REQUIRED)


include_directories( ${Boost_INCLUDE_DIR} ${CPR_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR} ${LIBBSONCXX_INCLUDE_DIR})
link_directories( ${Boost_LIBRARY_DIRS} )


add_executable(${PROJECT_NAME} main.cc)



target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} nlohmann_json::nlohmann_json ssl pthread)


