//
// Created by volglizolic on 4/7/20.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>

#include "../../include/hpc_utils.h"

int main(int argc, char** argv){

    /* To program the counters first we create an hpc_event_selector_t
     *  for every event bundle to be counted by any one counter.
     *
     *  hpc_event_selector_t are allocated and freed by the user */
    hpc_event_selector_t hpc_ev_sel_mul, hpc_ev_sel_div;

    /* Before we set the events to our selector, we must first initialize him. */
    hpc_init_event_selector(&hpc_ev_sel_mul);
    /* Then we add all the events that we want to trigger increment in a single counter. */
    /* Bellow we set the selector to integer or floating point multiplication */
    /* Note that SOME events (depending on platform) are incompatible between them,
     * and trying to set an event to a selector has incompatible events
     * will not succeed and return -1
     * To find which events are compatible check the platform specific vendor's manual
     * or check `./hpc_utils -h` */
    hpc_set_floating_point_multiplication_retired_event(&hpc_ev_sel_mul);
    hpc_set_integer_multiplication_instruction_retired_event(&hpc_ev_sel_mul);

    /* We create all the selectors we want. */
    hpc_init_event_selector(&hpc_ev_sel_div);
    hpc_set_integer_division_instruction_retired_event(&hpc_ev_sel_div);

    /* To apply the selectors to the counters, we must first add them to a set,
     * that we first initialize */
    hpc_event_selector_set_t hpc_event_selector_set;
    hpc_init_event_selector_set(&hpc_event_selector_set);

    /* when adding a selector to a set,
     * we specify the counter number that we want to program,
     * and the cpus in which we want to program the counter.
     * Every cpu has its own set of independent counters.
     * We specify the cpu set with a cpu_set_t type cpu_set from <sched.h>
     * If the cpu_set is NULL, then the selector is applied to all cpus across the system */
    hpc_add_event_selector_to_set(&hpc_event_selector_set, &hpc_ev_sel_mul, 3, NULL);
    hpc_add_event_selector_to_set(&hpc_event_selector_set, &hpc_ev_sel_div, 4, NULL);

    /* To finally program the counters, we must apply the set.
     * Before hpc_apply_event_selector_set, no counters are affected. */
    hpc_apply_event_selector_set(&hpc_event_selector_set);
    /* Before we free the hpc_event_selector_set_t,
     * we must call hpc_delete_event_selector_set */
    hpc_delete_event_selector_set(&hpc_event_selector_set);


    /* To take a snapshot from a list of cpus we call hpc_get_csr_snapshot.
     * Is the cpu_set is NULL, snapshots from all cpus are taken in parallel. */
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(1, &cpus);
    CPU_SET(3, &cpus);
    hpc_snapshot_set_t *t1 = hpc_get_csr_snapshot(&cpus);

    //this just runs some calculations
    cpu_set_t cpuset;
    for (int i = 0; i < 4; ++i) {
        int k;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        if ((k = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))){
            fprintf(stderr,"Could not set cpu affinity to %d. Output: %d.\t%s\n", i, k, strerror(errno));
        }
        for (int j = 0; j < 100000 * (i+1); ++j) {
            k = k * (i+1) - i;
        }
        for (int j = 0; j < 10000 * (i+1); ++j) {
            k = k / (i+1) - i;
        }
    }

    /* We take a second snapshot in the end of the segment we want to measure. */
    hpc_snapshot_set_t *t2 = hpc_get_csr_snapshot(&cpus);
    /* hpc_snapshot_diff provides the real value of the events occurred between t2 and t1.
     * The result contains the intersect of the cpu sets in t1 and t2.*/
    hpc_snapshot_set_t *res = hpc_snapshot_diff(t2, t1);

    /* The hpc_snapshot_set_t is the node containing a snapshot of a single cpu.
     * Those nodes form a linked list and elements are accessed as shown bellow.
     * For more information check the hpc_utils.h header file in `hpc_utils_lib/include`.*/

    hpc_snapshot_set_t *k1;
    for (k1 = res; k1 != NULL; k1 = k1->next) {
        printf("cpu: %d\t\t3: %lld\t4: %lld\n", k1->cpu, k1->snapshot.counter3, k1->snapshot.counter4);
    }

    return 0;


}
