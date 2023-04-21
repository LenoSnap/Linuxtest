#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define DEV_NAME            "led_chrdev"   //设备名
#define DEV_CNT                 (3) //这里申请3个设备号，因为这个RGB一共是三个灯

static dev_t devno;
struct class *led_chrdev_class;

struct led_chrdev {
	struct cdev dev;   描述一个字符设备的结构体

	unsigned int __iomem *va_dr;          //数据寄存器虚拟地址指针
	unsigned int __iomem *va_gdir;        //输入输出方向寄存器虚拟地址指针
	unsigned int __iomem *va_iomuxc_mux;   //端口复用寄存器虚拟地址指针
	unsigned int __iomem *va_ccm_ccgrx;    //时钟寄存器虚拟地址指针
	unsigned int __iomem *va_iomux_pad;    //电气属性寄存器虚拟地址指针

	unsigned long pa_dr;              //装载数据寄存器（物理地址）的变量
	unsigned long pa_gdir;            //装载输出方向寄存器（物理地址）的变量
	unsigned long pa_iomuxc_mux;     // 装载端口复用寄存器（物理地址）的变量
	unsigned long pa_ccm_ccgrx;      // 装载时钟寄存器（物理地址）的变量
	unsigned long pa_iomux_pad;      // 装载电气属性寄存器（物理地址）的变量

	unsigned int led_pin;         //  LED 的引脚
	unsigned int clock_offset;    //  时钟偏移地址（相对于 CCM_CCGRx）
};

/*
file_operations 中 open 函数的实现,主要是通过内存映射的方式将
*/
static int led_chrdev_open(struct inode *inode, struct file *filp)
{
	unsigned int val = 0;
	struct led_chrdev *led_cdev =
	    (struct led_chrdev *)container_of(inode->i_cdev, struct led_chrdev,
					      dev);
	filp->private_data =
	    container_of(inode->i_cdev, struct led_chrdev, dev);

	printk("open\n");

	led_cdev->va_dr = ioremap(led_cdev->pa_dr, 4);
	led_cdev->va_gdir = ioremap(led_cdev->pa_gdir, 4);
	led_cdev->va_iomuxc_mux = ioremap(led_cdev->pa_iomuxc_mux, 4);
	led_cdev->va_ccm_ccgrx = ioremap(led_cdev->pa_ccm_ccgrx, 4);
	led_cdev->va_iomux_pad = ioremap(led_cdev->pa_iomux_pad, 4);

	val = ioread32(led_cdev->va_ccm_ccgrx);
	val &= ~(3 << led_cdev->clock_offset);
	val |= (3 << led_cdev->clock_offset);
	iowrite32(val, led_cdev->va_ccm_ccgrx);

	iowrite32(5, led_cdev->va_iomuxc_mux);

	iowrite32(0x1F838, led_cdev->va_iomux_pad);

	val = ioread32(led_cdev->va_gdir);
	val &= ~(1 << led_cdev->led_pin);
	val |= (1 << led_cdev->led_pin);
	iowrite32(val, led_cdev->va_gdir);

	val = ioread32(led_cdev->va_dr);
	val |= (0x01 << led_cdev->led_pin);
	iowrite32(val, led_cdev->va_dr);

	return 0;
}

static int led_chrdev_release(struct inode *inode, struct file *filp)
{
	struct led_chrdev *led_cdev =
	    (struct led_chrdev *)container_of(inode->i_cdev, struct led_chrdev,
					      dev);
	iounmap(led_cdev->va_dr);
	iounmap(led_cdev->va_gdir);
	iounmap(led_cdev->va_iomuxc_mux);
	iounmap(led_cdev->va_ccm_ccgrx);
	iounmap(led_cdev->va_iomux_pad);
	return 0;
}

static ssize_t led_chrdev_write(struct file *filp, const char __user * buf,
				size_t count, loff_t * ppos)
{
	unsigned long val = 0;
	unsigned long ret = 0;

	int tmp = count;

	kstrtoul_from_user(buf, tmp, 10, &ret);
	struct led_chrdev *led_cdev = (struct led_chrdev *)filp->private_data;

	val = ioread32(led_cdev->va_dr);
	if (ret == 0)
		val &= ~(0x01 << led_cdev->led_pin);
	else
		val |= (0x01 << led_cdev->led_pin);

	iowrite32(val, led_cdev->va_dr);
	*ppos += tmp;
	return tmp;
}

