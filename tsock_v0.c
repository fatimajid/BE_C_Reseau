/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
/*pour la gestion des chaines de caracteres*/
#include <string.h>

void construire_message (char *message, char motif, int lg) {
    int i;
    for (i=0 ; i<lg; i++) message[i] = motif; 
}
void afficher_message (char *message, int lg) {
    int i;
    printf("message construit : ");
    for (i=0 ; i<lg ; i++) printf("%c", message[i]) ; printf("\n"); 
}


 // Fonctin de la source utilisant UDP
void source_udp (int port, char * host, int nb_msgs, int taille) {//void source_udp( )
    //création du socket 
    int sock_local = socket (AF_INET,SOCK_DGRAM,0);     
    
    // test du socket
    if ((sock_local = socket (AF_INET,SOCK_DGRAM,0))==-1){
        printf("echec de création du socket");
        exit(1);}
    
    //Construction de l'@ distante du socket
    struct sokaddr_in adresse_distante;                //initialisation d'une adresse locale de type struct sockaddr_in
    adresse_distante.sin_family = AF_INET;             //domaine du socket                                    
    adresse_distante.sin_port = port;                  //numéro de port
    
    //Affectation de l'adesse IP du socket
    struct hostent *hp;
    struct sockaddr_in adr_distant;
    
    if ((hp = gethostbyname(host)) == NULL){
        printf("erreur gethostbyname\n");
        exit(1);
    }
    memcpy((char*)&(adresse_distante.sin_addr.s_addr),hp->h_addr,hp->h_length);
    
    
    //construire_message
    
        
}

// Fonctin du puit utilisant UDP
void puit_udp(int port, char * host, int nb_msgs, int taille){
    int sock = socket(AF_INET, SOCK_DGRAM,0); //création du socket
    // test du socket
    if ((sock_local = socket (AF_INET,SOCK_DGRAM,0))==-1){
        printf("echec de création du socket");
        exit(1);}
        
    struct sokaddr_in adresse_locale;                //initialisation d'une adresse locale de type struct sockaddr_in
    adresse_locale.sin_family = AF_INET;             //domaine du socket                                    
    adresse_locale.sin_port = port;                  //numéro de port
    adresse_locale.sin.add.s.adr = INADDR_ANY;
    
    //Association de l'@ du socket avec le puit
    int lg_adresse_locale = sizeof(adresse_locale);
    //test du bind
    if (bind(sock_local,(struct sockaddr*)&adresse_locale,lg_adresse_distante) == -1){
        printf("echec du bind\n");
        exit(1);
        }
        
}        
        
void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int lg_message =30; 
	int proto = 1 ; /* 1 pour TCP et 0 pour udp */
	while ((c = getopt(argc, argv, "pn:su")) != -1) { //Ajout de l'ption 'u' dans le programme
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;
		//Ajout de l'option u du protocole UDP
        case 'u':
            printf("usage de protocole udp");
            proto = 0; 
            break;
        //Fin de l'étape 1
		case 'n':
			nb_message = atoi(optarg);
			break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

    int port = atoi(argv[argc-1]);
	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1) {
		printf("on est dans le source\n");
		char * destination = argv[argc-2];
	 } else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
		    printf("nb de tampons à envoyer = 10 par défaut\n");
		    if (proto == 0 ) 
			    source_udp(port, destination, nb_message, nb_message);
		    //else 
		       // source_tcp(); 
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
}

  
    
    
    
    
    
    
    
