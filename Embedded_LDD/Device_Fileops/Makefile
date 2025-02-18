
obj-m += file_ops.o

all:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
