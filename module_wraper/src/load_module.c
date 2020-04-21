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

int main(int argc, char** argv){
    if (argc != 4){
        fprintf(stderr,"Usage: %s [load|unload] <module_name> <cpu_num>\n", argv[0]);
        exit(-1);
    }

    if( access( argv[2], F_OK ) == -1 ) {
        fprintf(stderr,"Error: Could not access file %s\n", argv[2]);
        exit(-1);
    }

    char * load_command = malloc(255 * sizeof(char));
    char * unload_command = malloc(255 * sizeof(char));
    sprintf(load_command, "insmod %s\n", argv[2]);
    sprintf(unload_command, "rmmod %s\n", argv[2]);

    int k;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(atoi(argv[3]), &cpuset);
    if ((k = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset))){
        fprintf(stderr,"Could not set cpu affinity to %d. Output: %d.\t%s\n", atoi(argv[3]), k, strerror(errno));
    }

    char *const laod_parm_list[] = {"/sbin/insmod", argv[2], NULL};
    char *const unlaod_parm_list[] = {"/sbin/rmmod", argv[2], NULL};

    if(strcmp(argv[1], "load") == 0){
        execv("/sbin/insmod", laod_parm_list);
        printf("Return not expected. Must be an execv error.n");
    } else if (strcmp(argv[1], "unload") == 0){
        execv("/sbin/rmmod", unlaod_parm_list);
        printf("Return not expected. Must be an execv error.n");
    } else {
        fprintf(stderr,"Usage: %s [load|unload] <module_name>\n", argv[0]);
        exit(-1);
    }

    return 0;
}