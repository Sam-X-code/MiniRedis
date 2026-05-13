#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <thread>

#include "server.h"

bool Server::initializeWinsock(){
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0){
        std::cout << "WSAStartup failed!" << std::endl;
        return false;
    }

    std::cout << "Winsock initialized successfully!" << std::endl;
    return true;
}

bool Server::createSocket(){
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET){
        std::cout << "Socket creation failed!" << std::endl;
        WSACleanup();
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

    int bindResult = bind(serverSocket,(sockaddr*)&serverAddress,sizeof(serverAddress));
    if (bindResult == SOCKET_ERROR){
        std::cout << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << "Socket bound successfully!" << std::endl;
    return true;
}

bool Server::startListening(){
        int listenResult = listen(serverSocket, SOMAXCONN);

        if (listenResult == SOCKET_ERROR){
            std::cout << "Listen failed!" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

    std::cout << "Server is listening..." << std::endl;
    return true;
}

void Server::handleClient(SOCKET clientSocket){
    std::cout << "Client connected successfully!" << std::endl;

    char buffer[1024];

    while(true){
        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytesReceived == 0){
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        if (bytesReceived == SOCKET_ERROR){
            std::cout << "Receive failed!" << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        std::string response = commandHandler.execute(buffer);

        int bytesSent = send(
            clientSocket,
            response.c_str(),
            response.length(),
            0
        );

        if (bytesSent == SOCKET_ERROR){
            std::cout << "Failed to send response!" << std::endl;
            break;
        }
        
    }

    closesocket(clientSocket);
}

void Server::cleanup(){
    closesocket(serverSocket);
    WSACleanup();
}

Server::Server()
    : commandHandler(database){}

void Server::start(){
    std::cout << "Starting MiniRedis Server..." << std::endl;

    if (!initializeWinsock()) return;
    if (!createSocket()) return;
    if (!bindSocket()) return;
    if (!startListening()) return;

    std::cout << "Waiting for clients..." << std::endl;

    while (true){
        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if (clientSocket == INVALID_SOCKET){
            std::cout << "Accept failed!" << std::endl;
            continue;
        }

        std::thread clientThread(
            &Server::handleClient,
            this,
            clientSocket
        );

        clientThread.detach();
    }

}