#include <iostream>
#include <thread>
#include <chrono>

#include <winsock2.h>
#include <windows.h>

#include "server.h"

Server* g_server = nullptr;
constexpr int BUFFER_SIZE = 1024;
constexpr const char* SNAPSHOT_FILE = "../dump.rdb";

Server::Server()
    : commandHandler(database){}

BOOL WINAPI ConsoleHandler(DWORD signal){
    switch(signal){
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_BREAK_EVENT:

            if(g_server){
                g_server->shutdown();
            }

            return TRUE;
    }

    return FALSE;
}

bool Server::initializeWinsock(){
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0){
        std::cout << "WSAStartup failed! Error: "<< WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Winsock initialized successfully!" << std::endl;
    return true;
}

bool Server::createSocket(){
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET){
        std::cout << "Socket creation failed! Error: "<< WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Socket created successfully!" << std::endl;
    return true;
}

bool Server::bindSocket(){
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6379);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)) == SOCKET_ERROR){
        std::cout << "Bind failed! Error: "<< WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Socket bound successfully!" << std::endl;
    return true;
}

bool Server::startListening(){
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        std::cout << "Listen failed! Error: "<< WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Server is listening on port 6379..." << std::endl;
    return true;
}

void Server::handleClient(SOCKET clientSocket){
    std::cout << "Client connected." << std::endl;

    char buffer[BUFFER_SIZE];

    while (running){
        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytesReceived == 0){
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        if (bytesReceived == SOCKET_ERROR){
            std::cout << "Receive failed! Error: "<< WSAGetLastError() << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        std::string response = commandHandler.execute(buffer);

        int bytesSent = send(
            clientSocket,
            response.c_str(),
            static_cast<int>(response.length()),
            0);

        if (bytesSent == SOCKET_ERROR){
            std::cout << "Send failed! Error: "<< WSAGetLastError() << std::endl;
            break;
        }
    }

    closesocket(clientSocket);
}

void Server::autoSave(){
    while (running){
        std::this_thread::sleep_for(std::chrono::seconds(60));

        if(!running) break;

        if (database.save(SNAPSHOT_FILE)){
            std::cout << "[AutoSave] Snapshot saved." << std::endl;
        }
        else{
            std::cout << "[AutoSave] Failed to save snapshot." << std::endl;
        }
    }
}

void Server::cleanup(){
    closesocket(serverSocket);
    WSACleanup();
}

void Server::start(){
    g_server = this;
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    std::cout << "Starting MiniRedis Server..." << std::endl;

    if (!initializeWinsock()) return;

    if (database.load(SNAPSHOT_FILE)){
        std::cout << "Snapshot loaded successfully." << std::endl;
    }
    else{
        std::cout << "No snapshot found. Starting with an empty database."
                  << std::endl;
    }

    if (!createSocket()){
        cleanup();
        return;
    }

    if (!bindSocket()){
        cleanup();
        return;
    }

    if (!startListening()){
        cleanup();
        return;
    }

    std::thread autoSaveThread(&Server::autoSave, this);
    autoSaveThread.detach();

    std::cout << "Waiting for clients..." << std::endl;

    while (running){
        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if (clientSocket == INVALID_SOCKET){
            if (!running){
                break;
            }

            std::cout << "Accept failed! Error: "
                      << WSAGetLastError() << std::endl;
            continue;
        }

        std::thread clientThread(
            &Server::handleClient,
            this,
            clientSocket
        );

        clientThread.detach();
    }

    cleanup();
}

void Server::shutdown(){
    if (!running) return;

    running = false;

    std::cout << "\nSaving database before shutdown..." << std::endl;

    database.save(SNAPSHOT_FILE);

    closesocket(serverSocket);

    std::cout << "Server stopped successfully." << std::endl;
}