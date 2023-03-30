cmake_minimum_required(VERSION 3.10)

project(libc C ASM)

add_library(
  libc
  STATIC
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
