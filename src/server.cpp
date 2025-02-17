#include <iostream> 
#include <string>
#include <vector>
#include <algorithm>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <fstream>

void clean_socket(SOCKET sock){
    closesocket(sock);
    WSACleanup();
}

int main(int argc, char** argv){
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        std::cerr << "Error: WSAStartup failed with error code: " << WSAGetLastError() << std::endl;
        return 1;
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        std::cerr << "Error: socket failed with error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // binding the socket to the port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = 0;

    if(bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR){
        std::cerr << "Error: bind failed with error code: " << WSAGetLastError() << std::endl;
        clean_socket(sock);
        return 1;
    }

    // listening for connections
    int result = listen(sock, 50);
    if(result == SOCKET_ERROR){
        std::cerr << "Error: listen failed with error code: " << WSAGetLastError() << std::endl;
        clean_socket(sock);
        return 1;
    }

    // accepting a connection
    SOCKET client = accept(sock, NULL, NULL);
    if(client == INVALID_SOCKET){
        std::cerr << "Error: accept failed with error code: " << WSAGetLastError() << std::endl;
        clean_socket(sock);
        return 1;
    }

    // receive the request
    char request[4096] = {0};
    int byte_received = recv(client, request, 4096, 0);
    if(byte_received == SOCKET_ERROR){
        std::cerr << "Error: recv failed with error code: " << WSAGetLastError() << std::endl;
        clean_socket(sock);
        return 1;
    }

    // send the response
    if(memcmp(request, "GET / HTTP/1.1", 6) == 0){
        FILE* f = fopen("../views/index.html", "r");
        if(!f){
            std::cerr << "Error: Could not open index.html" << std::endl;
            const char *error_404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(client, error_404, strlen(error_404), 0);
        }
        char buffer[4096] = {0};
        fread(buffer, 1, 4096, f);
        send(client, buffer, 4096, 0);
    }

    closesocket(client);
    clean_socket(sock);
    return 0;
}