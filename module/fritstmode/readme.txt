Linux 内核模块的代码框架通常由下面几个部分组成：
• 模块加载函数 (必须)：当通过 insmod 或 modprobe 命令加载内核模块时，模块的加载函数
就会自动被内核执行，完成本模块相关的初始化工作。
• 模块卸载函数 (必须)：当执行 rmmod 命令卸载模块时，模块卸载函数就会自动被内核自动
执行，完成相关清理工作。
• 模块许可证声明 (必须)：许可证声明描述内核模块的许可权限，如果模块不声明，模块被
加载时，将会有内核被污染的警告。
• 模块参数：模块参数是模块被加载时，可以传值给模块中的参数。
• 模块导出符号：模块可以导出准备好的变量或函数作为符号，以便其他内核模块调用。
• 模块的其他相关信息：可以声明模块作者等信息


头文件包含了 <linux/init.h> 和 <linux/module.h>，这两个头文件是写内核模块必须要包含的。模
块初始化函数 hello_init 调用了 printk 函数，在内核模块运行的过程中，他不能依赖于 C 库函数，
因此用不了 printf 函数，需要使用单独的打印输出函数 printk。该函数的用法与 printf 函数类似。
完成模块初始化函数之后，还需要调用宏 module_init 来告诉内核，使用 hello_init 函数来进行初
始化。模块卸载函数也用 printk 函数打印字符串，并用宏 module_exit 在内核注册该模块的卸载
函数。最后，必须声明该模块使用遵循的许可证，这里我们设置为 GPL2 协议。

• #include <linux/module.h>：包含了内核加载 module_init()/卸载 module_exit() 函数和内核模
块信息相关函数的声明
• #include <linux/init.h>：包含一些内核模块相关节区的宏定义
• #include <linux/kernel.h>：包含内核提供的各种函数，如 printk

printk 函数
• printf：glibc 实现的打印函数，工作于用户空间
• printk：内核模块无法使用 glibc 库函数，内核自身实现的一个类 printf 函数，但是需要指定
打印等级。
– #define KERN_EMERG “<0>”通常是系统崩溃前的信息
– #define KERN_ALERT “<1>”需要立即处理的消息
– #define KERN_CRIT “<2>”严重情况
– #define KERN_ERR “<3>”错误情况
– #define KERN_WARNING “<4>”有问题的情况
– #define KERN_NOTICE “<5>”注意信息
– #define KERN_INFO “<6>”普通消息
– #define KERN_DEBUG “<7>”调试信息
查看当前系统 printk 打印等级：cat /proc/sys/kernel/printk，从左到右依次对应当前控
制台日志级别、默认消息日志级别、最小的控制台级别、默认控制台日志级别。
打印内核所有打印信息：dmesg，注意内核 log 缓冲区大小有限制，缓冲区数据可能被覆盖掉。

