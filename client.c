#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define SERVER_IP "127.0.0.1"  // IP du serveur
#define PORT 8080
#define BUFFER_SIZE 4000

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Échec de l'initialisation de Winsock. Code d'erreur : %d\n", WSAGetLastError());
        return 1;
    }

    // Créer un socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Erreur de création du socket. Code d'erreur : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Se connecter au serveur
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Erreur de connexion. Code d'erreur : %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    while (1) {
        int n = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) 
        {
            buffer[n] = '\0';  // Terminer la chaîne
            if(buffer[0] == 'o' && buffer[1] == 'k')
             {
                system("cls");
                printf("\n\n");
                send(client_socket,"ok",2, 0);
                continue;
            }
            else if(strcmp(buffer , "stop")==0) return 0;
            else printf("Message du serveur : %s\n", buffer);
        }
         else 
        {
            printf("Erreur de réception ou connexion fermée par le serveur.\n");
            break;
        }

        printf("Reponse : ");
        fgets(buffer, sizeof(buffer), stdin);
        // Supprimer le caractère de retour à la ligne (\n) si présent
        buffer[strcspn(buffer, "\n")] = '\0';

        send(client_socket, buffer, strlen(buffer), 0);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}