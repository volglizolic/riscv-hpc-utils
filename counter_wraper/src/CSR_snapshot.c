//
// Created by volglizolic on 1/30/20.
//
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include "../../utils/CSR_codes.h"
#include "CSR_snapshot.h"

void print_snapshot(CSR_snapshot *res){
    printf("Cycles:\t%ld\n"
           "Instructions Retired:\t%ld\n"
           "Timer:\t%ld\n"
           "Machine Hardware Performance Monitor Counter 3:\t%ld\n"
           "Machine Hardware Performance Monitor Counter 4:\t%ld\n",
           res->cycle, res->instret, res->time, res->m_hpm_csr_3, res->m_hpm_csr_4);
}

CSR_snapshot *get_snapshot(){
    CSR_snapshot *newSnapshot = malloc(sizeof(CSR_snapshot));
    newSnapshot->cycle = read_csr(CSR_CYCLE);
    newSnapshot->instret = read_csr(CSR_INSTRET);
    newSnapshot->time = read_csr(CSR_TIME);
    newSnapshot->m_hpm_csr_3 = read_csr(CSR_MHPMCOUNTER3);
    newSnapshot->m_hpm_csr_4 = read_csr(CSR_MHPMCOUNTER4);
    return newSnapshot;
}

void * in_thread_function(void *hart_id){
    int *hartId = (int *) hart_id;
    pthread_t thread = pthread_self();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(*hartId, &cpuset);
    int k;
    if ((k = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset))){
        fprintf(stderr,"Could not set cpu affinity to %d. Output: %d.\t%s\n", *hartId, k, strerror(errno));
    }
    return (void *) get_snapshot();
}

CSR_snapshot *get_snapshot_from_hart_id(int hartId){
    pthread_t tid;
    void *status = 0;
    if(pthread_create(&tid, NULL, in_thread_function, &hartId))
        fprintf(stderr,"Error creating thread in hard %d\n", hartId);
    if(pthread_join(tid, &status))
        fprintf(stderr, "Error joining thread in hart %d\n", hartId);
    CSR_snapshot *snapshot = (CSR_snapshot *) status;
    return snapshot;
}

CSR_snapshot *snapshot_subtraction(CSR_snapshot *end, CSR_snapshot *start){
    CSR_snapshot *newSnapshot = malloc(sizeof(CSR_snapshot));
    unsigned long   *newPtr = (unsigned long *) newSnapshot,
                    *endPtr = (unsigned long *) end,
                    *startPtr = (unsigned long *) start;
    for (unsigned long i = 0; i < sizeof(CSR_snapshot) / sizeof(long); newPtr++, endPtr++, startPtr++, i++)
        *newPtr = *endPtr - *startPtr;
    return newSnapshot;
}

