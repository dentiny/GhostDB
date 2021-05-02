# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(ghostdb)

# Set C++ compiler version
set(CMAKE_CXX_STANDARD 17)

# Set compiler options
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -Wconversion -Wformat -g3 -pthread")

# Generate the static library from the library sources
file(GLOB SOURCES "src/*.cc")

add_library(ghostdb STATIC 
  ${SOURCES}
)

target_include_directories(ghostdb
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

# Add an executable with the above sources
file(GLOB TEST "test/*.cc")

add_executable(testdb ${TEST})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(testdb
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
  PUBLIC
    /usr/local/include
)

target_link_libraries(testdb
  PRIVATE 
    ghostdb
  PUBLIC
    gtest
    gtest_main
)
