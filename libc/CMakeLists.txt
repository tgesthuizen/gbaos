cmake_minimum_required(VERSION 3.10)

project(libc C ASM)

add_library(
  startfiles
  OBJECT
  crt0.s
)
target_link_libraries(startfiles PUBLIC libc)

add_library(startfiles_static STATIC $<TARGET_OBJECTS:startfiles>)

add_library(
  libc
  STATIC
  abort.c
  assert.c
  errno.c
  memset.c
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
