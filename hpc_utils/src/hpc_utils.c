#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>
#include <fcntl.h>

#include "../../hpc_utils_lib/include/hpc_utils.h"
#include "help.h"

int main(int argc, char **argv) {
    enum {
        ET, ILIR, ISIR, AMOR, SIR, IAIR, CBR, JALIR, JALRIR, IMIR, IDIR, FPLIR, FPSIR, FPAR, FPMR, FPFMAR, FPDOSRR, OFPIR,
        LUI, LLI, CSRRI, ICODTIMB, DCODTIMB, BDM, BJTM, PFFCSRW, PFFOE, IMI,
        ICM, DCMOMMIOA, DCW, ITLBM, DTLBM
    };
    char *const token[] = {
            [ET] = "et", [ILIR] = "ilir", [ISIR] = "isir", [AMOR] = "amor", [SIR] = "sir", [IAIR] = "iair", [CBR] = "cbr",
                [JALIR] = "jalir", [JALRIR] = "jalrir", [IMIR] = "imir", [IDIR] = "idir", [FPLIR] = "fplir", [FPSIR] = "fpsir",
                [FPAR] = "fpar", [FPMR] = "fpmr", [FPFMAR] = "fpfmar", [FPDOSRR] = "fpdosrr", [OFPIR] = "ofpir",
            [LUI] = "lui", [LLI] = "lli", [CSRRI] = "csrri", [ICODTIMB] = "icodtimb", [DCODTIMB] = "dcodtimb",
                [BDM] = "bdm", [BJTM] = "bjtm", [PFFCSRW] = "pffcsrw", [PFFOE] = "pffoe", [IMI] = "imi",
            [ICM] = "icm", [DCMOMMIOA] = "dcmommioa", [DCW] = "dcw", [ITLBM] = "itlbm", [DTLBM] = "dtlbm",
            NULL
    };

    int opt;
    char *subopts = NULL;
    char *value;

    unsigned int counter_num, cpu;
    bool set_cpu_set_null;
    bool print_counters[32] = {false};
    print_counters[0] = true; print_counters[1] = true; print_counters[2] = true;
    cpu_set_t cpu_set;
    hpc_event_selector_set_t set;
    hpc_init_event_selector_set(&set);
    hpc_event_selector_t selector;

    FILE *exec_output_file = stdout;
    FILE *utils_output_file = stdout;


    while ((opt = getopt(argc, argv, "c:r:o:h")) != -1) {
        switch (opt) {
            case 'c':
                hpc_init_event_selector(&selector);
                counter_num = strtol(optarg, &subopts, 10);
                if( counter_num < 3 || counter_num > 31 ) print_usage(argv[0]); //if first is non numeric n = 0
                if(subopts[0] != ','){
                    if(subopts[0] == '\0') break;
                    else print_usage(argv[0]);
                }
                optarg = ++subopts;
                CPU_ZERO(&cpu_set);
                set_cpu_set_null = true;
                while ((cpu = strtol(optarg, &subopts, 10)) != 0){
                    set_cpu_set_null = false;
                    CPU_SET(cpu, &cpu_set);
                    if(subopts[0]=='\0') break;
                    optarg = ++subopts;
                }
                if(subopts[0]=='\0') goto finalize_c;
                while (subopts[0] != '\0') {
                    switch (getsubopt(&subopts, token, &value)) {
#define CASE_EVENT(ENUM, FUNC) case ENUM: \
                                   if ( hpc_set_##FUNC##_event(&selector) == -1 ){ \
                                       fprintf(stderr, "incompatible events\n"); \
                                       exit(-1); \
                                   } \
                                   break;
                        CASE_EVENT(ET, exception_taken)
                        CASE_EVENT(ILIR, integer_load_instruction_retired)
                        CASE_EVENT(ISIR, integer_store_instruction_retired)
                        CASE_EVENT(AMOR, atomic_memory_operation_retired)
                        CASE_EVENT(SIR, system_instruction_retired)
                        CASE_EVENT(IAIR, integer_arithmetic_instruction_retired)
                        CASE_EVENT(CBR, conditional_branch_retired)
                        CASE_EVENT(JALIR, jal_instruction_retired)
                        CASE_EVENT(JALRIR, jalr_instruction_retired)
                        CASE_EVENT(IMIR, integer_multiplication_instruction_retired)
                        CASE_EVENT(IDIR, integer_division_instruction_retired)
                        CASE_EVENT(FPLIR, floating_point_load_instruction_retired)
                        CASE_EVENT(FPSIR, floating_point_store_instruction_retired)
                        CASE_EVENT(FPAR, floating_point_addition_retired)
                        CASE_EVENT(FPMR, floating_point_multiplication_retired)
                        CASE_EVENT(FPFMAR, floating_point_fused_multiplication_retired)
                        CASE_EVENT(FPDOSRR, floating_point_division_or_square_root_retired)
                        CASE_EVENT(OFPIR, other_floating_point_instruction_retired)
                        CASE_EVENT(LUI, load_use_interlock)
                        CASE_EVENT(LLI, long_latency_interlock)
                        CASE_EVENT(CSRRI, csr_read_interlock)
                        CASE_EVENT(ICODTIMB, instruction_cache_or_dtim_busy)
                        CASE_EVENT(DCODTIMB, data_cache_or_dtim_busy)
                        CASE_EVENT(BDM, branch_direction_misprediction)
                        CASE_EVENT(BJTM, branch_jump_target_misprediction)
                        CASE_EVENT(PFFCSRW, pipeline_flush_from_csr_write)
                        CASE_EVENT(PFFOE, pipeline_flush_from_other_event)
                        CASE_EVENT(IMI, integer_multiplication_interlock)
                        CASE_EVENT(ICM, instruction_cache_miss)
                        CASE_EVENT(DCMOMMIOA, data_cache_miss_or_memory_mapped_io_access)
                        CASE_EVENT(DCW, data_cache_writeback)
                        CASE_EVENT(ITLBM, instruction_tlb_miss)
                        CASE_EVENT(DTLBM, data_tlb_miss)
                        default:
                            fprintf(stderr, "invalid event: %s\n\n", value);
                            print_usage(argv[0]);
                            break;
                    }
                }
            finalize_c:
                hpc_add_event_selector_to_set(&set, &selector, counter_num, set_cpu_set_null? NULL : &cpu_set);
                print_counters[counter_num] = true;
                break;
            case 'r':
                exec_output_file = fopen(optarg, "a");
                if(exec_output_file == NULL){
                    fprintf(stderr,"could not open output file: %s", optarg);
                    exit(-1);
                }
                break;
            case 'o':
                utils_output_file = fopen(optarg, "a");
                if(utils_output_file == NULL){
                    fprintf(stderr,"could not open output file: %s", optarg);
                    exit(-1);
                }
                break;
            case 'h':
            default: /* '?' */
                print_usage(argv[0]);
        }
    }
    if(optind == argc) {
        fprintf(stderr,"No executable specified\n");
        print_usage(argv[0]);
    }
    
    hpc_apply_event_selector_set(&set);
    hpc_snapshot_set_t *start = hpc_get_csr_snapshot(NULL);
    FILE *executable = popen(argv[optind], "r");
    char buf[1024];
    int buflen;
    while((buflen = read(fileno(executable), buf, 1024)) > 0)
    {
        write(fileno(exec_output_file), buf, buflen);
    }
    pclose(executable);
    hpc_snapshot_set_t *end = hpc_get_csr_snapshot(NULL);
    hpc_snapshot_set_t *result = hpc_snapshot_diff(end, start);
    hpc_delete_event_selector_set(&set);
    fprintf(utils_output_file, "\nHardware Event Counters for: %s\n", argv[optind]);
    for (hpc_snapshot_set_t *t = result; t != NULL ; t = t->next) {
        fprintf(utils_output_file, "\nCPU  %d:\n", t->cpu);
#define PRINT_COUNTER(num, counter) if(print_counters[num]) fprintf(utils_output_file, "  %-12s%lld\n", #counter, t->snapshot.counter);
        PRINT_COUNTER(0, cycles)
        PRINT_COUNTER(1, time)
        PRINT_COUNTER(2, instr_ret)
        PRINT_COUNTER(3, counter3)
        PRINT_COUNTER(4, counter4)
        PRINT_COUNTER(5, counter5)
        PRINT_COUNTER(6, counter6)
        PRINT_COUNTER(7, counter7)
        PRINT_COUNTER(8, counter8)
        PRINT_COUNTER(9, counter9)
        PRINT_COUNTER(10, counter10)
        PRINT_COUNTER(11, counter11)
        PRINT_COUNTER(12, counter12)
        PRINT_COUNTER(13, counter13)
        PRINT_COUNTER(14, counter14)
        PRINT_COUNTER(15, counter15)
        PRINT_COUNTER(16, counter16)
        PRINT_COUNTER(17, counter17)
        PRINT_COUNTER(18, counter18)
        PRINT_COUNTER(19, counter19)
        PRINT_COUNTER(20, counter20)
        PRINT_COUNTER(21, counter21)
        PRINT_COUNTER(22, counter22)
        PRINT_COUNTER(23, counter23)
        PRINT_COUNTER(24, counter24)
        PRINT_COUNTER(25, counter25)
        PRINT_COUNTER(26, counter26)
        PRINT_COUNTER(27, counter27)
        PRINT_COUNTER(28, counter28)
        PRINT_COUNTER(29, counter29)
        PRINT_COUNTER(30, counter30)
        PRINT_COUNTER(31, counter31)
    }
    fclose(exec_output_file);
    fclose(utils_output_file);

    return 0;
}