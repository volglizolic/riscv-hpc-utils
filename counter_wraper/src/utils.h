//
// Created by volglizolic on 1/29/20.
//

#ifndef WRAPER_UTILS_H
#define WRAPER_UTILS_H

#include "CSR_codes.h"

#define read_csr(csr) ({ unsigned long __tmp; \
asm volatile ("csrr %0, " csr : "=r"(__tmp)); \
__tmp; })

//#define read_csr(csr) 1
#define write_csr(csr, value) ({})

#ifndef PRODUCTION
#define DEBUG_WRAPER printf
#else
#define DEBUG_WRAPER
#endif

#endif //WRAPER_UTILS_H
