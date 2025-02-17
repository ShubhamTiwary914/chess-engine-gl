cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(chess)

#subdirectories
add_subdirectory(include/glm)
add_subdirectory(include/googletest)

find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

#libraries
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

#executables(.cpp)
file(GLOB ENGINE_SOURCES "src/engine/*.cpp")
file(GLOB APP_SOURCES "src/app/*.cpp")
file(GLOB UTILS_SOURCES "src/utils/*.cpp")
file(GLOB TEST_SOURCES "src/test/*.cpp")

# Create main library without test sources
add_library(${PROJECT_NAME}_lib
    ${ENGINE_SOURCES}
    ${APP_SOURCES}
    ${UTILS_SOURCES}
)

# Main executable
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${PROJECT_NAME}_lib 
    glm
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

# Test executable
enable_testing()
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    ${PROJECT_NAME}_lib
    GTest::gtest
    GTest::gtest_main
)