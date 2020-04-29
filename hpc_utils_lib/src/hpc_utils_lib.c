//
// Created by volglizolic on 4/22/20.
//

#define _GNU_SOURCE

#include "../include/hpc_utils.h"
#include "../../utils/CSR_codes.h"
#include "fu540.h"
#include "event_definition.h"
#include "../../utils/asm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <pthread.h>



int hpc_init_event_selector(hpc_event_selector_t *hpc_event_selector) {
    if(hpc_event_selector == NULL) {
        return -1;
    }
    memset(hpc_event_selector, 0, sizeof(hpc_event_selector_t));
    hpc_event_selector->first_time = true;
    return 0;
}



int hpc_init_event_selector_set(hpc_event_selector_set_t *hpc_event_selector_set) {
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
    }   \
    asm volatile ("csrw " HELPER(counter) ", %0" :: "rK"(k));

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
    CPU_ZERO(&(hpc_event_selector_set->full_cpu_set));
    for (int i = get_nprocs() - 1; i >= 0; i--) CPU_SET(i, &(hpc_event_selector_set->full_cpu_set));
    return 0;
}


int hpc_add_event_selector_to_set(hpc_event_selector_set_t* hpc_event_selector_set,
                                  hpc_event_selector_t *hpc_event_selector, unsigned int counter_number, cpu_set_t* cpu_set) {
    if (((1u << counter_number) & hpc_event_selector_set->available_counters) == 0) {
        errno = ENOSYS;
        return -1;
    }
    hpc_event_selector_set_internal_t *new_selector = malloc(sizeof(hpc_event_selector_set_internal_t));
    if (!new_selector) return -1;
    new_selector->counter_number = counter_number;
    mempcpy(&(new_selector->cpu_set), cpu_set ? cpu_set : &hpc_event_selector_set->full_cpu_set, sizeof(cpu_set_t));
    mempcpy(&(new_selector->events_mask), &(hpc_event_selector->events_mask), sizeof(hpc_events_t));
    new_selector->next = hpc_event_selector_set->head;
    hpc_event_selector_set->head = new_selector;
    return 0;
}

struct internal_apply_event_selector_set_t{
    hpc_event_selector_set_t *hpc_event_selector_set;
    unsigned cpu;
};

int apply_local_event_selector(hpc_events_t events_mask, unsigned int counter_number){
    int temp;
#define TRY_APPLY_EVENT_SELECTOR(csr, counter) \
    if(counter == counter_number) { \
        asm volatile ("csrw " HELPER(csr) ", %0"::"rK"(events_mask)); \
        asm volatile ("csrr %0, " HELPER(csr) : "=r"(temp)); \
        if (temp != events_mask) { \
            return -1; \
        } \
        break; \
    }

    do {
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT3, 3)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT4, 4)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT5, 5)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT6, 6)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT7, 7)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT8, 8)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT9, 9)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT10, 10)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT11, 11)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT12, 12)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT13, 13)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT14, 14)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT15, 15)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT16, 16)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT17, 17)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT18, 18)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT19, 19)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT20, 20)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT21, 21)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT22, 22)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT23, 23)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT24, 24)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT25, 25)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT26, 26)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT27, 27)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT28, 28)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT29, 29)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT30, 30)
        TRY_APPLY_EVENT_SELECTOR(CSR_MHPMEVENT31, 31)
    } while (false);
    return 0;
}

void *internal_apply_event_selector_set(void * arg){
    struct internal_apply_event_selector_set_t *data = (struct internal_apply_event_selector_set_t *) arg;
    hpc_event_selector_set_t *hpc_set = data->hpc_event_selector_set;
    unsigned int cpu_id = data->cpu;
    bool all_ok = true;
    for (hpc_event_selector_set_internal_t *t = hpc_set->head; t != NULL ; t = t->next){
        if(CPU_ISSET(cpu_id, &t->cpu_set))
            if(-1 == apply_local_event_selector(t->events_mask, t->counter_number))
                all_ok = false;
    }
    return (void *) (size_t) (all_ok ? 0 : -1);
}

