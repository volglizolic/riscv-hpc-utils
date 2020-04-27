//
// Created by volglizolic on 4/23/20.
//

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "event_definition.h"
#include "../include/hpc_utils.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int main(int argc, char** argv) {
    hpc_event_selector_set_t* hpc_event_selector_set = malloc(sizeof(hpc_event_selector_set_t));
    printf("%d\n", init_event_selector_set(hpc_event_selector_set));

    hpc_event_selector_t hpc_ev_sel;
    init_event_selector(&hpc_ev_sel);

//    event_class_t event_class = 1;
//    for (const available_event_class_t *av_event_class = available_event_class;
//         av_event_class < available_event_class + (sizeof(available_event_class)/sizeof(available_event_class[0]));
//         ++av_event_class) {
//         if (event_class == av_event_class->event_class_mask)
//            break;
//         if(av_event_class == available_event_class + sizeof(available_event_class)/sizeof(available_event_class[0]) - 1) {
//            printf("event_class not found\n");
//            return -1;
//        }
//    }
    printf("%u : %d\n", hpc_event_selector_set->available_counters, hpc_event_selector_set->no_available_counters);
    printBits(sizeof(hpc_event_selector_set->available_counters), & hpc_event_selector_set->available_counters);

}

