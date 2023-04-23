# Makefile 2.6
KERNEL_DIR=/home/amin/work/lichee/linux-3.10
#arm64位
ARCH=arm64
#编译器aarch64-linux-
CROSS_COMPILE=aarch64-linux-
export  ARCH  CROSS_COMPILE
# calculation模块是依赖于parametermodule模块的，所以这里一起编译
# CURDIR 是 make 的内嵌变量，自动设置为当前目录。
export  ARCH  CROSS_COMPILE

obj-m := xdev.o xbus.o xdrv.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
modules clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean	
