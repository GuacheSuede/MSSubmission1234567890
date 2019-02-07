project( main )
cmake_minimum_required( VERSION 3.4 )

find_package(Boost REQUIRED COMPONENTS system)
include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} )



add_executable( ${PROJECT_NAME} 
    main.cc 
    util.cc
    listener.cc
    session.cc
    algo.cc
)
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} pthread)
