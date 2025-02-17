#include <iostream> 
#include <string>
#include <vector>
#include <algorithm>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <fstream>


int main(int argc, char** argv){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    // binding the socket to the port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = 0;
    bind(s, (sockaddr*)&addr, sizeof(addr));

    // listening for connections
    listen(s, 10);

    // accepting a connection
    SOCKET client = accept(s, NULL, NULL);

    // receive
    char request[256] = {0};
    recv(client, request, 256, 0);

    // send
    if(memcmp(request, "GET / HTTP/1.1", 6) == 0){
        FILE* f = fopen("../views/index.html", "r");
        char buffer[256] = {0};
        fread(buffer, 1, 256, f);
        send(client, buffer, 256, 0);
    }

    return 0;
}