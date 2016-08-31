/*
**  版本：v1.1 2016.8.31 upload by Xiong
**  功能：接收来自客户端的消息，显示并回发给客户端，遇到exit结束连接，
**        采用TCP连接接收数据
**  环境：Visual Studio 2015 compiler on Windows 7 sp1
**  用法：selectServer.exe server_address port
**  注意：服务器ip与端口之间没有':'号，'0.0.0.0'表示所有ip地址
*/

#include <stdio.h>
#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib")

//#define PORT 8888
#define BUFFERSIZE 100
//#define ADDR "127.0.0.1"
#define ECHO 0xFE
#define REPLY 0xFF
//套接字环境
typedef struct _tagSOCKET_INFO {
    OVERLAPPED overlap;
    WSABUF wsaBuf;
    char buf[BUFFERSIZE];
    SOCKET soc;
    DWORD bytesend;
    DWORD bytesrecv;
}SOCKET_INFO, *LPSOCKET_INFO;

DWORD totalsocks = 0;
LPSOCKET_INFO socklist[FD_SETSIZE];

BOOL CreateSocketInfo(SOCKET s);
void FreeSocketInfo(DWORD index);

int main(int argc, char *argv[])
{
    WSADATA wsock;
    //侦听、连接套接字
    SOCKET listensocket, acceptsocket;
    //服务端地址
    SOCKADDR_IN serAddr;
    //select集合
    struct fd_set fdwrite, fdread;
    //缓冲区准备好的套接字计数
    DWORD index, sockok;
    //套接字工作模式
    ULONG sockmode;
    DWORD flags;
    //接收发送数据计数
    DWORD datasend, datarecv;
    //客户端地址
    SOCKADDR_IN cliAddr;
    int cliAddrLen;
    
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
        printf("WSAStartup() failed %d\n", WSAGetLastError());
        exit(0);
    }
    printf("Initialised sucessfully.\n");

    //创建侦听socket
    printf("\nCreating TCP Socket...\n");
    if ((listensocket = WSASocket(AF_INET, SOCK_STREAM, 0,NULL,0,
        WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
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
    printf("Listening on the %s:%d...\n",
        inet_ntoa(serAddr.sin_addr), ntohs(serAddr.sin_port));
    
    //更改侦听套接字工作模式至非阻塞模式+select
    sockmode = 1;
    if (SOCKET_ERROR == ioctlsocket(listensocket, FIONBIO, &sockmode))
    {
        printf("ioctlsocket() failed\n");
        return 0;
    }

    while (TRUE)
    {
        //初始化读写套接字集合
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        //将侦听套接字添加到集合中，以便检查连接请求
        FD_SET(listensocket, &fdread);

        //根据缓冲区当前的状态来为套接字设置可读性及可写性测试
        //例如当收到数据以后则要测试缓冲区的可写性
        //否则测试其可读性
        for (index = 0; index < totalsocks; index++)
        {
            if (socklist[index]->bytesrecv > socklist[index]->bytesend)
            {
                FD_SET(socklist[index]->soc, &fdwrite);
            }
            else
            {
                FD_SET(socklist[index]->soc, &fdread);
            }
        }
        //选择感兴趣的套接字集合
        if (SOCKET_ERROR == (sockok = select(0, &fdread, &fdwrite, NULL, NULL)))
        {
            printf("select failed: %d\n", WSAGetLastError());
            return -1;
        }
        //检查到来的连接请求
        if (FD_ISSET(listensocket, &fdread))
        {
            sockok--;
            //cliAddrLen 必须初始化为cliAddr的大小
            cliAddrLen = sizeof(cliAddr);
            if (INVALID_SOCKET != (acceptsocket = accept(listensocket,
                (SOCKADDR *)&cliAddr, &cliAddrLen)))
            {
                //接受连接并将新的套接字设置为非阻塞模式
                sockmode = 1;
                if (SOCKET_ERROR == (ioctlsocket(acceptsocket, FIONBIO, &sockmode)))
                {
                    printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
                    return 0;
                }
                //为新套接字创建工作环境
                if (FALSE == CreateSocketInfo(acceptsocket))
                {
                    return 0;
                }
                printf("successfully got a connection from %s:%d.\n",
                    inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
            }
            else
            {
                if (WSAEWOULDBLOCK != WSAGetLastError())
                {
                    printf("accept() failed with error %d\n", WSAGetLastError());
                    return 0;
                }
            }
        }
        //为所有的sockok套接字检查读写通知
        for (index = 0; sockok > 0 && index < totalsocks; index++)
        {
            LPSOCKET_INFO sockinfo = socklist[index];

            //缓冲区可读性测试
            if (FD_ISSET(sockinfo->soc, &fdread))
            {
                sockok--;
                sockinfo->wsaBuf.buf = sockinfo->buf;
                sockinfo->wsaBuf.len = BUFFERSIZE;
                flags = 0;
                if (SOCKET_ERROR == (WSARecv(sockinfo->soc,
                    &(sockinfo->wsaBuf), 1, &datarecv, &flags, NULL, NULL)))
                {
                    if (WSAEWOULDBLOCK != WSAGetLastError())
                    {
                        printf("Receive failed with error\n");
                        FreeSocketInfo(index);
                    }
                    continue;
                }
                else
                {
                    //cliAddrLen必须初始化为cliAddr的大小
                    cliAddrLen = sizeof(cliAddr);

                    //获取客户端地址信息
                    if (getpeername(sockinfo->soc, (SOCKADDR *)&cliAddr, &cliAddrLen))
                    {
                        printf("getpeername failed with error %d\n", WSAGetLastError());
                    }

                    printf("The following data comes from %s:%d\n",
                        inet_ntoa(cliAddr.sin_addr), htons(cliAddr.sin_port));
                    //接收的数据量
                    sockinfo->bytesrecv = datarecv;
                    //显示接收到的数据
                    printf("%s\n", sockinfo->wsaBuf.buf);
                    printf("Waiting to receive data...\n");
                    //若发现exit，则退出处理循环
                    if (0 == strncmp(sockinfo->wsaBuf.buf, "exit", sizeof("exit")))
                    {
                        printf("exit the receiving loop\n");
                        break;
                    }
                    //连接关闭，则接收到0字节
                    if (0 == datarecv)
                    {
                        FreeSocketInfo(index);
                        continue;
                    }
                }
            }
            //缓冲区可写性测试
            if (FD_ISSET(sockinfo->soc, &fdwrite))
            {
                sockok--;

                //继续发送剩余部分
                sockinfo->wsaBuf.buf = sockinfo->buf + sockinfo->bytesend;
                sockinfo->wsaBuf.len = sockinfo->bytesrecv - sockinfo->bytesend;
                if (SOCKET_ERROR == (WSASend(sockinfo->soc,
                    &(sockinfo->wsaBuf), 1, &datasend, 0, NULL, NULL)))
                {
                    if (WSAEWOULDBLOCK != WSAGetLastError())
                    {
                        printf("Send failed with error\n");
                        FreeSocketInfo(index);
                    }
                    continue;
                }
                else
                {
                    //增加已发送数据计数
                    sockinfo->bytesend += datasend;
                    //二者相等时说明接收数据已经发送完毕
                    if (sockinfo->bytesend == sockinfo->bytesrecv)
                    {
                        sockinfo->bytesend = 0;
                        sockinfo->bytesrecv = 0;
                        ZeroMemory(sockinfo->wsaBuf.buf, BUFFERSIZE);
                    }
                }
            }
        }
    }
    WSACleanup();
    return 0;
}

void FreeSocketInfo(DWORD index)
{
    LPSOCKET_INFO sockinfo = socklist[index];
    DWORD loop;
    closesocket(sockinfo->soc);

    printf("Closing socket...\n");

    GlobalFree(sockinfo);

    //从数组中移除
    for (loop = index; loop < totalsocks - 1; loop++)
    {
        socklist[loop] = socklist[loop + 1];
    }
    //递减已关闭连接套接字的计数
    totalsocks--;
}

BOOL CreateSocketInfo(SOCKET s)
{
    LPSOCKET_INFO sockinfo;

    printf("Accepted socket!\n");
    //从堆上分配内存
    if (NULL == (sockinfo = (LPSOCKET_INFO)GlobalAlloc(GPTR,
        sizeof(SOCKET_INFO))))
    {
        printf("GlobalAlloc() failed\n");
        return FALSE;
    }

    //套接字相关
    sockinfo->soc = s;
    sockinfo->bytesend = sockinfo->bytesrecv = 0;
    socklist[totalsocks] = sockinfo;
    //递增已接受连接套接字的计数
    totalsocks++;
    
    return TRUE;
}