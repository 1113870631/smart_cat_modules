KERNEL_DIR =/home/ww/workspace/orangepi/orangepi-build/kernel/orange-pi-3.4-sun8i
CURRENT_DIR =$(shell pwd)

obj-m = SR_04.o

bulid:modules

modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_DIR) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_DIR) clean   