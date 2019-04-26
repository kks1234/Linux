## 线程私有数据




在维护每个线程的私有数据的时候，我们可能会想到分配一个保存线程数据的数组，用线程的ID作为数组的索引来实现访问，但是有一个问题是系统生成的线程ID不能保证是一个小而连续的整数，并且用数组实现的时候由于其他线程也可以访问其数组中的数据，这样会引起数据混乱。这时候我们可以借助线程的私有数据来解决这个问题。

线程私有数据实现的主要思想是：在分配线程私有数据之前，创建与该数据相关联的健，这个键可以被进程中的所有线程使用，但每个线程把这个键与不同的线程私有数据地址进行关联，需要说明的是每个系统支持有限数量的线程特定数据元素（下面的例子以128个为限制）。那么这个键的实现原理是什么呢？

其实系统为每个进程维护了一个称之为Key结构的结构数组，如下图所示：
![avatar](https://github.com/kks1234/Linux/blob/master/test31/one.png)
（图1）

在上图中Key 结构的“标志”指示这个数据元素是否正在使用。在刚开始时所有的标志初始化为“不在使用”。当一个线程调用pthread_key_create创建一个新的线程特定数据元素时，系统会搜索Key结构数组，找出第一个“不在使用”的元素。并把该元素的索引（0~127)称为“键”。 返回给调用线程的正是这个索引。

除了进程范围内的Key结构数组之外，系统还在进程内维护了关于多个线程的多条信息。这些特定于线程的信息我们称之为Pthread结构。其中部分内容是我们称之为pkey数组的一个128个元素的指针数组。系统维护的关于每个线程的信息结构图如下：

![avatar](https://github.com/kks1234/Linux/blob/master/test31/two.png)

（图2）


在上图中，pkey数组所有元素都被初始化为空指针。这些128个指针是和进程内128个可能的键逐一关联的值。

那么当我们调用pthread_key_create函数时，系统会为我们做什么呢？
    　系统首先会返回给我们一个Key结构数组中第一个“未被使用”的键（即索引值），每个线程可以随后通过该键找到对应的位置，并且为这个位置存储一个值（指针）。 一般来说，这个指针通常是每个线程通过调用malloc来获得的。
知道了大概的私有数据实现的原理，那么在编程中如何使用线程的特定数据呢？

　　假设一个进程被启动，并且多个线程被创建。 其中一个线程调用pthread_key_create。系统在Key结构数组（图1）中找到第1个未使用的元素。并把它的索引（0~127）返回给调用者。我们假设找到的索引为1  （我们会使用pthread_once 函数确保pthread_key_create只被调用一次，这个在以后会讲到）。

　　之后线程调用pthread_getspecific获取本线程的pkey[1] 的值（图（2）中键1所值的指针）， 返回值是一个空值，线程那么调用malloc分配内存区并初始化此内存区。 之后线程调用pthread_setspecific把对应的所创建键的线程特定数据指针(pkey[1]) 设置为指向它刚刚分配的内存区。下图指出了此时的情形。

![avatar](https://github.com/kks1234/Linux/blob/master/test31/three.png)

（图3）


明白了怎样获取线程的特定数据值，那么如果线程终止时系统会执行什么操作呢？

我们知道，一个线程调用pthread_key_create创建某个特定的数据元素时，所指定的参数之一便是指向牧歌析构函数的指针。当一个线程终止时，系统将扫描该线程的pkey数组，为每个非空的pkey指针调用相应的析构函数。 相应的析构函数是存放在图1中的Key数组中的函数指针。这是一个线程终止时其线程特定数据的释放手段。

明白了线程私有数据的实现原理，我们就来看一下相应函数的用法：

```C
#include<pthread.h>

int phread_once(pthread_once_t *onceptr, vid(*init)(void));

in pthread_key_create(pthread_key_t *keyptr, void(* destructor)(void *value));
```

注意：pthread_once 使用onceptr 参数指向的变量中的值确保init参数所指的函数在进程范围内之被调用一次，onceptr必须是一个非本地变量（即全局变量或者静态变量），而且必须初始化为PTHREAD_ONCE_INIT。

这两个函数的典型用法如下：
```C
pthread_key_t r1_key;

pthread_once_t r1_once = PTHREAD_ONCE_INIT;

void destructor(void *ptr)

{

     free(ptr);

}

void excute_once(void)  // 确保键只被创建一次

{

    pthread_key_create(&r1_key, destructor);

}

int main()

{

   pthread_once(&r1_once, excute_once);

}

 ```

下面还有两个设置线程私有数据和获得线程私有数据的函数
```C
void *pthread_getspecific(pthread_key_t key);
```
```
int pthread_setspecific(pthread_key_t key, const void *value); -- 0 返回成功  返回其它值表示出错。
```
