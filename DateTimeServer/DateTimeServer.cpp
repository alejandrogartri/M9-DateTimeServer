#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <thread>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <vector>
#include <algorithm>

#define MAX_USERS 5
#define BUFFER_LENGTH 1024
#define PORT 9000

using namespace std;

WSAData wsaData;
SOCKET serverSocket;

int main(int argc, char** argv)
{
    // Inicializamos WinSock 2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "Error al inicializar WinSock: " << WSAGetLastError() << endl;
        return WSAGetLastError();
    }

    int result;

    // Creamos el socket y la dirección IP

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);

    // Asignamos la dirección al socket
    if (result = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0)
    {
        cerr << "Error al asignar la dirección IP al socket: " << WSAGetLastError() << endl;
        return result;
    }

    // Iniciamos la escucha de clientes
    if (result = listen(serverSocket, MAX_USERS) != 0)
    {
        cerr << "Error al iniciar la escucha en el servidor (Código de error: " << result << ")" << endl;
        return result;
    }

    while (1)
    {
        cout << "Esperando conexiones..." << endl;
        sockaddr_in clientAddress;
        int addressLength = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addressLength);

        cout << "Conexión aceptada desde " << inet_ntoa(clientAddress.sin_addr) << endl;

        auto t = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &t);

        ostringstream oss;
        oss << put_time(&localTime, "%d/%m/%Y %H:%M:%S");
        string stringTime = oss.str();

        send(clientSocket, stringTime.c_str(), BUFFER_LENGTH, 0);
        cout << "Fecha y hora enviadas: " << stringTime << endl;

        closesocket(clientSocket);
        cout << "Conexión terminada." << endl;
    }

    WSACleanup();
    return 0;
}