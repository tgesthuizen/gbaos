cmake_minimum_required(VERSION 3.10)

project(gba-tools C)

include(GNUInstallDirs)

add_executable(gbafix src/gbafix.c)
add_executable(gbfs src/gbfs.c)
add_executable(insgbfs src/insgbfs.c)
add_executable(lsgbfs src/lsgbfs.c)
add_executable(ungbfs src/ungbfs.c)

install(
  TARGETS gbafix gbfs insgbfs lsgbfs ungbfs
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
