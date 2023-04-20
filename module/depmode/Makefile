# Makefile 2.6
KERNEL_DIR=/home/amin/work/lichee/linux-3.10
#arm64位
ARCH=arm64
#编译器aarch64-linux-
CROSS_COMPILE=aarch64-linux-
export  ARCH  CROSS_COMPILE
# calculation模块是依赖于parametermodule模块的，所以这里一起编译
obj-m := parametermodule.o calculation.o 

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
.PHONE:clean copy 

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean	

copy:
	sudo  cp1 *.ko  /home/amin/Linuxmode/
	#cp *.ko /lib/module/4.15.0-20-generic/
