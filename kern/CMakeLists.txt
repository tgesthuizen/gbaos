add_subdirectory(font8x8)
add_subdirectory(tinyprintf)

add_library(kernel-headers INTERFACE)
target_include_directories(kernel-headers INTERFACE include)

configure_file(romfs.s.in ${CMAKE_CURRENT_BINARY_DIR}/romfs.s @ONLY)

set(kernel_public_headers
  kern/errno.h
  kern/syscall.h
)

add_executable(
  kernel

  display.c
  hwregs.c
  irq.c
  kstart.s
  main.c
  panic.c
  romfs.c
  string.c
  task.c

  ${CMAKE_CURRENT_BINARY_DIR}/romfs.s
)
set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/romfs.s
  PROPERTIES
  OBJECT_DEPENDS ${ROOTFS_FILE}
)
target_link_options(
  kernel
  PUBLIC
  -T ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld
)
set_target_properties(kernel PROPERTIES
  OUTPUT_NAME kernel.elf
  LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gba_cart.ld
)
target_compile_options(
  kernel
  PUBLIC
  -ffreestanding -nostdlib
)
target_link_libraries(
  kernel
  PUBLIC
  font8x8
  kernel-headers
  tinyprintf
)

add_custom_command(
  TARGET kernel
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:kernel> kernel.gba
  COMMAND ${GBAFIX_BINARY} kernel.gba -t "gbaos" -c2OSE -mTG
  COMMENT "Generating kernel.gba"
)

foreach(kheader ${kernel_public_headers})
  get_filename_component(include_subdir ${kheader} DIRECTORY)
  install(FILES include/${kheader}
    DESTINATION ${GBA_INCLUDE_DIR}/${include_subdir}
  )
endforeach()

install(
  TARGETS kernel
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/kernel.gba
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
