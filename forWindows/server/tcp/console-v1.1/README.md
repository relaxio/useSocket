## select 模型
>主要是通过select函数检查套接字是否可以进行I/O操作

### 已知
* 在阻塞模式的套接字进行I/O操作，WinSock调用会阻塞线程
* 在非阻塞模式的套接字进行I/O操作，WinSock调用大都返回`WSAEWOULDBLOCK`错误

### 函数原型
```c
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *excepfds, const struct timeval *timeout);
```
`nfds` 忽略，与Berkeley套接字兼容
`readfds` 指向一组可读性检查的套接字
`writefds` 指向一组可写性检查的套接字
`excepfds` 指向一组错误检查的套接字
`timeout` select等待的最长时间，对于阻塞操作则为NULL

fd_set结构定义如下：
```c
typedef struct fd_set{
        u_int fd_count;
        SOCKET fd_array[FD_SETSIZE];
    }fd_set;
```
