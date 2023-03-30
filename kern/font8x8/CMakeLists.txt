cmake_minimum_required(VERSION 3.10)

project(font8x8 C)

include(GNUInstallDirs)

add_library(font8x8 INTERFACE)
set_target_properties(
  font8x8
  PROPERTIES
  PUBLIC_HEADERS
  font8x8_basic.h
  font8x8_block.h
  font8x8_box.h
  font8x8_control.h
  font8x8_ext_latin.h
  font8x8_greek.h
  font8x8.h
  font8x8_hiragana.h
  font8x8_latin.h
  font8x8_misc.h
  font8x8_sga.h
)
target_include_directories(font8x8 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(
  TARGETS font8x8
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/font8x8
)
