cmake_minimum_required(VERSION 3.20)

# Set toolchain before project() call
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/rl78-toolchain.cmake" CACHE FILEPATH "")

project(Blink C ASM)

# ---------- Sources ----------
set(C_SOURCES
    src/r_cg_cgc.c
    src/r_cg_cgc_user.c
    src/r_cg_inthandler.c
    src/r_cg_vector_table.c
    src/r_cg_port_user.c
    src/r_hardware_setup.c
    src/r_cg_port.c
    src/r_cg_serial.c
    src/r_cg_serial_user.c
    src/r_main.c
    src/uart_sw.c
)

set(ASM_SOURCES
    src/start.S
)

# ---------- Executable ----------
add_executable(${PROJECT_NAME}.elf ${C_SOURCES} ${ASM_SOURCES})

# ---------- Include directories ----------
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/generate
)

# ---------- Compile options ----------
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -Og
    -g
    -ffunction-sections
    -fdata-sections
    -fdiagnostics-parseable-fixits
    -fno-strict-aliasing
    -Wuninitialized
    -Wmissing-declarations
    -Wpointer-arith
    -Wshadow
    -Waggregate-return
)

# ---------- Linker options ----------
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T ${CMAKE_SOURCE_DIR}/generate/linker_script.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map
    -Wl,--start-group
    -Wl,--end-group
)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    LINK_DEPENDS ${CMAKE_SOURCE_DIR}/generate/linker_script.ld
)

# ---------- Post-build: generate SREC and print size ----------
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${PROJECT_NAME}.elf> -O srec ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.srec
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Generating ${PROJECT_NAME}.srec and printing size"
)

# ---------- Flash target ----------
add_custom_target(flash
    COMMAND rl78flash -viva -m1 /dev/ttyUSB0 ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.srec
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing ${PROJECT_NAME}.srec to /dev/ttyUSB0"
)
