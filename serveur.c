#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "bizarre.h"

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define PORT 8080
#define BUFFER_SIZE 2000
char cible[15];
char nom2[20];
char nom3[20];
char test[100];
char dossier[]= "C:/Users/Alexis Waffo/Desktop/projet_gestion_de_tontine_en_c/Tontines/";
char** liste_tontine(char *nom , char *prenom , int *nombre)
 {
  int compt = 0;
  int i=1;
  char **tontines=NULL;
    FILE *f = fopen("tontines.csv", "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier tontines.csv");
        return NULL ;
    }
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), f)) {
        char reunion[15];
        i++;
        char chemin_complet[150];
        char vreunion[15];
        sscanf(ligne, "%*d;%[^;];%*f;%*f;%*[^;]",reunion);
        strcpy(vreunion , reunion);
        sprintf(chemin_complet, "%s%s.csv",dossier, reunion);
        FILE *f1 = fopen(chemin_complet, "r");
        if (f1 == NULL) {
            printf("%d", i);
            perror("Erreur lors de l'ouverture du fichier reunion");
            return NULL;
        }
        char ligne2[100];
        while ( fgets(ligne2  ,sizeof(ligne2), f1))
        {
          char nom_pers[15];
          char prenom_pers[15];
          sscanf(ligne2 , "%*d;%[^;];%[^;];%*[^;];%*[^;]", nom_pers , prenom_pers );
          if(strcmp(nom_pers , nom)==0 && strcmp(prenom_pers , prenom)==0)
          {
            compt ++;
            tontines=realloc(tontines,compt*sizeof(char*));
            tontines[compt-1]=strdup(vreunion);
            break ;
          }
         }
          fclose(f1);   
    }
    fclose(f);
    if(compt==0) printf("Vous n'etes encore dans aucune tontine \n");
    *nombre = compt ;
    return tontines;
 }
int state(char *pers ,char *prenom , char *reunion)
 {
    char tontine[90];
    char nom_pers[15];
    char prenom_pers[15];
    
    sprintf(tontine , "%s%s.csv",dossier, reunion);
    FILE *f=fopen(tontine , " r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de la tontine");
        return 4;}
    char ligne[100];
    while(fgets(ligne , sizeof(ligne), f))
    {
      char statut[15];
      sscanf(ligne , "%*d;%[^;];%[^;];%*[^;];%[^\n]", nom_pers,prenom_pers ,statut); 
        if(strcmp(statut , "admin")==0  && strcmp(prenom_pers , prenom)==0 && strcmp(nom_pers , pers)==0)
        {
          fclose(f);
          return 1 ;
        }
    }
    fclose(f);
    return 0;
 }

