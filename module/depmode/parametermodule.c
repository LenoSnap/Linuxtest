#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
/******************
module_param（name,type,perm）根据不同的应用场合结合内核模块传递不同的参数，提高内核灵活性
name ： 参数名
type： 参数类型 注意下面char和char*参数类型用法
perm： 读写权限  注意：不可设置执行权限，这个参数会在、/sys/module/模块名/paremeters 目录下生成该参数对应得文件名，但如果是0的话就不会生成该文件

用法就是在安装模块的时候加上如 ：sudo insmod parametermodule.ko itype=12 btype=1 ctype=a stype=abc 
******************/
static int itype=0;
module_param(itype,int,0);

static bool btype=0;
module_param(btype,bool,0700);

static char ctype=0;
module_param(ctype,byte,0);//参数是char 但类型写的是byte

static char  *stype=0;
module_param(stype,charp,0644);//参数是char *但类型写的是charp

static int __init param_init(void)
{
   printk(KERN_ALERT "param init!\n");
   printk(KERN_ALERT "itype=%d\n",itype);
   printk(KERN_ALERT "btype=%d\n",btype);
   printk(KERN_ALERT "ctype=%d\n",ctype);
   printk(KERN_ALERT "stype=%s\n",stype);
   return 0;
}

static void __exit param_exit(void)
{
   printk(KERN_ALERT "module exit!\n");
}
/******************
EXPORT_SYMBOL(sym) 符号共享 内核模块可用共享导出的符号表（变量共享或者函数共享）让其他模块使用导出的变量或者函数
sym：变量名或者函数名

在安装其他依赖于这个模块导出的共享符号时，建议先使用cat  /proc/kallsyms |grep 符号   查看符号表有没有如：cat  /proc/kallsyms |grep my_sub
******************/
EXPORT_SYMBOL(itype);

int my_add(int a, int b)
{
   return a+b;
}

EXPORT_SYMBOL(my_add);

int my_sub(int a, int b)
{
   return a-b;
}

EXPORT_SYMBOL(my_sub);

module_init(param_init);
module_exit(param_exit);

MODULE_LICENSE("GPL2");
MODULE_AUTHOR("embedfire ");
MODULE_DESCRIPTION("module_param");
MODULE_ALIAS("module_param");
