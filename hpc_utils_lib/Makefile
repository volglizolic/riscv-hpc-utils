include ../Makefile.include

#DESTDIR?=/usr/local/lib
OBJ_DIR=obj
SRCs=$(SRC_DIR)/hpc_utils_lib.c
HEADERs=$(SRC_DIR)/event_definition.h $(SRC_DIR)/fu540.h include/hpc_utils.h
OBJs=$(OBJ_DIR)/hpc_utils_lib.o

default: libhpc_utils.so libhpc_utils.a

all: libhpc_utils.so libhpc_utils.a tests

$(OBJ_DIR):
	@mkdir -p obj

$(OBJs): $(SRCs) $(OBJ_DIR) $(HEADERs)
	$(CC) -c $(CFLAGS) $(PTHREAD) -fpic $(SRCs) -o obj/hpc_utils_lib.o

libhpc_utils.so: $(OBJs)
	$(CC) -shared -o libhpc_utils.so $(OBJs)

libhpc_utils.a: $(OBJs)
	$(AR) rcs libhpc_utils.a $(OBJs)

#install: libhpc_utils.so libhpc_utils.a include/hpc_utils.h

tests:
	$(MAKE) -C tests

clean:
	rm -f OBJ_DIR/* libhpc_utils.so libhpc_utils.a
	$(MAKE) -C tests clean

.PHONY: default clean tests

