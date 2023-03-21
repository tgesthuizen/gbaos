cmake_minimum_required(VERSION 3.10)

project(gba-tinkering C ASM)

include(GNUInstallDirs)

add_compile_options(
  -Wall -Wextra
  -ffreestanding
  -mcpu=arm7tdmi
)

add_subdirectory(font8x8)

add_executable(kernel kstart.s main.c)
target_link_options(
  kernel
  PUBLIC
  -T ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld
)

add_custom_command(
  TARGET kernel
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:kernel> kernel.gba
  COMMENT "Generating kernel.gba"
)

install(
  TARGETS kernel
)

install(
  FILES kernel.gba
  DESTINATION ${CMAKE_INSTALL_DATADIR}
)
