//
// Created by volglizolic on 4/27/20.
//

#ifndef WRAPER_ASM_H
#define WRAPER_ASM_H

#define HELPER(x) #x
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " HELPER(reg) : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " HELPER(reg) ", %0" :: "rK"(val)); })

#endif //WRAPER_ASM_H
