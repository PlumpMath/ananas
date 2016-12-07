#include <cassert>

#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "Socket.h"

namespace ananas
{

const int kInvalid = -1;

const int kTimeout =  0;
const int kError = -1;
const int kEof = -2;

int CreateUDPSocket()
{
    return ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

int CreateTCPSocket()
{
    return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void CloseSocket(int& sock)
{
    if (sock != kInvalid)
    {
        ::shutdown(sock, SHUT_RDWR);
        ::close(sock);
        sock = kInvalid;
    }
}

void SetNonBlock(int sock, bool nonblock)
{
    int flag = ::fcntl(sock, F_GETFL, 0); 
    assert(flag >= 0 && "Non Block failed");

    if (nonblock)
        flag = ::fcntl(sock, F_SETFL, flag | O_NONBLOCK);
    else
        flag = ::fcntl(sock, F_SETFL, flag & ~O_NONBLOCK);
}

void SetNodelay(int sock)
{
    int nodelay = 1;
    ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(int));
}

void SetSndBuf(int sock, socklen_t winsize)
{
    ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&winsize, sizeof(winsize));
}

void SetRcvBuf(int sock, socklen_t winsize)
{
    ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&winsize, sizeof(winsize));
}

void SetReuseAddr(int sock)
{
    int reuse = 1;
    ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
}

bool GetLocalAddr(int sock, SocketAddr& addr)
{
    sockaddr_in localAddr;
    socklen_t   len = sizeof(localAddr);

    if (0 == ::getsockname(sock, (struct sockaddr*)&localAddr, &len))
    {
        addr.Init(localAddr);
    }
    else
    {
        return  false;
    }

    return  true;
}

bool GetPeerAddr(int sock, SocketAddr& addr)
{
    sockaddr_in  remoteAddr;
    socklen_t    len = sizeof(remoteAddr);
    if (0 == ::getpeername(sock, (struct sockaddr*)&remoteAddr, &len))
    {
        addr.Init(remoteAddr);
    }
    else
    {
        return  false;
    }

    return  true;
}

} // end namespace ananas
