#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <thread>
#include <iostream>
#include <windows.h>
#include <string>
#include <time.h>

#define BUFFER_LENGTH 1024
#define PORT 9000

using namespace std;

WSAData wsaData;
bool connected;

string username;

int main(int argc, char** argv)
{
    if (WSAStartup(WORD((2, 2)), &wsaData) != 0)
    {
        cerr << "Error al inicializar WinSock: " << WSAGetLastError() << endl;
        return WSAGetLastError();
    }

    int result;
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);

    if (result = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0)
    {
        cerr << "Error al conectarse al servidor (Código de error: " << result << ")" << endl;
        return result;
    }

    cout << "Conectado al servidor con éxito." << endl;

    char buffer[BUFFER_LENGTH];
    int bytes = recv(clientSocket, buffer, BUFFER_LENGTH, 0);
    if (bytes > 0)
    {
        cout << "Fecha y hora del servidor: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}