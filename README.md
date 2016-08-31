## 套接字编程
### 总览
+ 简单入门
  + [forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c][16082901]
  + [forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c][16082902]
  + [forWindows/server/tcp/console-v1.0/startTcpServer.c][16082903] [说明][16083101]
  + [forWindows/server/tcp/console-v1.1/selectServer.c][16083102] [说明][16083103]
  + [forWindows/server/udp/console-v1.0/startUdpServer.c][16082904]

## 说明
### 简单入门
`forWindows`
>* __环境__：Visual Studio 2015 compiler on Windows 7 sp1
* __版本__：v1.0 2016.8.29 upload by Xiong

`forWindows`[/client/__tcp__/console-v1.0/sendAndRecvMessage.c][16082901]  
`forWindows`[/client/__ucp__/console-v1.0/sendAndRecvMessage.c][16082902]
>* __功能__：循环不断地向服务器发送由控制台输入的数据，并从服务器接收数据，遇到`"exit"`结束连接，分别采用TCP、UDP协议发送数据。
* __用法__：`sendAndRecvMessage.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号

`forWindows`[/server/__tcp__/console-v1.0/startTcpServer.c][16082903] [说明][16083101]  
`forWindows`[/server/__udp__/console-v1.0/startUdpServer.c][16082904]  
`forWindows`[/server/__tcp__/console-v1.1/selectServer.c][16083102] [说明][16083103]
>* __功能__：接收来自客户端的消息，显示并回发给客户端，遇到`"exit"`结束连接，分别采用TCP、UDP协议接收数据，TCP对应接收TCP客户端发来的数据，UDP对应接收UDP客户端发来的数据。
* __用法__：`startTcpServer.exe server_address port`
`startUdpServer.exe server_address port`
`selectServer.exe server_address port`
* __注意__：服务器ip与端口之间没有':'号，'`0.0.0.0`'表示所有ip地址

[16082901]: forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c
[16082902]: forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c
[16082903]: forWindows/server/tcp/console-v1.0/startTcpServer.c
[16082904]: forWindows/server/udp/console-v1.0/startUdpServer.c

[16083101]: forWindows/server/tcp/console-v1.0/
[16083102]: forWindows/server/tcp/console-v1.1/selectServer.c
[16083103]: forWindows/server/tcp/console-v1.1/