int hpc_apply_event_selector_set(hpc_event_selector_set_t* hpc_event_selector_set){
    int no_procs = get_nprocs();
    pthread_t threads[no_procs];
    pthread_attr_t thread_attr[no_procs];
    cpu_set_t cpu_set[no_procs];
    bool created_successfully[no_procs];
    struct internal_apply_event_selector_set_t sets[no_procs];
    for(int i = 0; i < no_procs; i++){
        created_successfully[i] = false;
        sets[i].cpu = i;
        sets[i].hpc_event_selector_set = hpc_event_selector_set;
        if (pthread_attr_init(&thread_attr[i])!=0) continue;
        CPU_ZERO(&cpu_set[i]);
        CPU_SET(i, &cpu_set[i]);
        if(pthread_attr_setaffinity_np(&thread_attr[i], sizeof(cpu_set_t), &cpu_set[i])) continue;
        if(pthread_create(&threads[i], &thread_attr[i], internal_apply_event_selector_set, &sets[i]) == 0)
            created_successfully[i] = true;
    }
    int res = 0;
    for(int i = 0; i < no_procs; i++) {
        if(created_successfully[i]) {
            void  *status;
            pthread_join(threads[i], &status);
            if((int) (size_t) status == -1) res = -1;
        } else {
            res = -1;
        }
    }
    return res;
}

void hpc_delete_event_selector_set(hpc_event_selector_set_t* hpc_event_selector_set){
    struct hpc_event_selector_set_internal_t *current = hpc_event_selector_set->head, *prev;
    while (current!=NULL) {
        prev = current;
        current = current->next;
        free(prev);
    }
}


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

