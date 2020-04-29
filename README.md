# HPC_UTILS
HPC_UTILS is a utility for manipulating the hardware performance counters
in RISC-V architecture compliant platforms.

hpc_utils is based on `libhpc_utils`.

## Quick Start
To build hpc_utils and libhpc_utils, simply run `make`.
Currently libhpc_utils supports only SiFive's FU540-C000 U54. 
For running on Freedom-U-SDK link libhpc_utils (`libhpc_utils.a`) 
statically (with gcc argument `-static`) otherwise segfault occurs.

With hpc_utils you can program the counters and measure 
the events' occurrences during execution of 
a program. From more information run `./hpc_utils -h`.

For information on how to use libhpc_utils, 
check the `csr_manipulation_test.c` sample code in `./hpc_utils_lib/tests/src` 
and the `hpc_utils.h` header file in `./hpc_utils_lib/include/`.