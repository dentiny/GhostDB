# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(ghostdb)

# Set C++ compiler version
set(CMAKE_CXX_STANDARD 17)

# Set compiler options
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -Wconversion -Wformat -g3")

# Create a sources variable with a link to all cpp files to compile
file(GLOB SOURCES "src/*.cc" "test/*.cc")

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)