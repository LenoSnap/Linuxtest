模块手动加载
加载时必须先加载相关依赖模块
卸载时，顺序相反（只有被依赖的模块先卸载才能卸载该模块）

系统自动加载模块   所有的内核模块统一放到/lib/module/内核版本目录下
uname -r 查看内核版本
可以在makefile 里添加cp *.ko/lib/module/内核版本/

然后建立模块依赖关系：

sudo depmod -a

然后通过 cat /lib/module/内核版本/modules.dep |grep 模块名.ko            查看依赖关系

然后用sudo modeprode 模块名来加载模块和其依赖模块 但是一般默认加载的时候不传参


卸载的时候同样可用通过   sudo modeprode -r 模块名 来卸载