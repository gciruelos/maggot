cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS_DEBUG "-Os -std=c11 -Wall -Wpedantic ${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS_RELEASE "-w -Os -std=c11 ${CMAKE_C_FLAGS}")
add_executable(maggot maggot.c)
