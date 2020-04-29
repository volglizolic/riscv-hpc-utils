include ../Makefile.include

SRCs= $(SRC_DIR)/hpc_utils.c $(SRC_DIR)/help.c
HEADERs= $(SRC_DIR)/help.h
LIB_PATH=../hpc_utils_lib/
LIB=-lhpc_utils

default: $(BIN_DIR)/hpc_utils

all: default tests

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/hpc_utils: $(SRCs) $(BIN_DIR)
	$(CC) -L$(LIB_PATH) $(CFLAGS) $(PTHREAD) $(STATIC) $(SRCs) -o $@ $(LIB)

tests:
	$(MAKE) -C tests

clean:
	rm -f $(BIN_DIR)/hpc_utils
	$(MAKE) -C tests clean

.PHONY: default clean tests