static struct file_operations led_chrdev_fops = {
	.owner = THIS_MODULE,
	.open = led_chrdev_open,
	.release = led_chrdev_release,
	.write = led_chrdev_write,
};

static struct led_chrdev led_cdev[DEV_CNT] = {
	{.pa_dr = 0x0209C000,.pa_gdir = 0x0209C004,.pa_iomuxc_mux =
	 0x20E006C,.pa_ccm_ccgrx = 0x20C406C,.pa_iomux_pad =
	 0x20E02F8,.led_pin = 4,.clock_offset = 26},                    //初始化红灯结构体成员变量
	{.pa_dr = 0x20A8000,.pa_gdir = 0x20A8004,.pa_iomuxc_mux =
	 0x20E01E0,.pa_ccm_ccgrx = 0x20C4074,.pa_iomux_pad =
	 0x20E046C,.led_pin = 20,.clock_offset = 12},                   //初始化绿灯结构体成员变量
	{.pa_dr = 0x20A8000,.pa_gdir = 0x20A8004,.pa_iomuxc_mux =
	 0x20E01DC,.pa_ccm_ccgrx = 0x20C4074,.pa_iomux_pad =
	 0x20E0468,.led_pin = 19,.clock_offset = 12},                    //初始化蓝灯结构体成员变量
};

static __init int led_chrdev_init(void)   //内核 RGB 模块的加载函数
{
	int i = 0;
	dev_t cur_dev;
	printk("led chrdev init\n");
/*
通过查看内核源码可知：
register_chrdev_region静态获取和  alloc_chrdev_region 动态获取的区别，
register_chrdev_region直接将Major 注册进入，而 alloc_chrdev_region从
Major = 0 开始，逐个查找设备号，直到找到一个闲置的设备号，并将其注册进去
还有一个register_chrdev 函数，只要一句就可以搞定前面函数所做之事
不只帮我们注册了设备号，还帮我们做了cdev 的初始化以及cdev 的注册
前两个都调用了__register_chrdev_region(unsigned int major, unsigned int baseminor,
			   int minorct, const char *name)
 而第三个调用了__register_chrdev(major, 0, 256, name, fops)，缺点就是一下就要
 注册256个次设备号，在设备号多的时候可能占用设备号

*/ 

	alloc_chrdev_region(&devno, 0, DEV_CNT, DEV_NAME);

    // 调用 class_create() 函数创建一个 RGB 灯的设备类,这里先填好模块
	led_chrdev_class = class_create(THIS_MODULE, "led_chrdev");    

    //分别给RGB关联文件，至此inode与我们定义的文件关联
	for (; i < DEV_CNT; i++) {
		cdev_init(&led_cdev[i].dev, &led_chrdev_fops);    
		led_cdev[i].dev.owner = THIS_MODULE;

		cur_dev = MKDEV(MAJOR(devno), MINOR(devno) + i);

		cdev_add(&led_cdev[i].dev, cur_dev, 1);

		device_create(led_chrdev_class, NULL, cur_dev, NULL,
			      DEV_NAME "%d", i);
	}

	return 0;
}

module_init(led_chrdev_init);


/*
    调用 device_destroy() 函数用于从 linux 内核系统设备驱动程序模型中移除一个设备，并删
    除/sys/devices/virtual 目录下对应的设备目录及/dev/目录下对应的设备文件；
    调用 cdev_del() 函数来释放散列表中的对象以及 cdev 结构本身；
    释放被占用的设备号以及删除设备类
*/
static __exit void led_chrdev_exit(void)
{
	int i;
	dev_t cur_dev;
	printk("led chrdev exit\n");

	for (i = 0; i < DEV_CNT; i++) {
		cur_dev = MKDEV(MAJOR(devno), MINOR(devno) + i);

		device_destroy(led_chrdev_class, cur_dev);

		cdev_del(&led_cdev[i].dev);

	}
	unregister_chrdev_region(devno, DEV_CNT);
	class_destroy(led_chrdev_class);

}

module_exit(led_chrdev_exit);

MODULE_AUTHOR("amin");
MODULE_LICENSE("GPL");
