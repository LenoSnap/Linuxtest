#include <linux/init.h>
//#include <linux/kernel.h>
#include <linux/module.h>           //最基本的文件，支持动态添加和卸载模块。
#include <linux/miscdevice.h>        /*注册杂项设备头文件*/

static int __init hello_init(void)
{
    printk(KERN_EMERG "[ KERN_EMERG ]  Hello  linux\n");
    printk( "[ default ]  Hello  linux\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk("[ default ]   Hello  Module Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL2");
MODULE_AUTHOR("amin");
MODULE_DESCRIPTION("hello world module");
MODULE_ALIAS("test_module");