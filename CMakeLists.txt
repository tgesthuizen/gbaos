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

add_compile_options(
  -fpic -msingle-pic-base -mlong-calls
)
add_link_options(
  -pie
  -Wl,--no-dynamic-linker
  -Wl,-z,common-page-size=512
  -Wl,-z,max-page-size=512
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
endfunction()

add_subdirectory(tools)

find_program(
  GENROMFS_BINARY
  genromfs
  DOC "genromfs binary to build the romfs for the system"
  REQUIRED
)

add_custom_command(
  OUTPUT root.romfs
  DEPENDS rootfs_files
  COMMAND ${GENROMFS_BINARY} -V rootfs -f root.romfs -d ${ROM_ROOT}
)

add_custom_target(
  rootfs ALL
  DEPENDS root.romfs
)
