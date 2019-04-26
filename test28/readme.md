## 线程创建


### -lpthread和-pthread的区别
编译程序包括 预编译， 编译，汇编，链接，包含头文件了，仅能说明有了线程函数的声明， 但是还没有实现， 加上-lpthread是在链接阶段，链接这个库。
<stdio.h>等都是静态库，不需要做额外的表示，连接时会直接链接进代码里。pthread是动态库，需要用-lpthread，所有的动态库都需要用-lxxx来引用
用gcc编译使用了POSIX thread的程序时通常需要加额外的选项，以便使用thread-safe的库及头文件，一些老的书里说直接增加链接选项 -lpthread 就可以了

而gcc手册里则指出应该在编译和链接时都增加 -pthread 选项

 

编译选项中指定 -pthread 会附加一个宏定义 -D_REENTRANT，该宏会导致 libc 头文件选择那些thread-safe的实现；链接选项中指定 -pthread 则同 -lpthread 一样，只表示链接 POSIX thread 库。由于 libc 用于适应 thread-safe 的宏定义可能变化，因此在编译和链接时都使用 -pthread 选项而不是传统的 -lpthread 能够保持向后兼容，并提高命令行的一致性。

 

目前gcc 4.5.2中已经没有了关于 -lpthread的介绍了。所以以后的多线程编译应该用-pthread，而不是-lpthread。
