模块手动加载
加载时必须先加载相关依赖模块
卸载时，顺序相反（只有被依赖的模块先卸载才能卸载该模块）

modinfo 模块名可以查看模块的相关信息

系统自动加载模块   所有的内核模块统一放到/lib/module/内核版本目录下
uname -r 查看内核版本
可以在makefile 里添加cp *.ko /lib/module/内核版本/
可用cp *.ko /lib/module/`uname -r`/  一步到位
然后建立模块依赖关系：(生成modules.dep和map文件)

sudo depmod -a
最后在/etc/modules 加上我们自己的模块，注意在该配置文件中，模块不写成.ko 形式代表该模块
与内核紧耦合，有些是系统必须要跟内核紧耦合，比如 mm 子系统，一般写成.ko 形式比较好，如
果出现错误不会导致内核出现 panic 错误，如果集成到内核，出错了就会出现 panic:
cat /etc/modprbe.d/your.conf 模块名

通过 cat /lib/module/内核版本/modules.dep |grep 模块名.ko            查看依赖关系

然后用sudo modeprode 模块名来加载模块和其依赖模块 但是一般默认加载的时候不传参


卸载的时候同样可用通过   sudo modeprode -r 模块名 来卸载