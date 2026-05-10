#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

class Server
{
public:
    void start();

private:
    bool initializeWinsock();
    bool createSocket();
    bool bindSocket();
    bool startListening();
    void acceptClient();
    void cleanup();

    SOCKET serverSocket;
    WSADATA wsaData;
};

#endif