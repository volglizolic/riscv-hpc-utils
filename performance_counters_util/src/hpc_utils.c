//
// Created by volglizolic on 4/22/20.
//

#include "../include/hpc_utils.h"
#include "../../utils/CSR_codes.h"
#include "fu540.h"
#include "event_definition.h"
#include "asm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>


int init_event_selector(hpc_event_selector_t *hpc_event_selector) {
    if(hpc_event_selector == NULL) {
        return -1;
    }
    memset(hpc_event_selector, 0, sizeof(hpc_event_selector_t));
    hpc_event_selector->first_time = true;
    return 0;
};



int init_event_selector_set(hpc_event_selector_set_t *hpc_event_selector_set) {
    if(no_available_event_classes == 0) {
        errno = ENOSYS;
        return -1;
    }
    if(hpc_event_selector_set == NULL) {
        errno = EINVAL;
        return -1;
    }

#ifdef STATIC_COUNTER_ENABLED
    hpc_event_selector_set->available_counters = COUNTERS_MASK;
    hpc_event_selector_set->no_available_counters = NUMBER_OF_COUNTERS;
#else
    #define TEST_COUNTER_ENABLED(counter, counter_number) \
    asm volatile ("csrr %0, " HELPER(counter) : "=r"(k)); \
    asm volatile ("csrw " HELPER(counter) ", %0" :: "rK"(legit_event)); \
    asm volatile ("csrr %0, " HELPER(counter) : "=r"(k2)); \
    if(k2 == legit_event){ \
        hpc_event_selector_set->no_available_counters++; \
        hpc_event_selector_set->available_counters |= 1u << counter_number##u; \
    } else { \
        printf("%d is not implemented\n", counter_number); \
    }   \
    asm volatile ("csrw " HELPER(counter) ", %0" :: "rK"(legit_event));

    //This is not tested
    hpc_event_selector_set->available_counters = 0u;
    hpc_event_selector_set->no_available_counters = 0;
    int k = 0, k2 = 0;
    events_t legit_event = available_event_class[0].events_mask[0];
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT3, 3);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT4, 4);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT5, 5);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT6, 6);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT7, 7);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT8, 8);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT9, 9);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT10, 10);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT11, 11);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT12, 12);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT13, 13);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT14, 14);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT15, 15);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT16, 16);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT17, 17);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT18, 18);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT19, 19);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT20, 20);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT21, 21);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT22, 22);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT23, 23);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT24, 24);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT25, 25);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT26, 26);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT27, 27);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT28, 28);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT29, 29);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT30, 30);
    TEST_COUNTER_ENABLED(CSR_MHPMEVENT31, 31);
#endif
    hpc_event_selector_set->head = NULL;
    return 0;
};


//find appropriate class
//find if event supported in class and set mask if so 
//If event is supported but class is wrong, return invalid argument EINVAL, else return function not supported ENOSYS
#define DEFINE_SET_EVENT_MASK(x, mask) int hpc_set_##x##_event(hpc_event_selector_t * hpcEventSelector){ \
    for (const available_event_class_t *av_event_class = available_event_class; \
            av_event_class < available_event_class + no_available_event_classes; \
            ++av_event_class) \
        if (hpcEventSelector->first_time || hpcEventSelector->event_class_mask == av_event_class->event_class_mask) \
            for(int i = 0; i < av_event_class->no_events; i++) \
        		if(av_event_class->events_mask[i] == mask){ \
        		    if(hpcEventSelector->first_time){ \
		    			hpcEventSelector->event_class_mask = av_event_class->event_class_mask; \
            	    	hpcEventSelector->events_mask = av_event_class->event_class_mask | mask; \
						hpcEventSelector->first_time = false; \
            	    	return 0; \
					} else { \
						hpcEventSelector->events_mask |= mask; \
						return 0; \
					} \
				} \
	for (const available_event_class_t *av_event_class = available_event_class; \
			av_event_class < available_event_class + no_available_event_classes; \
			++av_event_class) \
       	for(int i = 0; i < av_event_class->no_events; i++) \
        	if(av_event_class->events_mask[i] == mask){ \
		   		errno = EINVAL; \
				return -1; \
			} \
	errno = ENOSYS; \
	return -1; \
} 

