cmake_minimum_required(VERSION 3.10)
project(WebpageFetcher)

set(CMAKE_CXX_STANDARD 11)

# Find the libcurl library
find_package(CURL REQUIRED)

# Include directories for libcurl
include_directories(${CURL_INCLUDE_DIRS})

# Source files for the project
set(SOURCES
    client.cpp
    server.cpp
)

# Executable for the client
add_executable(client client.cpp)
target_link_libraries(client ${CURL_LIBRARIES})

# Executable for the server
add_executable(server server.cpp)
target_link_libraries(server ${CURL_LIBRARIES})
