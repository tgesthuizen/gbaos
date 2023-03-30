add_subdirectory(font8x8)
add_subdirectory(tinyprintf)

add_library(kernel-headers INTERFACE)
target_include_directories(kernel-headers INTERFACE include)

add_executable(
  kernel

  display.c
  kstart.s
  main.c
  string.c
)
target_link_options(
  kernel
  PUBLIC
  -T ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld
)
target_link_libraries(
  kernel
  PUBLIC
  font8x8
  kernel-headers
  tinyprintf
)
set_target_properties(kernel PROPERTIES
  OUTPUT_NAME kernel.elf
  LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld
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
