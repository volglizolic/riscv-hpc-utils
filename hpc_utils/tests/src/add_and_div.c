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
#include <getopt.h>

int main(int argc, char** argv){
    int additions = 100000, divisions = 1000;
    int opt;
    while ((opt = getopt(argc, argv, "a:d:")) != -1) {
        switch (opt) {
            case 'a':
                additions = atoi(optarg);
                break;
            case 'd':
                divisions = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-a no_additions] [-d no_divisions]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    cpu_set_t cpuset;
    for (int i = 0; i < 4; ++i) {
        int k;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        if ((k = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))){
            fprintf(stderr,"Could not set cpu affinity to %d. Output: %d.\t%s\n", i, k, strerror(errno));
        }
        for (int j = 0; j < additions * (i+1); ++j) {
            k = k * (i+1) - i;
        }
        for (int j = 0; j < divisions * (i+1); ++j) {
            k = k / (i+1) - i;
        }
    }

    return 0;


}
