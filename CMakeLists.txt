cmake_minimum_required(VERSION 2.6)


add_definitions(-Wall -g)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


include_directories(${CMAKE_SOURCE_DIR}/../prefix_root_local/include)

set(SRC Main.cpp Eveluation.cpp)

add_executable(tr ${SRC})



target_link_libraries(tr -lz -lm -ldl -lpthread)