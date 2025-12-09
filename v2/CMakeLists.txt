cmake_minimum_required(VERSION 3.15)
project(ChessEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#dependencies
find_package(SDL2 REQUIRED)

#fetch
file(GLOB SRC "src/*.cpp")
file(GLOB HEADERS "lib/*.h")

#loader
add_library(libs ${SRC} ${HEADERS})
target_include_directories(libs PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib
)
add_executable(${PROJECT_NAME} main.cpp)

#main run
target_link_libraries(${PROJECT_NAME} PRIVATE
    libs
    SDL2::SDL2
)

#testing setup googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#create executables from testfiles
enable_testing()

file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cc")
add_executable(
  ${PROJECT_NAME}_tests
  ${TEST_SOURCES}
)
target_link_libraries(
  ${PROJECT_NAME}_tests
  libs
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)