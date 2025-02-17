#include <iostream> 
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

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
    // if(memcmp(request, "GET / HTTP/1.1", 15) == 0){
    //     char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>connection established!</h1></body></html>";
    //     send(client, response, strlen(response), 0);
    // }
    // else{
    //     char response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    //     send(client, response, strlen(response), 0);
    // }   

    return 0;
}