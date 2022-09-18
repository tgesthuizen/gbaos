cmake_minimum_required(VERSION 3.10)

project(gba-tinkering CXX ASM)

add_compile_options(
  -Wall -Wextra
  -ffreestanding
  -fno-rtti -fno-exceptions
  -mcpu=arm7tdmi
)

add_library(asm-parts STATIC crt0.s)
add_executable(example main.cpp)
target_link_libraries(
  example
  PUBLIC
  asm-parts
)
add_custom_command(
  TARGET example
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:example> example.gba
  COMMENT "Generating example.gba"
)
target_link_options(example PUBLIC -T ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld)
