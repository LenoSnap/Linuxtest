# Makefile 2.6
KERNEL_DIR=/home/amin/work/lichee/linux-3.10
#arm64位
ARCH=arm64
#编译器aarch64-linux-
CROSS_COMPILE=aarch64-linux-
export  ARCH  CROSS_COMPILE

obj-m := led_pdev.o led_pdrv.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
modules clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean	
