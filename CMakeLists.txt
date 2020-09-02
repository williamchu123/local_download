cmake_minimum_required(VERSION 3.16)
project(local_download C)

set(CMAKE_C_STANDARD 99)

add_executable(local_download main.c conc.h conc.c)