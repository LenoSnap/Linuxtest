# Makefile 2.6
KERNEL_DIR=/home/amin/work/lichee/linux-3.10
#arm64位
ARCH=arm64
#编译器aarch64-linux-
CROSS_COMPILE=aarch64-linux-
export  ARCH  CROSS_COMPILE
# hello是模块名，也是对应的c文件名。
obj-m := hellomode.o
md  += hellomode.ko
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
.PHONE:clean copy insmod rmmod

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean	

copy:
	sudo  cp  *.ko  /home/amin/Linuxmode/
insmod:
	sudo  insmod $(md)
rmmod:
	sudo  rmmod $(md)