#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#define PORT 12345

int sock, socket2, lg;
char mess[80];
struct sockaddr_in local; // champs d entete local
struct sockaddr_in distant; // champs d entete distant

void creer_socket()
{
    // preparation des champs d entete
    // mise a zero de la zone adresse
    bzero(&local, sizeof(local));
    // famille d adresse internet
    local.sin_family = AF_INET;
    // numero de port
    local.sin_port = htons(PORT);
    // types d adresses prises en charge
    local.sin_addr.s_addr = INADDR_ANY;
    // fin de remplissage
    bzero(&(local.sin_zero),8);
    lg = sizeof(struct sockaddr_in);
    // creation socket du serveur mode TCP/IP
    if((sock = socket(local.sin_family, SOCK_STREAM, 0)) < 0) 
        { perror("socket"); exit(1); }
    // nommage de la socket
    if(bind(sock, (struct sockaddr*)&local, sizeof(struct sockaddr)) == -1) 
        { perror("bind"); exit(1); }
}

int main()
{
    // creation socket
    creer_socket();
    // mise a l ecoute
    if(listen(sock, 5) == -1) 
        {perror("listen"); exit(1);}

    // boucle sans fin pour la gestion des connexions
    while(1)
    { 
        
        // attente connexion client
        printf ("En attente d un client\n");
        if((socket2 = accept(sock, (struct sockaddr*)&distant, (socklen_t*)&lg)) == -1)
                { perror("accept"); exit(1); }
        printf ("Un client vient de se connecter sur le port %i de l'adresse locale\n", PORT);
        strcpy(mess,"");
        while (strncmp(mess,"fin",3) != 0)
        {
            read(socket2, mess, 80);
            printf("le client me dit %s \n", mess);
            write(socket2, "message recu !", 80);
        }
        // on lui ferme la socket
        close(socket2);
    }

    return 0;
}