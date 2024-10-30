cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ATSP VERSION 1.0 LANGUAGES CXX) 

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
file(GLOB SOURCES "src/*.cpp" "main.cpp" "src/Algorithms/*.cpp")
add_executable(ATSP ${SOURCES})

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp" "src/Algorithms/*.cpp")

# Link the sources to the executable
target_sources(ATSP PRIVATE ${SOURCES})