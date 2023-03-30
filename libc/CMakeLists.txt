cmake_minimum_required(VERSION 3.10)

project(libc C ASM)

add_library(
  libc
  STATIC
  abort.c
  assert.c
  errno.c
)
target_include_directories(
  libc
  PUBLIC
  include
)
target_link_libraries(
  libc
  PUBLIC
  kernel-headers
)
set_target_properties(
  libc
  PROPERTIES
  OUTPUT_NAME c
)
