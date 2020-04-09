include ../Makefile.include

default: module_wraper

module_wraper: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

.PHONY: default

