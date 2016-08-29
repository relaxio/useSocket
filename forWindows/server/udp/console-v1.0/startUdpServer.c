/*
**  版本：v1.0 2016.8.29 upload by Xiong
**  功能：接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，
**        采用UDP协议接收数据
**  环境：Visual Studio 2015 compiler on Windows 7 sp1
**  用法：startUdpServer.exe server_address port
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
    SOCKET sersocket;
    SOCKADDR_IN serAddr;
    int serAddrsize = sizeof(serAddr);
    int nRet = 0;
    char buf[1024];

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
    printf("\nCreating UDP Socket...\n");
    if (INVALID_SOCKET == (sersocket = socket(AF_INET, 
    	SOCK_DGRAM, IPPROTO_UDP)))
    {
        printf("Creation of socket failed %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }
    printf("UDP Socket Created sucessfully.\n");

    //绑定套接字
    if (SOCKET_ERROR == bind(sersocket, 
    	(SOCKADDR *)&serAddr, sizeof(serAddr)))
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(sersocket);
        WSACleanup();
        return 0;
    }

    printf("Waiting to  receice data...\n");

    while (1)
    {
        memset(buf, 0, sizeof(buf));

        if (SOCKET_ERROR == (nRet = recvfrom(sersocket, buf, sizeof(buf), 
        	0, (SOCKADDR *)&serAddr, &serAddrsize)))
        {
            printf("ERROR: recvfrom failed with error %d\n", WSAGetLastError());
            closesocket(sersocket);
            WSACleanup();
            return 0;
        }

        //显示接收到的数据
        buf[nRet] = '\0';
        printf(buf);
        printf("\n");
        
        if (SOCKET_ERROR == (nRet = sendto(sersocket, buf, strlen(buf), 
        	0, (SOCKADDR *)&serAddr, sizeof(serAddr))))
        {
            printf("ERROR:sendto failed with error %d\n", WSAGetLastError());
            closesocket(sersocket);
            WSACleanup();
            return 0;
        }

        //若发现exit，则退出处理循环
        if (strncmp(buf, "exit", sizeof("exit")) == 0)
        {
            printf("exit the receiving loop\n");
            break;
        }
    }

    //关闭已连接套接字
    closesocket(sersocket);
    WSACleanup();
    return 0;
}