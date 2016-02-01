
ifeq ($(ODP),)
ODP=/home/simon/src_packages/odp_inst
endif

GCC=gcc
CFLAGS=-L$(ODP)/lib -I$(ODP)/include -I./include
LFLAGS=-lodp -lodphelper

lwstack += src/lwstack/iif.o
lwstack += src/lwstack/portif.o
lwstack += src/lwstack/tfif.o
lwstack += src/lwstack/pkt_info.o


test:
	echo $(CFLAGS)

src/prog/%.o: src/prog/%.c
	$(GCC) -c $(CFLAGS) $< -o $@

src/ustack/%.o: src/ustack/%.c
	$(GCC) -c $(CFLAGS) $< -o $@

ustack += src/ustack/netinput.o
ustack += src/ustack/arp.o

src/lwstack/%.o: src/lwstack/%.c
	$(GCC) -c $(CFLAGS) $< -o $@

run:
	echo \#!/bin/sh > run
	echo LD_LIBRARY_PATH=$(ODP)/lib $$\* >> run
	chmod +x run

build: $(lwstack)
	rm $(lwstack)

odp_prog: src/prog/odp_prog.o
	$(GCC) $(CFLAGS) $< $(LFLAGS) -o $@

host += src/prog/host.o
host += $(ustack)
host: $(host)
	$(GCC) $(CFLAGS) $(host) $(LFLAGS) -o $@

