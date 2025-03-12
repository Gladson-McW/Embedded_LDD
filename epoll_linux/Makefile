obj-m += epoll_linux.o

KOBJ = /lib/modules/$(shell uname -r)/build

all:
	gcc -o user user_space.c
	make -C $(KOBJ) M=$(shell pwd) modules

clean:
	make -C $(KOBJ) M=$(shell pwd) clean