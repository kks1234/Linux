## 条件变量(cond)：

### 为什么需要条件变量？

当一个线程互斥的访问某个变量时，它可能发现在其他线程改变该变量状态之前，它什么也做不了！（此时就是死锁，一种僵死状态）
我们需要一种机制，当互斥量锁住以后，当前线程还是无法完成自己的操作，那么就应该释放互斥量，让其他线程继续工作。 
1. 可以用轮询机制，不停的查询你需要的条件； 
2. 用条件变量机制；

### 条件变量函数: 
####（1）初始化
```
函数初始化
int pthread_cond_init(pthread_cond_t *restrict cont,const pthread_condattr_t *restrict attr);
```
参数：跟互斥量相似 
cond: 要初始化的条件变量； 
attr: 条件变量属性，一般设为NULL； 
当然也可以用宏 : PTHREAD_CONT_INITALIZER 静态初始化
```
pthread_cond_t cond = PTHREAD_CONT_INITALIZER;
```
####（2）等待条件满足
```
//非阻塞，
int pthread_cond_timedwite(pthread_conda_t *restrict cond,pthread_mutex_t *restrict mutex,const struct timespec *restrict abstime);
//阻塞
int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
```
参数： 
cond: 要在这个条件变量上等待； 
mutex: 互斥量 
abstime: 指定等待时间，如果在规定时间没有通知，返回 ETIMEDOUT错误； 
pthread_cond_wait()做的三件事： 
1，释放锁—->2，等待锁—->3，收到条件信息，尝试获取锁

####（3)唤醒等待
```
//唤醒改条件变量上的所有线程
int pthread_cond_broadcast(pthread_cond_t *cond);
//至少唤醒一个等待的线程
int pthread_cond_signal(pthread_cond_t *cond);
```
####（4）销毁条件变量
```
int pthread_cond_destroy(pthread_cond_t *cond);
```
