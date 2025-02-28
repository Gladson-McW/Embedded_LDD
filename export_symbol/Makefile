obj-m += exp_sym1.o

obj-m += exp_sym2.o


KDIR = /lib/modules/$(shell uname -r)/build

all:

	make -C $(KDIR) M=$(shell pwd) modules

clean:

	make -C $(KDIR) M=$(shell pwd) clean

	