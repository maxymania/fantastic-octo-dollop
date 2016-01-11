
ifeq ($(DPDK),)
DPDK=/home/simon/src_packages/odp_inst
endif

GCC=gcc
CFLAGS=-L$(DPDK)/lib -I$(DPDK)/include
LFLAGS=-lodp -lodphelper

lwstack += src/lwstack/iif.o
lwstack += src/lwstack/portif.o


test:
	echo $(CFLAGS)

src/prog/%.o: src/prog/%.c
	$(GCC) -c $(CFLAGS) $< -o $@

src/lwstack/%.o: src/lwstack/%.c
	$(GCC) -c $(CFLAGS) $< -o $@

run:
	echo \#!/bin/sh > run
	echo LD_LIBRARY_PATH=$(DPDK)/lib $$\* >> run
	chmod +x run

build: $(lwstack)
	rm $(lwstack)

odp_prog: src/prog/odp_prog.o
	$(GCC) $(CFLAGS) $< $(LFLAGS) -o $@



