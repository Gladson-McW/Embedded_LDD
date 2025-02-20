
obj-m += ioctl_driv.o

all:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:

	make -C /lib/modules/$(shelll uname -r)/build M=$(shell pwd) clean
