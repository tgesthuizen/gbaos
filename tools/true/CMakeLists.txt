cmake_minimum_required(VERSION 3.10)

project(true C)

add_executable(true true.c)
target_compile_definitions(true PUBLIC TRUE_EXITCODE=0)
add_executable(false true.c)
target_compile_definitions(false PUBLIC TRUE_EXITCODE=1)

add_romfs_binary(true)
add_romfs_binary(false)
