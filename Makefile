obj-m += chrdev.o

PWD := $(CURDIR)

all:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: all
	sudo insmod chrdev.ko
	sudo dmesg -c

unload:
	sudo rmmod chrdev.ko
	sudo dmesg -c
