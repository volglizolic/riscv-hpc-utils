SUBPROJECTS = hpc_utils_lib hpc_utils

default: $(SUBPROJECTS)

all:
	@for DIR in $(SUBPROJECTS); do \
    	$(MAKE) -C $$DIR all; \
    done

$(SUBPROJECTS):
	$(MAKE) -C $@

tests:
	@for DIR in $(SUBPROJECTS); do \
    	$(MAKE) -C $$DIR tests; \
    done

clean:
	@for DIR in $(SUBPROJECTS); do \
		$(MAKE) -C $$DIR clean; \
	done

.PHONY: default all $(SUBPROJECTS)
