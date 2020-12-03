#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
// Les variables pre processeur
#define SERV "127.0.0.1"
#define PORT 12345
// socket et numéro de port
int sock, port;
// Le buffer pour les messages
char mess[80];
// champs d entete addresseServeur
struct sockaddr_in addresseServeur;
// Le nom du server
struct hostent* server;

void creer_socket()
{
    port = PORT;
    server = gethostbyname(SERV);
    if(!server) {fprintf(stderr, "Problème serveur \"%s\"\n", SERV); exit(1);}
    // famille d adresse internet
    addresseServeur.sin_family = AF_INET;
    // Le socket
    sock = socket(addresseServeur.sin_family, SOCK_STREAM, 0);
    // mise a zero de la zone adresse
    bzero(&addresseServeur, sizeof(addresseServeur));
    // on copie server->h_addr dans &addresseServeur.sin_addr.s_addr avec une taille de server->h_lengt
    bcopy(server->h_addr, &addresseServeur.sin_addr.s_addr, server->h_length);
    // Le port
    addresseServeur.sin_port = htons(port);
}

int main()
{
    creer_socket();
    
    printf ("Connection au serveur\n");
    if(connect(sock, (struct sockaddr*)&addresseServeur, sizeof(addresseServeur)) < 0)
        { perror("Erreur lors de la connection"); exit(1); }
    printf ("Connecté au serveur %s sur le port %i \n", SERV, PORT);

    strcpy(mess,"");
    while (strncmp(mess,"fin",3) != 0)
    { 
        printf ("Question : "); 
        gets(mess);
        write(sock,mess,80);
        read (sock,mess,80);
        printf ("Reponse : %s\n",mess);
    }

    close (sock); 

    return 0;
}