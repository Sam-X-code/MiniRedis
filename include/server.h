#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <atomic>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>

#include "command_handler.h"
#include "database.h"


class Server
{
public:
    Server();
    void start();
    void shutdown();

private:
    Database database;
    CommandHandler commandHandler;

    bool initializeWinsock();
    bool createSocket();
    bool bindSocket();
    bool startListening();
    void handleClient(SOCKET clientSocket);
    void cleanup();
    void autoSave();
    std::atomic<bool> running{true};

    std::vector<std::thread> workers;
    std::queue<SOCKET> clientQueue;

    std::mutex queueMutex;
    std::condition_variable condition;
    void workerThread();

    SOCKET serverSocket;
    WSADATA wsaData;
};

#endif