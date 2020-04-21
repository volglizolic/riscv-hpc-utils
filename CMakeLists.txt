cmake_minimum_required(VERSION 3.15)
project(Wraper C)

set(CMAKE_C_STANDARD 11)

add_executable(counter_wraper counter_wraper/src/main.c utils/CSR_codes.h counter_wraper/src/CSR_snapshot.c counter_wraper/src/CSR_snapshot.h)

add_executable(module_wraper
        module_wraper/src/load_module.c
        utils/CSR_codes.h)

add_executable(csr_manipulation_test
        csr_manipulation_test/src/csr_manipulation_test.c
        utils/CSR_codes.h)