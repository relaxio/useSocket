## 套接字编程示例
### 总览
+ 简单入门示例
  + [forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c](forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c)
  + [forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c](forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c)
  + [forWindows/server/tcp/console-v1.0/startTcpServer.c](forWindows/server/tcp/console-v1.0/startTcpServer.c)
  + [forWindows/server/udp/console-v1.0/startUdpServer.c](forWindows/server/udp/console-v1.0/startUdpServer.c)

## 说明
### 简单入门示例
`forWindows`
* 环境：Visual Studio 2015 compiler on Windows 7 sp1  

`forWindows/client/tcp/console-v1.0/sendAndRecvMessage.c`
>* 版本：v1.0 2016.8.29 upload by Xiong
* 功能：循环不断地向服务器发送一条由控制台输入的消息，并从服务器接收一条消息，遇到exit结束连接，采用TCP连接发送数据
* 用法：`sendAndRecvMessage.exe server_address port`
* 注意：服务器ip与端口之间没有':'号

`forWindows/client/ucp/console-v1.0/sendAndRecvMessage.c`
* 版本：v1.0 2016.8.29 upload by Xiong
* 功能：循环不断地向服务器发送一条由控制台输入的消息，并从服务器接收一条消息，遇到exit结束连接，采用UDP数据报发送数据
* 用法：`sendAndRecvMessage.exe server_address port`
* 注意：服务器ip与端口之间没有':'号

`forWindows/server/tcp/console-v1.0/startTcpServer.c`
* 版本：v1.0 2016.8.29 upload by Xiong
* 功能：接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，采用TCP连接接收数据
* 用法：`startTcpServer.exe server_address port`
* 注意：服务器ip与端口之间没有':'号

`forWindows/server/udp/console-v1.0/startUdpServer.c`
* 版本：v1.0 2016.8.29 upload by Xiong
* 功能：接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，采用UDP协议接收数据
* 用法：`startUdpServer.exe server_address port`
* 注意：服务器ip与端口之间没有':'号

