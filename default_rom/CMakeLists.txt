cmake_minimum_required(VERSION 3.10)
project(default_rom C)

# Find RGBDS tools
find_program(RGBASM rgbasm)
find_program(RGBLINK rgblink)
find_program(RGBFIX rgbfix)

if(NOT RGBASM OR NOT RGBLINK OR NOT RGBFIX)
    message(FATAL_ERROR "RGBDS tools not found. Please install RGBDS (https://github.com/gbdev/rgbds)")
endif()

# Set up build commands for the ROMs
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/default_rom.gb
    COMMAND ${RGBASM} -o ${CMAKE_CURRENT_BINARY_DIR}/default_rom.o ${CMAKE_CURRENT_SOURCE_DIR}/default_rom.asm
    COMMAND ${RGBLINK} -o ${CMAKE_CURRENT_BINARY_DIR}/default_rom.gb ${CMAKE_CURRENT_BINARY_DIR}/default_rom.o
    COMMAND ${RGBFIX} -v -p 0 ${CMAKE_CURRENT_BINARY_DIR}/default_rom.gb
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/default_rom.asm 
            ${CMAKE_CURRENT_SOURCE_DIR}/hardware.inc 
            ${CMAKE_CURRENT_SOURCE_DIR}/default_rom.2bpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building default_rom.gb"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.gb
    COMMAND ${RGBASM} -o ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.o ${CMAKE_CURRENT_SOURCE_DIR}/default_rom_cgb.asm
    COMMAND ${RGBLINK} -o ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.gb ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.o
    COMMAND ${RGBFIX} -v -p 0 -c ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.gb
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/default_rom_cgb.asm 
            ${CMAKE_CURRENT_SOURCE_DIR}/hardware.inc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building default_rom_cgb.gb"
)

# Create target for the ROMs
add_custom_target(default_rom ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/default_rom.gb 
            ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.gb
    COMMENT "Building all ROMs"
)

# Install the ROMs to the build directory
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/default_rom.gb
    ${CMAKE_CURRENT_BINARY_DIR}/default_rom_cgb.gb
    DESTINATION ${CMAKE_BINARY_DIR}
)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add any source files for the ROM here
# file(GLOB_RECURSE ROM_SOURCES "*.c")

# If you have specific ROM building steps, add them here
# add_custom_target(default_rom
#     COMMAND <your rom building command>
# )

# For now, we'll just create an empty target
