include ../../Makefile.include

SRCs=$(SRC_DIR)/csr_manipulation_test.c
LIB_PATH=../
LIB=-lhpc_utils

tests: $(BIN_DIR)/hpc_utils_test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/hpc_utils_test: $(SRCs) $(BIN_DIR)
	$(CC) -L$(LIB_PATH) $(CFLAGS) $(STATIC) $(PTHREAD) $(SRCs) -o $@ $(LIB)

clean:
	rm -f $(BIN_DIR)/hpc_utils_test

.PHONY: tests

