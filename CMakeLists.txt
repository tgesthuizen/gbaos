cmake_minimum_required(VERSION 3.10)

project(gba-tinkering C ASM)

include(GNUInstallDirs)

add_compile_options(
  -Wall -Wextra
  -mcpu=arm7tdmi
  -mthumb-interwork
  -mthumb
)

add_subdirectory(kern)
add_subdirectory(libc)
link_libraries(startfiles_static libc)

add_subdirectory(tools)
