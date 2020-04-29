cmake_minimum_required(VERSION 3.15)
project(Wraper C)

set(CMAKE_C_STANDARD 11)

add_executable(hpc_utils hpc_utils/src/hpc_utils.c hpc_utils_lib/include/hpc_utils.h hpc_utils/src/help.c hpc_utils/src/help.h)

add_executable(hpc_utils_lib_tests
        hpc_utils_lib/tests/src/csr_manipulation_test.c)

add_executable(additions_and_divisions
        hpc_utils/tests/src/add_and_div.c)

add_executable(hpc_utils_lib
        utils/CSR_codes.h
        hpc_utils_lib/include/hpc_utils.h
        hpc_utils_lib/src/hpc_utils_lib.c
        hpc_utils_lib/src/fu540.h
        hpc_utils_lib/src/event_definition.h
        utils/asm.h)