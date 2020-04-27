//
// Created by volglizolic on 4/7/20.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include "../../utils/CSR_codes.h"

int main(int argc, char** argv){
    if (argc != 3){
        fprintf(stderr,"Usage: %s <register> <value>\n", argv[0]);
        exit(-1);
    }

    int k;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);
    if ((k = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))){
        fprintf(stderr,"Could not set cpu affinity to %d. Output: %d.\t%s\n", atoi(argv[3]), k, strerror(errno));
    }

    unsigned long counter = atoi(argv[1]), event = atoi(argv[2]);
    if ( event == 0)
        write_csr(CSR_MHPMEVENT3, INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED_EVENT);
    else
        write_csr(CSR_MHPMEVENT3, INTEGER_DIVISION_INSTRUCTION_RETIRED_EVENT);

    printf("Made it thus far\nShould be %lu, is %lu\n", event, read_csr(CSR_MHPMEVENT3));
    return 0;


}
