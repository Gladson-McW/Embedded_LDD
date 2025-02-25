obj-m += timer_sim.o

KDIR = /lib/modules/$(shell uname -r)/build

all:

	make -C $(KDIR) M=$(shell pwd) modules

clear:

	make -C $(KDIR) M=$(shell pwd) clean