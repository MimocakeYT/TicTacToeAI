cmake_minimum_required(VERSION 3.14)

project(TicTacToeAI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent module is available since CMake 3.11
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(SFML)

# Add source to this project's executable
add_executable(TicTacToeAI "src/main.cpp")

# Link SFML libraries
target_link_libraries(TicTacToeAI sfml-graphics sfml-window sfml-system)

# Include SFML headers
target_include_directories(TicTacToeAI PRIVATE ${SFML_INCLUDE_DIR})

# Set output directory
set_target_properties(TicTacToeAI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)