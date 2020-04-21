#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>


#include "../../utils/CSR_codes.h"
#include "CSR_snapshot.h"





int main(int argc, char **argv) {
    if(argc<2){
        printf("%s:Usage %s <program to execute>\n", argv[0], argv[0]);
        exit(-1);
    }

    int k = 2;

    CSR_snapshot *end[5], *start[5];
    for (int i = 0; i < 4; ++i) {
        start[i] = get_snapshot_from_hart_id(i);
    }

//    if(strcmp(argv[1], "same") != 0) {
//        printf("\n\n----IN SAME-----\n\n");
//
//        FILE *fp = popen("./wraper_RiscV same","r");
//        char *ln = NULL;
//        size_t len = 0;
//
//        while (getline(&ln, &len, fp) != -1)
//            fputs(ln, stdout);
//        free(ln);
//        pclose(fp);
//    }

    cpu_set_t cpuset;
    for (int i = 0; i < 4; ++i) {
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



    for (int i = 0; i < 4; ++i) {
        end[i] = get_snapshot_from_hart_id(i);
    }
    printf("\n\n----k = %d------\n\n", k);

    for (int i = 0; i < 4; ++i) {
//        printf("\n\n----------------\n\nThread %d:\n\nStart:\n", i);
//        print_snapshot(start[i]);
//        printf("\nEnd:\n");
//        print_snapshot(end[i]);
        printf("\n\n----------------\n\nThread %d:\n", i);
        print_snapshot(snapshot_subtraction(end[i],start[i]));
    }

    return 0;
}