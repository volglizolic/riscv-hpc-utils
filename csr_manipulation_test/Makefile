include ../Makefile.include

default: csr_manipulation_test

csr_manipulation_test: $(SRCS)/*
	$(CC) $(CFLAGS) $(SRCS)/* -o $@
	cp $@ $(OUTPUT)/$@

clean:
	rm -f csr_manipulation_test

.PHONY: default csr_manipulation_test

