/*
**  版本：v1.0 2016.8.29 upload by Xiong
**  功能：循环不断地向服务器发送一条由控制台输入的消息，并从服务器接收一条消息，
**        遇到exit结束连接，采用UDP数据报发送数据
**  环境：Visual Studio 2015 compiler on Windows 7 sp1
**  用法：sendAndRecvMessage.exe server_address port
**  注意：服务器ip与端口之间没有':'号
*/
#include <stdio.h>
#include <winsock2.h> 

#pragma comment(lib, "ws2_32.lib")
//#define PORT 8888
//#define ADDR "127.0.0.1"

int main(int argc, char *argv[])
{
    WSADATA wsock;
    SOCKET cliSocket;
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
    if ((cliSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
        == INVALID_SOCKET)
    {
        printf("Creation of socket failed %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }
    printf("UDP Socket Created sucessfully.\n");

    //收发处理
    printf("Trying to send messages.\n");
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        printf("\nMessage to be sent:");
        gets(buf);

        if ((nRet = sendto(cliSocket, buf, strlen(buf), 0,
            (SOCKADDR *)&serAddr, sizeof(serAddr))) == SOCKET_ERROR)
        {
            printf("ERROR:sendto failed with error%d\n", WSAGetLastError());
            closesocket(cliSocket);
            WSACleanup();
            return 0;
        }

        printf("The data was sent to %s successfully.\n", inet_ntoa(serAddr.sin_addr));

        if (strncmp(buf, "exit", sizeof("exit")) == 0)
        {
            break;
        }

        memset(buf, 0, sizeof(buf));
        if ((nRet = recvfrom(cliSocket, buf, sizeof(buf), 0,
            (SOCKADDR *)&serAddr, &serAddrsize)) == SOCKET_ERROR)
        {
            printf("ERROR:recvfrom failed with error %d\n", WSAGetLastError());
            closesocket(cliSocket);
            WSACleanup();
            return 0;
        }
        printf("The following data was received from %s successfully.\n",
            inet_ntoa(serAddr.sin_addr));
        printf(buf);
    }

    //关闭已连接套接字
    closesocket(cliSocket);
    WSACleanup();
    return 0;
}