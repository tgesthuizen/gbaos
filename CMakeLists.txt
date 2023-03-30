cmake_minimum_required(VERSION 3.10)

project(gba-tinkering C ASM)

include(GNUInstallDirs)

add_compile_options(
  -Wall -Wextra
  -ffreestanding
  -mcpu=arm7tdmi
  -mthumb-interwork
  -mthumb
)

add_subdirectory(kern)
