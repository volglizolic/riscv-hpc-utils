//
// Created by volglizolic on 4/22/20.
//

#ifndef PERFORMANCE_COUNTERS_UTILS_HPC_UTILS_H
#define PERFORMANCE_COUNTERS_UTILS_HPC_UTILS_H

#define _GNU_SOURCE

#include <sched.h>
#include <stdbool.h>


typedef unsigned long long hpc_counter_value_t;

#if __riscv_xlen == 32
typedef unsigned int hpc_events_t;
typedef unsigned int hpc_event_class_t;
#else
typedef unsigned long long hpc_events_t;
typedef unsigned long long hpc_event_class_t;
#endif


typedef struct  __attribute__((packed)) __attribute__((aligned((64)))){
    hpc_counter_value_t cycles;
    hpc_counter_value_t time;
    hpc_counter_value_t instr_ret;
    hpc_counter_value_t counter3;
    hpc_counter_value_t counter4;
    hpc_counter_value_t counter5;
    hpc_counter_value_t counter6;
    hpc_counter_value_t counter7;
    hpc_counter_value_t counter8;
    hpc_counter_value_t counter9;
    hpc_counter_value_t counter10;
    hpc_counter_value_t counter11;
    hpc_counter_value_t counter12;
    hpc_counter_value_t counter13;
    hpc_counter_value_t counter14;
    hpc_counter_value_t counter15;
    hpc_counter_value_t counter16;
    hpc_counter_value_t counter17;
    hpc_counter_value_t counter18;
    hpc_counter_value_t counter19;
    hpc_counter_value_t counter20;
    hpc_counter_value_t counter21;
    hpc_counter_value_t counter22;
    hpc_counter_value_t counter23;
    hpc_counter_value_t counter24;
    hpc_counter_value_t counter25;
    hpc_counter_value_t counter26;
    hpc_counter_value_t counter27;
    hpc_counter_value_t counter28;
    hpc_counter_value_t counter29;
    hpc_counter_value_t counter30;
    hpc_counter_value_t counter31;
} hpc_snapshot_t;

typedef struct hpc_snapshot_set_t {
    struct hpc_snapshot_set_t * next;
    unsigned int cpu;
    hpc_snapshot_t snapshot;
}hpc_snapshot_set_t;

typedef struct {
    hpc_event_class_t event_class_mask;
    hpc_events_t events_mask;
    bool first_time;
} hpc_event_selector_t;

typedef struct hpc_event_selector_set_internal_t{
    struct hpc_event_selector_set_internal_t *next;
    hpc_events_t events_mask;
    unsigned int counter_number;
    cpu_set_t cpu_set;
} hpc_event_selector_set_internal_t;

typedef struct {
    struct hpc_event_selector_set_internal_t *head;
    unsigned int available_counters;
    int no_available_counters;
    cpu_set_t full_cpu_set;
} hpc_event_selector_set_t;

extern int hpc_init_event_selector(hpc_event_selector_t *hpc_event_selector);

extern int hpc_init_event_selector_set(hpc_event_selector_set_t* hpc_event_selector_set);
extern int hpc_add_event_selector_to_set(hpc_event_selector_set_t* hpc_event_selector_set,
                                  hpc_event_selector_t *hpc_event_selector, unsigned int counter_number, cpu_set_t* cpu_set);
extern int hpc_apply_event_selector_set(hpc_event_selector_set_t* hpc_event_selector_set);
extern void hpc_delete_event_selector_set(hpc_event_selector_set_t* hpc_event_selector_set);

extern int hpc_set_exception_taken_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_load_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_store_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_atomic_memory_operation_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_system_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_arithmetic_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_conditional_branch_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_jal_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_jalr_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_multiplication_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_division_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_load_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_store_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_addition_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_multiplication_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_fused_multiplication_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_floating_point_division_or_square_root_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_other_floating_point_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);

extern int hpc_set_load_use_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_long_latency_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_csr_read_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_instruction_cache_or_dtim_busy_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_data_cache_or_dtim_busy_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_branch_direction_misprediction_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_branch_jump_target_misprediction_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_pipeline_flush_from_csr_write_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_pipeline_flush_from_other_event_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_integer_multiplication_interlock_event(hpc_event_selector_t *hpc_event_mask);

extern int hpc_set_instruction_cache_miss_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_data_cache_miss_or_memory_mapped_io_access_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_data_cache_writeback_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_instruction_tlb_miss_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_set_data_tlb_miss_event(hpc_event_selector_t *hpc_event_mask);

extern int hpc_unset_exception_taken_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_load_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_store_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_atomic_memory_operation_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_system_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_arithmetic_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_conditional_branch_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_jal_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_jalr_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_multiplication_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_division_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_load_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_store_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_addition_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_multiplication_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_fused_multiplication_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_floating_point_division_or_square_root_retired_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_other_floating_point_instruction_retired_event(hpc_event_selector_t *hpc_event_mask);

extern int hpc_unset_load_use_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_long_latency_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_csr_read_interlock_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_instruction_cache_or_dtim_busy_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_data_cache_or_dtim_busy_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_branch_direction_misprediction_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_branch_jump_target_misprediction_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_pipeline_flush_from_csr_write_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_pipeline_flush_from_other_event_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_integer_multiplication_interlock_event(hpc_event_selector_t *hpc_event_mask);

extern int hpc_unset_instruction_cache_miss_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_data_cache_miss_or_memory_mapped_io_access_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_data_cache_writeback_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_instruction_tlb_miss_event(hpc_event_selector_t *hpc_event_mask);
extern int hpc_unset_data_tlb_miss_event(hpc_event_selector_t *hpc_event_mask);

extern hpc_snapshot_set_t* hpc_get_csr_snapshot(cpu_set_t *cpu_set);

extern hpc_snapshot_set_t* hpc_snapshot_diff(hpc_snapshot_set_t* end, hpc_snapshot_set_t* start);

extern hpc_snapshot_t* hpc_get_snapshot_of_cpu(hpc_snapshot_set_t* set, unsigned int cpu);

#endif //PERFORMANCE_COUNTERS_UTILS_HPC_UTILS_H
