CURRENTPATH:=$(shell pwd)
KERNELPATH:=/home/ww/smart/orangepi-build/kernel/orange-pi-3.4-sun8i

obj-m = ir-distance.o

bulid:modules

modules:
	$(MAKE) -C $(KERNELPATH) M=$(CURRENTPATH) modules
clean:
	$(MAKE) -C $(KERNELPATH) M=$(CURRENTPATH) clean