cmake_minimum_required(VERSION 3.10)

project(
  genromfs
  VERSION 0.5.7
  LANGUAGES C
)

include(GNUInstallDirs)

add_executable(genromfs genromfs.c)
target_compile_definitions(
  genromfs
  PRIVATE
  VERSION="${PROJECT_VERSION}"
)

install(
  TARGETS genromfs
  DESTINATION ${CMAKE_INSTALL_BINDIR}  
)
