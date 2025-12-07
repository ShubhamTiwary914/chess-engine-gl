cmake_minimum_required(VERSION 3.15)
project(ChessEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

file(GLOB SRC "src/*.cpp")
file(GLOB HEADERS "lib/*.h")

add_library(libs ${SRC} ${HEADERS})
target_include_directories(libs PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    libs
    SDL2::SDL2
)
