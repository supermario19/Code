#include <SCU/IO/core/EventLoop.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <SCU/IO/util/Logger.h>
#include <SCU/IO/util/Timestamp.h>
#include <functional>
using namespace SCU::IO::core;
using namespace SCU::IO::util;

void readCallback(Timestamp receiveTime)
{
    LOG_INFO << "new connection coming in time:" << receiveTime.toFormatString();
}

int main()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(12345);

    bind(socketfd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address));

    listen(socketfd, 5);

    EventLoop loop;
    Channel   channel(&loop, socketfd);
    channel.setReadCallback(std::bind(readCallback, Timestamp::now()));
    channel.enableReading();
    
    loop.loop();

    return 0;
}