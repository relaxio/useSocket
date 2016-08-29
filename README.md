## 套接字编程
### 总览
+ 简单入门
  + [forWindows/client/tcp/console-v1.0/`sendAndRecvMessage.c`][16082901]
  + [`forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c`][16082902]
  + [`forWindows/server/tcp/console-v1.0/startTcpServer.c`][16082903]
  + [`forWindows/server/udp/console-v1.0/startUdpServer.c`][16082904]

## 说明
### 简单入门
`forWindows`
* 环境：Visual Studio 2015 compiler on Windows 7 sp1  

`forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c`
[view code][16082901]
>* __版本__：v1.0 2016.8.29 upload by Xiong
* __功能__：循环不断地向服务器发送一条由控制台输入的消息，并从服务器接收一条消息，遇到"exit"结束连接，采用TCP连接发送数据
* __用法__：`sendAndRecvMessage.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号

[`forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c`][16082902]
>* __版本__：v1.0 2016.8.29 upload by Xiong
* __功能__：循环不断地向服务器发送一条由控制台输入的消息，并从服务器接收一条消息，遇到"exit"结束连接，采用UDP数据报发送数据
* __用法__：`sendAndRecvMessage.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号

[`forWindows/server/tcp/console-v1.0/startTcpServer.c`][16082903]
>* __版本__：v1.0 2016.8.29 upload by Xiong
* __功能__：接收来自客户端的消息，显示并回发给客户端，遇到"exit"结束连接，采用TCP连接接收数据
* __用法__：`startTcpServer.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号

[`forWindows/server/udp/console-v1.0/startUdpServer.c`][16082904]
>* __版本__：v1.0 2016.8.29 upload by Xiong
* __功能__：接收来自客户端的消息，显示并回发给客户端，遇到"exit"结束连接，采用UDP协议接收数据
* __用法__：`startUdpServer.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号


[16082901]: forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c
[16082902]: forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c
[16082903]: forWindows/server/tcp/console-v1.0/startTcpServer.c
[16082904]: forWindows/server/udp/console-v1.0/startUdpServer.c


