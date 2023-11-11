cmake_minimum_required(VERSION 3.10)

project(libc C ASM)

add_library(
  startfiles
  OBJECT
  crt0.s
)
target_link_libraries(startfiles PUBLIC libc)

add_library(startfiles_static STATIC $<TARGET_OBJECTS:startfiles>)

set(libc_headers
  assert.h
  errno.h
  stdlib.h
  string.h
)

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
target_compile_options(
  libc
  PRIVATE
  -nostdlib
)
set_target_properties(
  libc
  PROPERTIES
  OUTPUT_NAME c
)

install(TARGETS libc
  DESTINATION ${GBA_INCLUDE_DIR}
)
install(FILES $<TARGET_OBJECTS:startfiles>
  DESTINATION ${GBA_LIB_DIR}
  RENAME crt0.o
)
foreach(hdr ${libc_headers})
  install(FILES include/${hdr}
    DESTINATION ${GBA_INCLUDE_DIR}
  )
endforeach()
