include ../Makefile.include

default: performance_counters_util

performance_counters_util: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

clean:
	rm -f performance_counters_util

.PHONY: default performance_counters_util

