//
// Created by Volglizolic on 4/29/20.
//

#include "help.h"
#include <stdio.h>
#include <stdlib.h>

void print_usage(char * name) {
#define SPACES(x) "                                   "x
#define SSPACES(x) "                                     "x
#define SSSPACES(x) "                                       "x
#define NSPACES(x) x

#define PRINT_OPT(arg,opt,desc) printf("%2s-%-2s%-30s%-20s\n", " ", arg, opt, desc)
#define PRINT_SUBOPT(opt,desc)  printf("%7s%-30s%-20s\n", " ", opt, desc)

    printf("Usage: %s [options] <prog-and-arg>\n"
           "Measures hardware events in riscv platform using the available hardware performance counters.\n\n"
           "Options:\n", name);
    PRINT_OPT("c", "<COUNTER>[,CPU][,EVENT]", "program event counter");
    PRINT_SUBOPT("<COUNTER>", "specify counter number: 3-31\n" SSSPACES("available counters vary depending on platform"));
    PRINT_SUBOPT("[,CPU]", "list of cpus in which counters are programmed\n" SSSPACES("if left empty all cpus are programmed accordingly"));
    PRINT_SUBOPT("[,EVENT]", "list of events to be counted\n" SSSPACES("available counters vary depending on platform"));
    printf(SSSPACES("NOTE that not all events are compatible\n")SSSPACES("in the same counter at the same time\n"));
    printf(SSSPACES("For the list of compatible events,\n")SSSPACES("check the platform specific part of this message bellow\n"));
    printf(SSSPACES("list of available events:\n"));
    printf(SSSPACES("et\texception taken\n"));
    printf(SSSPACES("ilir\tinteger load instruction retired\n"));
    printf(SSSPACES("isir\tinteger store instruction retired\n"));
    printf(SSSPACES("amor\tatomic memory operation retired\n"));
    printf(SSSPACES("sir\tsystem instruction retired\n"));
    printf(SSSPACES("iair\tinteger arithmetic instruction retired\n"));
    printf(SSSPACES("cbr\tconditional branch retired\n"));
    printf(SSSPACES("jalir\tjal instruction retired\n"));
    printf(SSSPACES("jalrir\tjalr instruction retired\n"));
    printf(SSSPACES("imir\tinteger multiplication instruction retired\n"));
    printf(SSSPACES("idir\tinteger division instruction retired\n"));
    printf(SSSPACES("fplir\tfloating point load instruction retired\n"));
    printf(SSSPACES("fpsir\tfloating point store instruction retired\n"));
    printf(SSSPACES("fpar\tfloating point addition retired\n"));
    printf(SSSPACES("fpmr\tfloating point multiplication retired\n"));
    printf(SSSPACES("fpfmr\tfloating point fused multiplication retired\n"));
    printf(SSSPACES("fpdosrr\tfloating point division or square root retired\n"));
    printf(SSSPACES("ofpir\tother floating point instruction retired\n"));
    printf(SSSPACES("lui\tload use interlock\n"));
    printf(SSSPACES("lli\tlong latency interlock\n"));
    printf(SSSPACES("csrri\tcsr read interlock\n"));
    printf(SSSPACES("icodtimb\tinstruction cache or dtim busy\n"));
    printf(SSSPACES("dcodtimb\tdata cache or dtim busy\n"));
    printf(SSSPACES("bdm\tbranch direction misprediction\n"));
    printf(SSSPACES("bjtm\tbranch jump target misprediction\n"));
    printf(SSSPACES("pffcsrw\tpipeline flush from csr write\n"));
    printf(SSSPACES("pffoe\tpipeline flush from other event\n"));
    printf(SSSPACES("imi\tinteger multiplication interlock\n"));
    printf(SSSPACES("icm\tinstruction cache miss\n"));
    printf(SSSPACES("dcmommioa\tdata cache miss or memory mapped io access\n"));
    printf(SSSPACES("dcw\tdata cache writeback\n"));
    printf(SSSPACES("itlbm\tinstruction tlb miss\n"));
    printf(SSSPACES("dtlbm\tdata tlb miss\n"));
    PRINT_OPT("r","<FILE>","redirect <prog-and-arg> output to FILE");
    PRINT_OPT("o","<FILE>","print counters output to FILE");
    PRINT_OPT("h","","displays this message");
    printf("\nPlatforms:\n");
    printf("%2sCompatible options depend on the specifics of the platform as listed bellow.\n\n","");
    printf("%4s%-31s" "SiFive FU540-C000 platform for U54 cores\n", "", "FU540-C000 U54");
    printf("%6s%-29s" "et, ilir, isir, amor, sir, iair, cbr,\n"
                      SSPACES("jalir, jalrir, imir, idir, fplir, fpsir,\n")
                      SSPACES("fpar, fpmr, fpfmar, fpdosrr, ofpir\n"), "", "Instruction Commit Events");
                      printf("%6s%-29s" "lui, lli, csrri, icodtimb, dcodtimb,\n"
                      SSPACES("bdm, bjtm, pffcsrw, pffoe, imi\n"),
                      "", "Microarchitectural Events");
    printf("%6s%-29s" "icm, dcmommioa, dcw, itlbm, dtlbm\n", "", "Memory System Events");

    printf("\nExample:\n");
    printf("%s -c3,0,2,3,et,ilir \"ls -la\"\n", name);
    printf(SPACES("count in hardware performance counter 3, of cpu: 0, 2 and 3, \n")
           SPACES("the number of \"Exception Taken\" or \"Integer Load Instruction Retired\" events.\n"));

    exit(EXIT_FAILURE);
}