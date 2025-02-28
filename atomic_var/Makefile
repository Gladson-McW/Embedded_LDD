
obj-m +=atom_var.o 

KOBJ = /lib/modules/$(shell uname -r)/build

all:

	make -C $(KOBJ) M=$(shell pwd) modules

clean:

	make -C $(KOBJ) M=$(shell pwd) clean
