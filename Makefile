#CC=riscv64-unknown-linux-gnu-gcc
#CFLAGS += -pthread -static
#SRCS =./src
#OUTPUT =../bin

default: counter_wraper module_wraper

counter_wraper:
	cd $@ && $(MAKE)

module_wraper:
	cd $@ && $(MAKE)

.PHONY: default counter_wraper module_wraper
