cmake_minimum_required(VERSION 3.10)
project(cbc C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
if(DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Add default_rom as a subdirectory
add_subdirectory(default_rom)

# Make all target depend on default_rom
add_dependencies(${PROJECT_NAME} default_rom)

# Custom target for cleaning build artifacts
add_custom_target(cleanup
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove sandbox
    COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_NAME}
    COMMENT "Cleaning build artifacts"
)

add_custom_target(
    sandbox_clang
    COMMAND clang -Wall -o sandbox sandbox.c src/opcodes.c -I src/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building sandbox with clang"
) 