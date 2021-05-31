cmake_minimum_required(VERSION 2.80)

set(projectName TCPAndUDP)
project(${projectName})


message(${projectName})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")

message(${CMAKE_CXX_FLAGS})
aux_source_directory(. SRC_LIST)
message(${SRC_LIST})
add_executable(server server.cpp)
add_executable(client clientTCP.cpp)

