#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

#include "command_handler.h"
#include "database.h"


class Server
{
public:
    Server();
    void start();

private:
    Database database;
    CommandHandler commandHandler;

    bool initializeWinsock();
    bool createSocket();
    bool bindSocket();
    bool startListening();
    void handleClient(SOCKET clientSocket);
    void cleanup();

    SOCKET serverSocket;
    WSADATA wsaData;
};

#endif