DEFINE_SET_EVENT_MASK(exception_taken, EXCEPTION_TAKEN_EVENT)
DEFINE_SET_EVENT_MASK(integer_load_instruction_retired, INTEGER_LOAD_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(integer_store_instruction_retired, INTEGER_STORE_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(atomic_memory_operation_retired, ATOMIC_MEMORY_OPERATION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(system_instruction_retired, SYSTEM_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(integer_arithmetic_instruction_retired, INTEGER_ARITHMETIC_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(conditional_branch_retired, CONDITIONAL_BRANCH_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(jal_instruction_retired, JAL_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(jalr_instruction_retired, JALR_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(integer_multiplication_instruction_retired, INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(integer_division_instruction_retired, INTEGER_DIVISION_INSTRUCTION_RETIRED_EVENT)
DEFINE_SET_EVENT_MASK(floating_point_load_instruction_retired, FLOATING_POINT_LOAD_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(floating_point_store_instruction_retired, FLOATING_POINT_STORE_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(floating_point_addition_retired, FLOATING_POINT_ADDITION_RETIRED)
DEFINE_SET_EVENT_MASK(floating_point_multiplication_retired, FLOATING_POINT_MULTIPLICATION_RETIRED)
DEFINE_SET_EVENT_MASK(floating_point_fused_multiplication_retired, FLOATING_POINT_FUSED_MULTIPLY_ADD_RETIRED)
DEFINE_SET_EVENT_MASK(floating_point_division_or_square_root_retired, FLOATING_POINT_DIVISION_OR_SQUARE_ROOT_RETIRED)
DEFINE_SET_EVENT_MASK(other_floating_point_instruction_retired, OTHER_FLOATING_POINT_INSTRUCTION_RETIRED)

DEFINE_SET_EVENT_MASK(load_use_interlock, LOAD_USE_INTERLOCK)
DEFINE_SET_EVENT_MASK(long_latency_interlock, LONG_LATENCY_INTERLOCK)
DEFINE_SET_EVENT_MASK(csr_read_interlock, CSR_READ_INTERLOCK)
DEFINE_SET_EVENT_MASK(instruction_cache_or_dtim_busy, INSTRUCTION_CACHE_OR_DTIM_BUSY)
DEFINE_SET_EVENT_MASK(data_cache_or_dtim_busy, DATA_CACHE_OR_DTIM_BUSY)
DEFINE_SET_EVENT_MASK(branch_direction_misprediction, BRANCH_DIRECTION_MISPREDICTION)
DEFINE_SET_EVENT_MASK(branch_jump_target_misprediction, BRANCH_JUMP_TARGET_MISPREDICTION)
DEFINE_SET_EVENT_MASK(pipeline_flush_from_csr_write, PIPELINE_FLUSH_FROM_CSR_WRITE)
DEFINE_SET_EVENT_MASK(pipeline_flush_from_other_event, PIPELINE_FLUSH_FROM_OTHER_EVENT)
DEFINE_SET_EVENT_MASK(integer_multiplication_interlock, INTEGER_MULTIPLICATION_INTERLOCK)
DEFINE_SET_EVENT_MASK(floating_point_interlock, FLOATING_POINT_INTERLOCK)

DEFINE_SET_EVENT_MASK(instruction_cache_miss, INSTRUCTION_CACHE_MISS)
DEFINE_SET_EVENT_MASK(data_cache_miss_or_memory_mapped_io_access, DATA_CACHE_MISS_OR_MEMORY_MAPPED_IO_ACCESS)
DEFINE_SET_EVENT_MASK(data_cache_writeback, DATA_CACHE_WRITEBACK)
DEFINE_SET_EVENT_MASK(instruction_tlb_miss, INSTRUCTION_TLB_MISS)
DEFINE_SET_EVENT_MASK(data_tlb_miss, DATA_TLB_MISS)


#define DEFINE_UNSET_EVENT_MASK(x, mask) int hpc_unset_##x##_event(hpc_event_selector_t * hpcEventSelector){ \
    if (hpcEventSelector->events_mask == mask) { \
        hpcEventSelector->event_class_mask = 0; \
        hpcEventSelector->events_mask = 0; \
        hpcEventSelector->first_time = true; \
        return 0; \
    } \
    hpcEventSelector->events_mask &= ~mask | event_class_mask; \
    return 0; \
}

DEFINE_UNSET_EVENT_MASK(exception_taken, EXCEPTION_TAKEN_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_load_instruction_retired, INTEGER_LOAD_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_store_instruction_retired, INTEGER_STORE_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(atomic_memory_operation_retired, ATOMIC_MEMORY_OPERATION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(system_instruction_retired, SYSTEM_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_arithmetic_instruction_retired, INTEGER_ARITHMETIC_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(conditional_branch_retired, CONDITIONAL_BRANCH_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(jal_instruction_retired, JAL_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(jalr_instruction_retired, JALR_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_multiplication_instruction_retired, INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_division_instruction_retired, INTEGER_DIVISION_INSTRUCTION_RETIRED_EVENT)
DEFINE_UNSET_EVENT_MASK(floating_point_load_instruction_retired, FLOATING_POINT_LOAD_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(floating_point_store_instruction_retired, FLOATING_POINT_STORE_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(floating_point_addition_retired, FLOATING_POINT_ADDITION_RETIRED)
DEFINE_UNSET_EVENT_MASK(floating_point_multiplication_retired, FLOATING_POINT_MULTIPLICATION_RETIRED)
DEFINE_UNSET_EVENT_MASK(floating_point_fused_multiplication_retired, FLOATING_POINT_FUSED_MULTIPLY_ADD_RETIRED)
DEFINE_UNSET_EVENT_MASK(floating_point_division_or_square_root_retired, FLOATING_POINT_DIVISION_OR_SQUARE_ROOT_RETIRED)
DEFINE_UNSET_EVENT_MASK(other_floating_point_instruction_retired, OTHER_FLOATING_POINT_INSTRUCTION_RETIRED)

DEFINE_UNSET_EVENT_MASK(load_use_interlock, LOAD_USE_INTERLOCK)
DEFINE_UNSET_EVENT_MASK(long_latency_interlock, LONG_LATENCY_INTERLOCK)
DEFINE_UNSET_EVENT_MASK(csr_read_interlock, CSR_READ_INTERLOCK)
DEFINE_UNSET_EVENT_MASK(instruction_cache_or_dtim_busy, INSTRUCTION_CACHE_OR_DTIM_BUSY)
DEFINE_UNSET_EVENT_MASK(data_cache_or_dtim_busy, DATA_CACHE_OR_DTIM_BUSY)
DEFINE_UNSET_EVENT_MASK(branch_direction_misprediction, BRANCH_DIRECTION_MISPREDICTION)
DEFINE_UNSET_EVENT_MASK(branch_jump_target_misprediction, BRANCH_JUMP_TARGET_MISPREDICTION)
DEFINE_UNSET_EVENT_MASK(pipeline_flush_from_csr_write, PIPELINE_FLUSH_FROM_CSR_WRITE)
DEFINE_UNSET_EVENT_MASK(pipeline_flush_from_other_event, PIPELINE_FLUSH_FROM_OTHER_EVENT)
DEFINE_UNSET_EVENT_MASK(integer_multiplication_interlock, INTEGER_MULTIPLICATION_INTERLOCK)
DEFINE_UNSET_EVENT_MASK(floating_point_interlock, FLOATING_POINT_INTERLOCK)

DEFINE_UNSET_EVENT_MASK(instruction_cache_miss, INSTRUCTION_CACHE_MISS)
DEFINE_UNSET_EVENT_MASK(data_cache_miss_or_memory_mapped_io_access, DATA_CACHE_MISS_OR_MEMORY_MAPPED_IO_ACCESS)
DEFINE_UNSET_EVENT_MASK(data_cache_writeback, DATA_CACHE_WRITEBACK)
DEFINE_UNSET_EVENT_MASK(instruction_tlb_miss, INSTRUCTION_TLB_MISS)
DEFINE_UNSET_EVENT_MASK(data_tlb_miss, DATA_TLB_MISS)

int apply_event_selector(hpc_event_selector_t hpc_ev_sel, int counter){
    int temp;
#define TRY_APPLY_EVENT_SELECTOR(csr, counter_num) \
    if(counter == counter_num) { \
        asm volatile ("csrw " HELPER(csr) ", %0"::"rK"(hpc_ev_sel.events_mask)); \
        asm volatile ("csrr %0, " HELPER(csr) : "=r"(temp)); \
        if (temp != hpc_ev_sel.events_mask) { \
            return -1; \
        } \
        break; \
    }

    do {
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT3, 3);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT4, 4);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT5, 5);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT6, 6);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT7, 7);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT8, 8);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT9, 9);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT10, 10);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT11, 11);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT12, 12);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT13, 13);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT14, 14);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT15, 15);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT16, 16);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT17, 17);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT18, 18);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT19, 19);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT20, 20);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT21, 21);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT22, 22);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT23, 23);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT24, 24);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT25, 25);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT26, 26);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT27, 27);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT28, 28);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT29, 29);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT30, 30);
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT31, 31);
    } while (false);
    return 0;
}