DEFINE_SET_EVENT_MASK(exception_taken, EXCEPTION_TAKEN)
DEFINE_SET_EVENT_MASK(integer_load_instruction_retired, INTEGER_LOAD_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(integer_store_instruction_retired, INTEGER_STORE_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(atomic_memory_operation_retired, ATOMIC_MEMORY_OPERATION_RETIRED)
DEFINE_SET_EVENT_MASK(system_instruction_retired, SYSTEM_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(integer_arithmetic_instruction_retired, INTEGER_ARITHMETIC_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(conditional_branch_retired, CONDITIONAL_BRANCH_RETIRED)
DEFINE_SET_EVENT_MASK(jal_instruction_retired, JAL_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(jalr_instruction_retired, JALR_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(integer_multiplication_instruction_retired, INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED)
DEFINE_SET_EVENT_MASK(integer_division_instruction_retired, INTEGER_DIVISION_INSTRUCTION_RETIRED)
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

DEFINE_UNSET_EVENT_MASK(exception_taken, EXCEPTION_TAKEN)
DEFINE_UNSET_EVENT_MASK(integer_load_instruction_retired, INTEGER_LOAD_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(integer_store_instruction_retired, INTEGER_STORE_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(atomic_memory_operation_retired, ATOMIC_MEMORY_OPERATION_RETIRED)
DEFINE_UNSET_EVENT_MASK(system_instruction_retired, SYSTEM_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(integer_arithmetic_instruction_retired, INTEGER_ARITHMETIC_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(conditional_branch_retired, CONDITIONAL_BRANCH_RETIRED)
DEFINE_UNSET_EVENT_MASK(jal_instruction_retired, JAL_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(jalr_instruction_retired, JALR_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(integer_multiplication_instruction_retired, INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED)
DEFINE_UNSET_EVENT_MASK(integer_division_instruction_retired, INTEGER_DIVISION_INSTRUCTION_RETIRED)
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

void *internal_get_csr_snapshot(void * arg){
    hpc_snapshot_t *snapshot = (hpc_snapshot_t *) arg;
    snapshot->cycles = read_csr(CSR_CYCLE);
    snapshot->time = read_csr(CSR_TIME);
    snapshot->instr_ret = read_csr(CSR_INSTRET);
    snapshot->counter3 = read_csr(CSR_HPMCOUNTER3);
    snapshot->counter4 = read_csr(CSR_HPMCOUNTER4);
    snapshot->counter5 = read_csr(CSR_HPMCOUNTER5);
    snapshot->counter6 = read_csr(CSR_HPMCOUNTER6);
    snapshot->counter7 = read_csr(CSR_HPMCOUNTER7);
    snapshot->counter8 = read_csr(CSR_HPMCOUNTER8);
    snapshot->counter9 = read_csr(CSR_HPMCOUNTER9);
    snapshot->counter10 = read_csr(CSR_HPMCOUNTER10);
    snapshot->counter11 = read_csr(CSR_HPMCOUNTER11);
    snapshot->counter12 = read_csr(CSR_HPMCOUNTER12);
    snapshot->counter13 = read_csr(CSR_HPMCOUNTER13);
    snapshot->counter14 = read_csr(CSR_HPMCOUNTER14);
    snapshot->counter15 = read_csr(CSR_HPMCOUNTER15);
    snapshot->counter16 = read_csr(CSR_HPMCOUNTER16);
    snapshot->counter17 = read_csr(CSR_HPMCOUNTER17);
    snapshot->counter18 = read_csr(CSR_HPMCOUNTER18);
    snapshot->counter19 = read_csr(CSR_HPMCOUNTER19);
    snapshot->counter20 = read_csr(CSR_HPMCOUNTER20);
    snapshot->counter21 = read_csr(CSR_HPMCOUNTER21);
    snapshot->counter22 = read_csr(CSR_HPMCOUNTER22);
    snapshot->counter23 = read_csr(CSR_HPMCOUNTER23);
    snapshot->counter24 = read_csr(CSR_HPMCOUNTER24);
    snapshot->counter25 = read_csr(CSR_HPMCOUNTER25);
    snapshot->counter26 = read_csr(CSR_HPMCOUNTER26);
    snapshot->counter27 = read_csr(CSR_HPMCOUNTER27);
    snapshot->counter28 = read_csr(CSR_HPMCOUNTER28);
    snapshot->counter29 = read_csr(CSR_HPMCOUNTER29);
    snapshot->counter30 = read_csr(CSR_HPMCOUNTER30);
    snapshot->counter31 = read_csr(CSR_HPMCOUNTER31);

#if __riscv_xlen == 32
    snapshot->cycles = ((hpc_counter_value_t) read_csr(CSR_CYCLEH)) << 32u;
    snapshot->time = ((hpc_counter_value_t) read_csr(CSR_TIMEH)) << 32u;
    snapshot->instr_ret = ((hpc_counter_value_t) read_csr(CSR_INSTRETH)) << 32u;
    snapshot->counter3 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER3H)) << 32u;
    snapshot->counter4 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER4H)) << 32u;
    snapshot->counter5 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER5H)) << 32u;
    snapshot->counter6 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER6H)) << 32u;
    snapshot->counter7 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER7H)) << 32u;
    snapshot->counter8 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER8H)) << 32u;
    snapshot->counter9 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER9H)) << 32u;
    snapshot->counter10 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER10H)) << 32u;
    snapshot->counter11 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER11H)) << 32u;
    snapshot->counter12 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER12H)) << 32u;
    snapshot->counter13 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER13H)) << 32u;
    snapshot->counter14 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER14H)) << 32u;
    snapshot->counter15 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER15H)) << 32u;
    snapshot->counter16 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER16H)) << 32u;
    snapshot->counter17 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER17H)) << 32u;
    snapshot->counter18 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER18H)) << 32u;
    snapshot->counter19 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER19H)) << 32u;
    snapshot->counter20 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER20H)) << 32u;
    snapshot->counter21 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER21H)) << 32u;
    snapshot->counter22 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER22H)) << 32u;
    snapshot->counter23 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER23H)) << 32u;
    snapshot->counter24 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER24H)) << 32u;
    snapshot->counter25 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER25H)) << 32u;
    snapshot->counter26 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER26H)) << 32u;
    snapshot->counter27 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER27H)) << 32u;
    snapshot->counter28 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER28H)) << 32u;
    snapshot->counter29 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER29H)) << 32u;
    snapshot->counter30 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER30H)) << 32u;
    snapshot->counter31 = ((hpc_counter_value_t) read_csr(CSR_HPMCOUNTER31H)) << 32u;
