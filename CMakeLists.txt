cmake_minimum_required(VERSION 3.10)

project(gba-tinkering C ASM)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(DiscoverHostTools)

add_compile_options(
  -Wall -Wextra
  -mcpu=arm7tdmi
  -mthumb-interwork
  -mthumb
)

set(GBA_DATA_DIR ${CMAKE_INSTALL_DATADIR}/lib/gba)
set(GBA_SYSROOT_DIR ${GBA_DATA_DIR}/sysroot/)
set(GBA_BIN_DIR ${GBA_SYSROOT_DIR}/bin)
set(GBA_INCLUDE_DIR ${GBA_SYSROOT_DIR}/include)
set(GBA_LIB_DIR ${GBA_SYSROOT_DIR}/lib)

set(SYSTEM_PAGE_SIZE 512)
set(ROOTFS_FILE ${CMAKE_CURRENT_BINARY_DIR}/root.romfs)

add_subdirectory(kern)

add_compile_options(
  -fpic
  -fomit-frame-pointer
  -msingle-pic-base
  -mlong-calls
)
add_link_options(
  -pie
  -Wl,--no-dynamic-linker
  -Wl,-z,common-page-size=${SYSTEM_PAGE_SIZE}
  -Wl,-z,max-page-size=${SYSTEM_PAGE_SIZE}
  -Wl,-z,noexecstack
  -Wl,-z,text
)

add_subdirectory(libc)
link_libraries(startfiles_static libc)

set(ROM_ROOT ${CMAKE_CURRENT_BINARY_DIR}/romfs)
file(MAKE_DIRECTORY ${ROM_ROOT})
file(MAKE_DIRECTORY ${ROM_ROOT}/bin)

add_custom_target(
  rootfs_files
)

function(add_romfs_binary bin_target)
  add_custom_command(
    TARGET ${bin_target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${bin_target}> ${ROM_ROOT}/bin/${bin_target}
    COMMAND ${CMAKE_OBJCOPY} -S ${ROM_ROOT}/bin/${bin_target}
  )
  add_dependencies(rootfs_files ${bin_target})
  install(TARGETS ${bin_target}
    DESTINATION ${GBA_BIN_DIR}
  )
endfunction()

add_subdirectory(tools)

add_custom_command(
  OUTPUT ${ROOTFS_FILE}
  DEPENDS rootfs_files
  COMMAND ${GENROMFS_BINARY} -a ${SYSTEM_PAGE_SIZE} -V rootfs -f ${ROOTFS_FILE} -d ${ROM_ROOT}
)

add_custom_target(
  rootfs
  DEPENDS ${ROOTFS_FILE}
)

install(FILES ${ROOTFS_FILE}
  DESTINATION ${GBA_DATA_DIR}
)
