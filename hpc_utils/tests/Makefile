include ../../Makefile.include

SRCs=$(SRC_DIR)/add_and_div.c

tests: $(BIN_DIR)/add_and_div

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/add_and_div: $(SRCs) $(BIN_DIR)
	$(CC) $(CFLAGS) $(STATIC) $(PTHREAD) $(SRCs) -o $@

clean:
	rm -f $(BIN_DIR)/add_and_div

.PHONY: tests clean

