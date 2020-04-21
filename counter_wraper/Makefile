include ../Makefile.include

default: counter_wraper

counter_wraper: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

clean:
	rm -f counter_wraper

.PHONY: default counter_wraper

