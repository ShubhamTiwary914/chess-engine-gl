cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Set policy for OpenGL preference
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add source files for the main project
add_executable(${PROJECT_NAME}
    ../main.cpp
    ../include/glad/glad.c
    ../stb_impl.cpp
)

# Include directories for the main project
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link libraries for the main project
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
)

# Add Google Test
add_subdirectory(include/gtest/googletest)

# Add test executable
add_executable(test_chess
    test.cpp
)

# Include directories for tests
target_include_directories(test_chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/gtest/googletest/include
)

# Link Google Test libraries
target_link_libraries(test_chess PRIVATE
    gtest
    gtest_main
)

# Optionally, add a custom command to run tests after building
add_custom_command(TARGET test_chess
                   POST_BUILD
                   COMMAND test_chess)