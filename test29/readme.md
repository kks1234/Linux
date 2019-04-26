## pint pthread_once (pthread_once_t *__once_control,void (*__init_routine) (void)) __nonnull ((1, 2))



###适用范围
这种情况一般用于某个多线程调用的模块使用前的初始化，但是无法判定哪个线程先运行，从而不知道把初始化代码放在哪个线程合适的问题。 
当然，我们一般的做法是把初始化函数放在main里，创建线程之前来完成，但是如果我们的程序最终不是做成可执行程序，而是编译成库的形式，那么main函数这种方式就没法做到了。

###基本原理
Linux Threads 使用互斥锁和条件变量保证由pthread_once()指定的函数执行且仅执行一次，而once_control则表征是否执行过。如果 once_control的初值不是PTHREAD_ONCE_INIT（Linux Threads定义为0），pthread_once()的行为就会不正常。在Linux Threads中，实际”一次性函数”的执行状态有三种：NEVER（0）、IN_PROGRESS（1）、DONE（2），如果 once初值设为1，则由于所有pthread_once()都必须等待其中一个激发”已执行一次”信号，因此所有pthread_once()都会陷入永久的等待中；如果设为2，则表示该函数已执行过一次，从而所有pthread_once()都会立即返回0。

