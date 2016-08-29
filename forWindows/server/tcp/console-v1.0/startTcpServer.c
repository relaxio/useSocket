/*
**  版本：v1.0 2016.8.29 upload by Xiong
**  功能：接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，
**        采用TCP连接接收数据
**  环境：Visual Studio 2015 compiler on Windows 7 sp1
**  用法：startTcpServer.exe server_address port
**  注意：服务器ip与端口之间没有':'号，'0.0.0.0'表示所有ip地址
*/
#include <stdio.h>
#include <winsock2.h> 

#pragma comment(lib, "ws2_32.lib")
//#define PORT 8888
//#define ADDR "127.0.0.1"

int main(int argc, char *argv[])
{
    WSADATA wsock;
    SOCKET listensocket, newconnection;
    SOCKADDR_IN serAddr, cliAddr;
    int cliAddrLen = sizeof(cliAddr);
    int nRet = 0;
    char buf[100];

    if (argc < 2)        //参数不够启动失败
    {
        printf("input %s server_address port (no :)\n", argv[0]);
        return -1;
    }

    if (argc >= 3)
    {
        //设置SOCKADDR_IN 地址结构
        serAddr.sin_family = AF_INET;        
        serAddr.sin_addr.s_addr = inet_addr(argv[1]);
        serAddr.sin_port = htons(atoi(argv[2]));

        if (INADDR_NONE == serAddr.sin_addr.s_addr)
        {
            printf("[ECHO] invalid address\n");
            return -1;
        }
    }

    //初始化WinSock 2.2
    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
    {
        fprintf(stderr, "WSAStartup() failed %d\n", WSAGetLastError());
        exit(0);
    }
    printf("Initialised sucessfully.\n");

    //创建侦听socket
    printf("\nCreating TCP Socket...\n");
    if ((listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
        == INVALID_SOCKET)
    {
        printf("Creation of socket failed %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }
    printf("TCP Socket Created sucessfully.\n");

    //绑定套接字
    if (bind(listensocket, (SOCKADDR *)&serAddr,
        sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(listensocket);
        WSACleanup();
        return 0;
    }

    //侦听连接
    if (listen(listensocket, 5) == SOCKET_ERROR)
    {
        printf("listen failed with error %d\n", WSAGetLastError());
        closesocket(listensocket);
        WSACleanup();
        return 0;
    }

    //接受连接
    if ((newconnection = accept(listensocket, (SOCKADDR *)&cliAddr,
        &cliAddrLen)) == INVALID_SOCKET)
    {
        printf("accept failed with error %d\n", WSAGetLastError());
        closesocket(listensocket);
        WSACleanup();
        return 0;
    }

    printf("successfully got a connection from %s:%d.\n",
        inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

    //此时可以继续侦听新的连接，或者停止侦听进行数据收发
    //对于单线程程序，应该停止侦听开始接收数据
    closesocket(listensocket);

    printf("Waiting to  receice data...\n");

    while (1)
    {
        memset(buf, 0, sizeof(buf));

        if ((nRet = recv(newconnection, buf, sizeof(buf), 0))
            == SOCKET_ERROR)
        {
            printf("recv failed with error %d\n", WSAGetLastError());
            closesocket(newconnection);
            WSACleanup();
            return 0;
        }

        //显示接收到的数据
        buf[nRet] = 0;
        printf(buf);
        printf("\n");

        //若发现exit，则退出处理循环
        if (strncmp(buf, "exit", sizeof("exit")) == 0)
        {
            printf("exit the receiving loop\n");
            break;
        }
        if ((nRet = send(newconnection, buf, strlen(buf), 0))
            == SOCKET_ERROR)
        {
            printf("send failed with error %d\n", WSAGetLastError());
        }
    }

    //关闭已连接套接字
    closesocket(newconnection);
    WSACleanup();
    return 0;
}