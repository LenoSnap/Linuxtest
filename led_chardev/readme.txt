字符设备驱动：
            1.宏
内核模块：
            2.函数
1.注册设备号
2.初始化设字符备并添加到内核
3.实现的文件操作




               struct chrdev {
	                           struct cdev dev;   描述一个字符设备的结构体
定义设备结构体:                  成员变量.....
                             };

初始化填充设备结构体...


           1.宏  定义设备名 设备号 设备数量等

                 
                 module_init(chrdev_init)
            
            module_exit(chrdev_exit)                  register_chrdev_regio静态申请
                                               申请设备号：alloc_chrdev_region动态申请
                                                          __register_chrdev()还会帮我们cdev 的初始化以及cdev 的注册

                                                调用class_create(THIS_MODULE, "chrdev_name")  //多个设备使用
                                            
                     __init int chrdev_init()    cdev_init() 初始化设备
                                                要设置dev.owner = THIS_MODULE 该成员可以防止设备的方法正在被使用时，设备所在模块被卸载
                                                 cdev_add() 注册设备
		                                       device_create(led_chrdev_class, NULL, cur_dev, NULL,DEV_NAME "%d", i);//多个设备使用

            2.函数 


                                               device_destroy()多个设备使用
               __init int chrdev_exit()                          cdev_del(dev);删除设备
                                               unregister_chrdev_region(devno, DEV_CNT)删除设备号
                                               class_destroy(led_chrdev_class);删除设备类（多个设备使用）
        



               static struct file_operations chrdev_fops = {
	                 .owner = THIS_MODULE,
	                 .open = led_chrdev_open,
实现的文件操作 	      .release = chrdev_release,
	                 .write = chrdev_write,
                     };







                            static ssize_t chrdev_open(struct inode *inode, struct file *filp)
                            {
                                unsigned int val = 0;
                                //通过 container_of函数将 chrdev 结构变量中 dev 成员的地址传到这个结构体变量的首地址
                                struct led_chrdev *led_cdev =
                                    (struct led_chrdev *)container_of(inode->i_cdev, struct led_chrdev,
                                                    dev);

                                //把文件的私有数据 private_data 指向设备结构体 led_cdev
                                filp->private_data =                                     
                                    container_of(inode->i_cdev, struct led_chrdev, dev); 
                                //iounmap进行内核地址的映射
                                //初始化设备的值  把修改后的值重新写入到被映射后的虚拟地址当中，即往寄存器中写入了数据
                                return 0;
                            }

                            static ssize_t  chrdev_write(struct file *filp, const char __user * buf,
                                            size_t count, loff_t * ppos)
                            {
                                unsigned int val = 0;
                                
                                //用kstrtoul_from_user或者copy_from_user()将用户缓存拷贝进内核
                                //把文件的私有数据 private_data 指向设备结构体 cdev
详细实现文件操作                 filp->private_data =                                     
                                    container_of(inode->i_cdev, struct led_chrdev, dev); 
                                //往寄存器中写入数据
                                return 0;
                            }

                            static ssize_t  chrdev_write(struct file *filp, const char __user * buf,
                                            size_t count, loff_t * ppos)
                            {
                                unsigned int val = 0;
                                
                                //用kstrtoul_to_user或者copy_tom_user()将内核数据拷贝到用户空间
                                //把文件的私有数据 private_data 指向设备结构体 cdev
                                filp->private_data =                                     
                                    container_of(inode->i_cdev, struct chrdev, dev); 
                                //往寄存器中读出数据
                                return 0;
                            }


                            tatic int led_chrdev_release(struct inode *inode, struct file *filp)
                            {
                                //把文件的私有数据 private_data 指向设备结构体 cdev
                                struct led_chrdev *led_cdev =
                                    (struct led_chrdev *)container_of(inode->i_cdev, struct led_chrdev,
                                                    dev);  
                                iounmap(led_cdev->va_dr);

                                return 0;
                            }