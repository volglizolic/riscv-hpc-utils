include ../Makefile.include

default: counter_wraper

counter_wraper: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

.PHONY: default

