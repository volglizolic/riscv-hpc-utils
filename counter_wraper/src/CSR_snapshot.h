//
// Created by volglizolic on 1/30/20.
//

#ifndef WRAPER_CSR_SNAPSHOT_H
#define WRAPER_CSR_SNAPSHOT_H


typedef struct {
    unsigned long cycle;
    unsigned long time;
    unsigned long instret;
    unsigned long m_hpm_csr_3;
    unsigned long m_hpm_csr_4;
} CSR_snapshot __attribute__ ((aligned (64)));


//CSR_snapshot *get_snapshot();

CSR_snapshot *snapshot_subtraction(CSR_snapshot *end, CSR_snapshot *start);

void print_snapshot(CSR_snapshot *res);

CSR_snapshot *get_snapshot_from_hart_id(int hartId);


#endif //WRAPER_CSR_SNAPSHOT_H
