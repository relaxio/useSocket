## 基本流程说明
>接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，采用TCP连接接收数据。

* Step1. 对WinSock库进行初始化，调用`WSAStartup`函数;
```c
int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
```
`wVersionRequested` Windows Sockets最高版本，高字节副版本号，低字节主版本号  
`lpWSAData` 指向WSADATA数据结构，用于接收Windows Sockets实现细节

* Step2. 创建用于监听的套接字，调用`socket` 或者 `WSASocket`

* Step3. 绑定套接字，调用`bind`

* Step4. 监听套接字，调用`listen`

* Step5. 有连接到达时，接收连接请求（客户端通过`connect`调用），调用`accept` 或 `WSAAccept`，创建临时连接套接字

* Step6. 在连接套接字上进行数据收发操作
接收数据 `recv` 或者 `WSARecv`  
发送数据 `send` 或者 `WSASend`
