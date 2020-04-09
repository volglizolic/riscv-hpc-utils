cmake_minimum_required(VERSION 3.15)
project(Wraper C)

set(CMAKE_C_STANDARD 11)

add_executable(Wraper counter_wraper/src/main.c counter_wraper/src/CSR_codes.h counter_wraper/src/utils.c counter_wraper/src/utils.h counter_wraper/src/CSR_snapshot.c counter_wraper/src/CSR_snapshot.h)

add_executable(LoadModule module_wraper/src/load_module.c)