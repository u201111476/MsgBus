cmake_minimum_required(VERSION 3.10)
set(target MsgBusTest)
project(${target})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11 -Wwrite-strings -g")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -w")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(EXECUTABLE_OUTPUT_PATH ../bin)

include_directories(./src)
include_directories(./src/MsgBus)
include_directories(./src/ThreadPool)

file(GLOB SRC_LIST
"./src/*.cpp"
)

add_executable(${target}  ${SRC_LIST})


