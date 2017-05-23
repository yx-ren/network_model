#ifndef __THREADS_TCP_SERVER_H__
#define __THREADS_TCP_SERVER_H__

#include <memory>
#include <string.h>
#include <list>
#include <queue>
#include <thread>
#include <server/TcpServer.h>
#if defined(WIN32) || defined(_WIN32)
#include <mutex>
#else
#include <condition_variable>
#endif

#if defined(WIN32) || defined (_WIN32)
#include <ws2tcpip.h>
#include <WinSock.h>
#include <process.h>
#include <mstcpip.h>
#else
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class ThreadsClientHandle
{
public:
    explicit ThreadsClientHandle(SocketContextPtr context)
        : mContext(context)
    {}

    virtual ~ThreadsClientHandle()
    {}

    virtual void doWork(void) = 0;

protected:
    virtual void handleReadEvent(void) = 0;

    virtual void handleWriteEvent(void) = 0;

    std::string dumpHex(const char* buf, int len);

protected:
    std::mutex mMutex;
    std::condition_variable mCond;
    SocketContextPtr mContext;

    std::queue<std::shared_ptr<std::string>> mMessageQueue;
};
typedef std::shared_ptr<ThreadsClientHandle> ThreadsClientHandlePtr;

class ThreadsTcpServer : public TcpServer
{
public:
    explicit ThreadsTcpServer(uint16_t port)
        : TcpServer(port)
    {}

    explicit ThreadsTcpServer(const std::string ip, uint16_t port)
        : TcpServer(ip, port)
    {}

    virtual ~ThreadsTcpServer()
    {}

protected:
    virtual void handleClient(SocketContextPtr context) = 0;
};
typedef std::shared_ptr<ThreadsTcpServer> ThreadsTcpServerPtr;

#endif
