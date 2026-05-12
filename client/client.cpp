#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

int main(){
    std::cout << "MiniRedis Client Starting..." << std::endl;

    // Initialize Winsock
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0){
        std::cout << "WSAStartup failed!" << std::endl;
        return 1;
    }

    std::cout << "Winsock initialized successfully!" << std::endl;

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET){
        std::cout << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    std::cout << "Socket created successfully!" << std::endl;

    // Server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6379);

    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Connect to server
    int connectResult = connect(clientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress));
    if (connectResult == SOCKET_ERROR){
        std::cout << "Connection failed!" << std::endl;

        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    while(true){
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);

        if (message == "exit"){
            break;
        }

        int bytesSent = send(
            clientSocket,
            message.c_str(),
            message.length(),
            0
        );

        if (bytesSent == SOCKET_ERROR){
            std::cout << "Send failed!" << std::endl;
            break;
        }
        
        char buffer[1024];

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytesReceived == 0){
            std::cout << "Server disconnected." << std::endl;
            break;
        }

        if (bytesReceived == SOCKET_ERROR){
            std::cout << "Receive failed!" << std::endl;
            break;
        }
        else{
            buffer[bytesReceived] = '\0';
            std::cout << buffer << std::endl;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}