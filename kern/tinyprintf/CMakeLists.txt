cmake_minimum_required(VERSION 3.10)

project(tinyprintf C)

add_library(tinyprintf STATIC tinyprintf.c)
set_target_properties(
  tinyprintf
  PROPERTIES
  PUBLIC_HEADERS
  tinyprintf.h
)
target_compile_definitions(
  tinyprintf
  PUBLIC
  TINYPRINTF_DEFINE_TFP_PRINTF=0
  TINYPRINTF_DEFINE_TFP_SPRINTF=0
  TINYPRINTF_OVERRIDE_LIBC=0
)
target_include_directories(
  tinyprintf
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
