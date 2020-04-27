#CC=riscv64-unknown-linux-gnu-gcc
#CFLAGS += -pthread -static
#SRCS =./src
#OUTPUT =../bin

SUBPROJECTS = counter_wraper module_wraper csr_manipulation_test performance_counters_util

all: $(SUBPROJECTS)

$(SUBPROJECTS):
	$(MAKE) -C $@

clean:
	rm -rf bin/*
	for DIR in $(SUBPROJECTS); do \
		$(MAKE) -C $$DIR clean; \
	done

.PHONY: all $(SUBPROJECTS)
