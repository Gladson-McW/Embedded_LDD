
obj-m += link_list.o 

KOBJ = /lib/modules/$(shell uname -r)/build

all:

	make -C $(KOBJ) M=$(shell pwd) modules

clean:

	make -C $(KOBJ) M=$(shell pwd) clean