void menu2(SOCKET client_socket,char *nom , char *prenom , char *cible)
{   int n ;
    char buffer[300];
   
    while(1)
    {
    send(client_socket ,texte_admin, sizeof(texte_admin),0 );
    n = recv(client_socket, buffer, sizeof(buffer), 0);
    }
    send(client_socket, test, sizeof(test),0);
    
}
void menu1(SOCKET client_socket)
{
    int n;
    char message[100];
    char buffer[1000];
    int s= 1;
    int nombre;
    char **liste = liste_tontine(nom_actuel, prenom_actuel, &nombre);
    char liste_complete[1000] = "";

    for (int i = 0; i < nombre; i++) {
        char ligne[100];
        sprintf(ligne, "-> %s\n", liste[i]);
        strcat(liste_complete, ligne); 
        free(liste[i]); // Libérer la mémoire de chaque chaîne
    }
    free(liste); // Libérer le tableau de chaînes*/
    sprintf(buffer, "\n%s%s",liste_complete,menu);
    while(s==1)
    {  
     send(client_socket, buffer, strlen(buffer), 0);     // Envoi du menu au client
     n = recv(client_socket, buffer, sizeof(buffer), 0); // Réception de la réponse du client
     if (n > 0) buffer[n] = '\0';                       // Terminer la chaîne
 
     switch(buffer[0])
     {
        case '1':
        strcpy(message , "Donner le nom de la tontine : ");
            send(client_socket , message , strlen(message),0);
            n = recv(client_socket, cible, sizeof(cible), 0);
            if (n>0) cible[n] = '\0';
            printf("recu , %s %s %s %d ",nom_actuel,prenom_actuel,cible, state(nom_actuel, prenom_actuel, cible));
    
            if(state(nom_actuel, prenom_actuel, cible)==1)
            {
                send(client_socket, "ok rafraichir",13,0);
                n=recv(client_socket, message , sizeof(message),0);
                if(n>0) message[n]='\0';
                strcpy(buffer, menu);
                menu2(client_socket,nom_actuel, prenom_actuel, cible);
            }
            else
            {
                send(client_socket, "pas bon",7,0);  
                n=recv(client_socket, message , sizeof(message),0);
                if(n>0) message[n]='\0';           
            }
            break;

        case '2' :
        
            strcpy(message , "Donner le nom de la tontine : ");
            send(client_socket , message , strlen(message),0);
            n = recv(client_socket, nom2, sizeof(nom2), 0);
            if (n>0) nom2[n] = '\0';
            strcpy(message , "Donner le montant de la cotisation : ");
            send(client_socket , message , strlen(message),0);
            n = recv(client_socket, nom3, sizeof(nom3), 0);
            if (n>0) nom3[n] = '\0';
            strcpy(message , "Donner l'interet des dettes : ");
            send(client_socket , message , strlen(message),0);
            n = recv(client_socket, interet, sizeof(interet), 0);
            if (n>0) interet[n] = '\0';
            strcpy(message , "Donner le mot de passe de la tontine : ");
            send(client_socket , message , strlen(message),0);
            n = recv(client_socket, mdp_actuel, sizeof(mdp_actuel), 0);
            if (n>0) mdp_actuel[n] = '\0';
            printf("Nom recu : %s %s %s %s", nom_actuel, prenom_actuel,interet,mdp_actuel);
            strcpy(message , "ok Rafraichir");
            send(client_socket , message , strlen(message),0);
            n=recv(client_socket, message , sizeof(message),0);
            if(n>0) message[n]='\0';
            //Creer_tontine(nom2, atof(nom3), atof(interet), mdp_actuel);
            strcpy(message , "Tontine cree avec succes !\n");
            send(client_socket , message , strlen(message),0);
            n=recv(client_socket, message , sizeof(message),0);
            if(n>0) message[n]='\0';
            strcpy(buffer, menu);
            
        case '3':
          send(client_socket,"ok",2,0);
          n=recv(client_socket, message , sizeof(message),0);
            s=0;

     }

   }
   return ;
}
void menu_gene( SOCKET client_socket)
{
    int n;
    char message[200];
    char buffer[300];
    int s = 1;
    strcpy(buffer, menu_global_texte);
    while(1)
   {
    send(client_socket, buffer, strlen(buffer), 0);     // Envoi du menu au client
    n = recv(client_socket, buffer, sizeof(buffer), 0); // Réception de la réponse du client
    if (n > 0) buffer[n] = '\0';                       // Terminer la chaîne

    switch(buffer[0])
    {
       case '1':
       strcpy(message , "Enregistrement, donner le nom :  ");
       send(client_socket , message , strlen(message),0);
       n = recv(client_socket, nom_actuel, sizeof(nom_actuel), 0);
       if (n>0) nom_actuel[n] = '\0';
       strcpy(message , "Enregistrement, donner le prenom :  ");
       send(client_socket , message , strlen(message),0);
       if(n>0) n= recv(client_socket, prenom_actuel, sizeof(prenom_actuel), 0);
       prenom_actuel[n] = '\0';
       strcpy(message , "Enregistrement, donner mdp :  ");
       send(client_socket , message , strlen(message),0);
       n= recv(client_socket, mdp_actuel, sizeof(mdp_actuel), 0);
       if(n>0) mdp_actuel[n] = '\0';
       printf("Nom recu : %s %s %s", nom_actuel, prenom_actuel,mdp_actuel);
       strcpy(message , "ok Rafraichir");  
       send(client_socket , message , strlen(message),0);
       n=recv(client_socket, message , sizeof(message),0);
       if(n>0) message[n]='\0';
       Ajouter_utilisateur(nom_actuel, prenom_actuel,mdp_actuel);
       strcpy(message , "ok Utilisateur ajoute avec succes !\n");
       send(client_socket , message , strlen(message),0);
       n=recv(client_socket, message , sizeof(message),0);
       if(n>0) message[n]='\0';
       strcpy(buffer, menu_global_texte);
       break;


        case '2':
    {
    strcpy(message , "Connexion, donner le nom :  ");
    send(client_socket , message , strlen(message),0);
    n = recv(client_socket, nom_actuel, sizeof(nom_actuel), 0);
     if (n>0) nom_actuel[n] = '\0';
    strcpy(message ,"Connexion, donner le prenom :  ");
    send(client_socket , message , strlen(message),0);
     if(n>0) n= recv(client_socket, prenom_actuel, sizeof(prenom_actuel), 0);
    prenom_actuel[n] = '\0';
    strcpy(message , "Connexion, donner mdp :  ");
    send(client_socket , message , strlen(message),0);
    n= recv(client_socket, mdp_actuel, sizeof(mdp_actuel), 0);
    if(n>0) mdp_actuel[n] = '\0';
    printf("Nom recu : %s %s %s", nom_actuel, prenom_actuel, mdp_actuel);
    strcpy(message , "ok Rafraichir");
    send(client_socket , message , strlen(message),0);
    n=recv(client_socket, message , sizeof(message),0);
    if(n>0) message[n]='\0';
    if(authentifier(nom_actuel, prenom_actuel, mdp_actuel) == 1) {
        // Message de bienvenue
        strcpy(message, "ok Bienvenu ");
        strcat(message, nom_actuel);
        strcat(message, " ");
        strcat(message, prenom_actuel);
        strcat(message, "\n");
        send(client_socket, message, strlen(message), 0);
        n = recv(client_socket, message, sizeof(message), 0);
        if (n > 0) message[n] = '\0';
        strcpy(buffer, menu_global_texte);
        menu1(client_socket);
    }
    else
    {
        strcpy(message , "ok Nom ou mot de passe incorrecte \n");
        send(client_socket , message , strlen(message),0);
        n=recv(client_socket, message , sizeof(message),0);
        strcpy(buffer, menu_global_texte);

    
    }
     break;
   }
    case '4':
    s = 0;
    printf("il a choisi de sortir");
    send(client_socket, "stop",4,0);
    return;
    break ;
    }
  }    
  // Fermer la connexion avec le client
  printf("Connexion avec le client fermee.\n");
  closesocket(client_socket);

}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_size;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Échec de l'initialisation de Winsock. Code d'erreur : %d\n", WSAGetLastError());
        system("pause");
        return 1;
    }

    printf("Serveur en attente de connexions sur le port %d...\n", PORT);
    
    // Créer un socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Erreur de création du socket. Code d'erreur : %d\n", WSAGetLastError());
        system("pause");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Lier le socket à l'adresse du serveur
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Erreur de liaison. Code d'erreur : %d\n", WSAGetLastError());
        system("pause");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }


    // Écouter les connexions entrantes
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Erreur d'écoute. Code d'erreur : %d\n", WSAGetLastError());
        system("pause");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }


    // Accepter les connexions entrantes et gérer les clients
   
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == INVALID_SOCKET) {
            printf("Erreur d'acceptation. Code d'erreur : %d\n", WSAGetLastError());
        
        }

        printf("Client connecte\n");
        // Gérer la communication avec le client
        menu_gene(client_socket);
        closesocket(server_socket);
        WSACleanup();
    
    

    return 0;
   
}
