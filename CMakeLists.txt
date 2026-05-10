cmake_minimum_required(VERSION 3.10)

project(MiniRedis)

set(CMAKE_CXX_STANDARD 17)

add_executable(miniredis
    src/main.cpp
    src/server.cpp
)

target_include_directories(miniredis PRIVATE include)
target_link_libraries(miniredis ws2_32)

add_executable(client
    client/client.cpp
)

target_link_libraries(client ws2_32)