#endif
    return 0;
}

hpc_snapshot_set_t* hpc_get_csr_snapshot(cpu_set_t *cpu_set){
    int no_procs = get_nprocs();
    int active_procs = cpu_set ? CPU_COUNT(cpu_set) : no_procs;
    hpc_snapshot_set_t *hpc_snapshot_set = malloc(sizeof(struct hpc_snapshot_set_t) * active_procs);
    if(!hpc_snapshot_set)
        return NULL;
    pthread_t threads[active_procs];
    pthread_attr_t thread_attr[active_procs];
    cpu_set_t cpu_mask[active_procs];
    bool created_successfully[active_procs];
    for (int i = 0, j=0; i < no_procs; ++i) {
        if (no_procs == active_procs || CPU_ISSET(i, cpu_set)){
            created_successfully[j] = false;
            hpc_snapshot_set[j].cpu = i;
            hpc_snapshot_set[j].next = (j == active_procs-1)? NULL : &hpc_snapshot_set[j+1];
            if (pthread_attr_init(&thread_attr[j])!=0) continue;
            CPU_ZERO(&cpu_mask[j]);
            CPU_SET(i, &cpu_mask[j]);
            if(pthread_attr_setaffinity_np(&thread_attr[j], sizeof(cpu_set_t), &cpu_mask[j])) { j++; continue; }
            if(pthread_create(&threads[j], &thread_attr[j], internal_get_csr_snapshot, &(hpc_snapshot_set[j].snapshot)) == 0)
                created_successfully[j] = true;
            j++;
        }
    }
    for(int j = 0;  j < no_procs; j++) {
        if(created_successfully[j])
            pthread_join(threads[j], NULL);
    }
    return hpc_snapshot_set;
}

hpc_snapshot_set_t* hpc_snapshot_diff(hpc_snapshot_set_t* end, hpc_snapshot_set_t* start){
    hpc_snapshot_set_t *s, *e, *res;
    cpu_set_t s_set, e_set, anded_set, zero_set;
    CPU_ZERO(&s_set); CPU_ZERO(&e_set); CPU_ZERO(&zero_set);
    for (e = end; e != NULL; e = e->next) CPU_SET(e->cpu, &e_set);
    for (s = start; s != NULL; s = s->next) CPU_SET(s->cpu, &s_set);
    CPU_AND(&anded_set, &e_set, &s_set);
    if (CPU_EQUAL(&zero_set, &anded_set)) {return NULL;}
    int no_intersection = CPU_COUNT(&anded_set);
    res = malloc(sizeof(struct hpc_snapshot_set_t) * no_intersection);
    if(!res) return NULL;
    int i = 0;
    for(s = start, e = end; s!=NULL && e!=NULL;){
        if(s->cpu < e->cpu){
            s = s->next;
        } else if(e->cpu < s->cpu){
            e = e->next;
        } else { // if (e->cpu == s->cpu )
            res[i].cpu = e->cpu;
            res[i].next = (i == no_intersection-1)? NULL : &res[i+1];
            hpc_counter_value_t *newPtr = (hpc_counter_value_t *) &(res[i].snapshot),
                    *endPtr = (hpc_counter_value_t *) &(e->snapshot),
                    *startPtr = (hpc_counter_value_t *) &(s->snapshot);
            for (int j = 0; j < sizeof(hpc_snapshot_t) / sizeof(hpc_counter_value_t); newPtr++, endPtr++, startPtr++, j++)
                *newPtr = *endPtr - *startPtr;
            i++;
            s = s->next;
            e = e->next;
        }
    }
    return res;
}

hpc_snapshot_t* hpc_get_snapshot_of_cpu(hpc_snapshot_set_t* set, unsigned int cpu){
    for (hpc_snapshot_set_t *t = set; t != NULL ; t = t->next) {
        if(t->cpu == cpu) return &(set->snapshot);
    }
    return NULL;
}