//
// Created by volglizolic on 4/23/20.
//

#ifndef WRAPER_EVENT_DEFINITION_H
#define WRAPER_EVENT_DEFINITION_H

typedef struct {
    unsigned long event_class_mask;
    int no_events;
#if __riscv_xlen == 32
    unsigned int events_mask[32];
#else
    unsigned long long events_mask[64];
#endif
} available_event_class_t;

#define NUM_OF_ARGS(...)  (sizeof((int[]){0, ##__VA_ARGS__})/sizeof(int)-1)

#define DECLARE_CLASS_EVENTS( x, ... ) \
    {.event_class_mask = x, .events_mask = {__VA_ARGS__}, .no_events = NUM_OF_ARGS(__VA_ARGS__) },

#include "fu540.h"

#if __riscv_xlen == 32
static const unsigned int event_class_mask = EVENT_CLASS_MASK;
#else
static const unsigned long long event_class_mask = EVENT_CLASS_MASK;
#endif

static const available_event_class_t available_event_class[] = {
DECLARE_CLASS_EVENTS(INSTRUCTION_COMMIT_EVENT_CLASS,
                     EXCEPTION_TAKEN_EVENT,
                     INTEGER_LOAD_INSTRUCTION_RETIRED_EVENT,
                     INTEGER_STORE_INSTRUCTION_RETIRED_EVENT,
                     ATOMIC_MEMORY_OPERATION_RETIRED_EVENT,
                     SYSTEM_INSTRUCTION_RETIRED_EVENT,
                     INTEGER_ARITHMETIC_INSTRUCTION_RETIRED_EVENT,
                     CONDITIONAL_BRANCH_RETIRED_EVENT,
                     JAL_INSTRUCTION_RETIRED_EVENT,
                     JALR_INSTRUCTION_RETIRED_EVENT,
                     INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED_EVENT,
                     INTEGER_DIVISION_INSTRUCTION_RETIRED_EVENT,
                     FLOATING_POINT_LOAD_INSTRUCTION_RETIRED,
                     FLOATING_POINT_STORE_INSTRUCTION_RETIRED,
                     FLOATING_POINT_ADDITION_RETIRED,
                     FLOATING_POINT_MULTIPLICATION_RETIRED,
                     FLOATING_POINT_FUSED_MULTIPLY_ADD_RETIRED,
                     FLOATING_POINT_DIVISION_OR_SQUARE_ROOT_RETIRED,
                     OTHER_FLOATING_POINT_INSTRUCTION_RETIRED)

DECLARE_CLASS_EVENTS(MICROARCHITECTURAL_EVENT_CLASS,
                     LOAD_USE_INTERLOCK,
                     LONG_LATENCY_INTERLOCK,
                     CSR_READ_INTERLOCK,
                     INSTRUCTION_CACHE_OR_DTIM_BUSY,
                     DATA_CACHE_OR_DTIM_BUSY,
                     BRANCH_DIRECTION_MISPREDICTION,
                     BRANCH_JUMP_TARGET_MISPREDICTION,
                     PIPELINE_FLUSH_FROM_CSR_WRITE,
                     PIPELINE_FLUSH_FROM_OTHER_EVENT,
                     INTEGER_MULTIPLICATION_INTERLOCK,
                     FLOATING_POINT_INTERLOCK)

DECLARE_CLASS_EVENTS(MEMORY_SYSTEM_EVENT_CLASS,
                     INSTRUCTION_CACHE_MISS,
                     DATA_CACHE_MISS_OR_MEMORY_MAPPED_IO_ACCESS,
                     DATA_CACHE_WRITEBACK,
                     INSTRUCTION_TLB_MISS,
                     DATA_TLB_MISS)
};

static const int no_available_event_classes = sizeof(available_event_class) / sizeof(available_event_class[0]);

#endif //WRAPER_EVENT_DEFINITION_H
