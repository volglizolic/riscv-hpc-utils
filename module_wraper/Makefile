include ../Makefile.include

default: module_wraper

module_wraper: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

clean:
	rm -f module_wraper

.PHONY: default module_wraper

