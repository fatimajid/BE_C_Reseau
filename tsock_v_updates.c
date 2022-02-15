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

//construction du message
    void construire_message(char *message, char motif, int lg, int i)
    {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    if (i>=27)
        {
        if (i%26==0)
            motif = 'z';
        else
            motif = alphabet[i%26-1];
        }

    else
        motif=alphabet[i - 1];

    for (int j=0;j<lg-5;j++)
        {
        *(message+j+5) = motif;
        }
    }
        
    //Affichage des messages

    void afficher_message(char *message, int lg)

    {
  	for (int i=0;i<lg;i++)
  	    printf("%c", *(message+i));
  	printf("]\n");
    }


 // Fonctin de la source utilisant UDP
void source_udp (int port, char * host, int nb_msgs, int taille) {//void source_udp( )
    //création du socket 
    int sock_local ; //= socket (AF_INET,SOCK_DGRAM,0);     
    
    // test du socket
    if ((sock_local = socket (AF_INET,SOCK_DGRAM,0))==-1){
        printf("echec de création du socket");
        exit(1);}
    
    //Construction de l'@ distante du socket
    struct sockaddr_in adresse_distante;                //initialisation d'une adresse locale de type struct sockaddr_in
    adresse_distante.sin_family = AF_INET;             //domaine du socket                                    
    adresse_distante.sin_port = port;                  //numéro de port
    
    //Affectation de l'adesse IP du socket
    struct hostent *hp;
    struct sockaddr_in adr_distant;
    
    //Test 
    if ((hp = gethostbyname(host)) == NULL){
        printf("erreur gethostbyname\n");
        exit(1);
    }
    memcpy((char*)&(adresse_distante.sin_addr.s_addr),hp->h_addr,hp->h_length);
    
    //construction et affichage du message
    int lg_msg = 30;
    char *message=malloc(sizeof(char)*lg_msg) ; //initialisation de la variable message
    
    printf("%d\n",nb_msgs);
    for (int i=1; i<=nb_msgs ; i++)
    {
        
    	construire_message(message,'a',lg_msg,i);
      	printf("SOURCE : Envoi n°%d (%d) : [", i, lg_msg);
      	

      	if((sendto(sock_local,message,lg_msg,0,(struct sockaddr*)&adresse_distante,sizeof(adresse_distante)))==-1)   
      	{
      		printf("Erreur sendto\n");
      		exit(1);
      	}
      	afficher_message(message,lg_msg);
    }

  	if(close(sock_local)==-1) //fermeture + test erreur à la fermeture
    {
      printf("Echec à la destruction du socket\n");
      exit(1);
    }
}
   
    
    
// Fonctin du puit utilisant UDP 
void puit_udp(int port, int nb_msgs, int taille){
    int sock_local; //socket(AF_INET, SOCK_DGRAM,0); //création du socket
    // test du socket
    if ((sock_local = socket (AF_INET,SOCK_DGRAM,0))==-1){
        printf("echec de création du socket");
        exit(1);}
        
    struct sockaddr_in adresse_locale,adresse_distante;                //initialisation d'une adresse locale de type struct sockaddr_in
    adresse_locale.sin_family = AF_INET;             //domaine du socket                                    
    adresse_locale.sin_port = port;                  //numéro de port
    adresse_locale.sin_addr.s_addr = INADDR_ANY;
    
    //Association de l'@ du socket avec le puit
    int lg_adresse_locale = sizeof(adresse_locale);
    
    //test du bind
    
    if (bind(sock_local,(struct sockaddr*)&adresse_locale,lg_adresse_locale) == -1){
        printf("echec du bind\n");
        exit(1);
        }
        
    
    int lg_msg = 30;
    char *message=malloc(sizeof(char)*lg_msg) ; //initialisation de la variable message
    
    int i=1;
    int lg_dist=sizeof(adresse_distante);
    while(recv!=0)
    {
        int recv=recvfrom(sock_local,message,lg_msg,0,(struct sockaddr*)&adresse_distante,&lg_dist);
        if (recv==-1) //process de réception + test erreur
        {
            printf("Erreur receive from\n");
            exit(1);
        }

        if (recv!=0)
        {
            printf("PUITS : Réception n°%d (%d) : [",i,lg_msg);
            afficher_message(message,recv);
        }
        if (i==nb_msgs)
        {
            recv=0;
            printf("On a atteint le nombre de messages à recevoir\n");
        }

        i++;
    }
  
   if(close(sock_local)==-1) //fermeture + test erreur à la fermeture
    {
      printf("Echec à la destruction du socket\n");
      exit(1);
    }
}    
        

//source avec TCP

void source_tcp(int port, char * host, int nb_msgs, int taille){
    printf("source tcp\n");
    //Création socket + test de création
    int socket_local;
	if((socket_local=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Erreur à l'ouverture du Socket Stream (TCP)");
		exit(1);
	}
	
	//Construction adresse socket locale et distante
	
	struct sockaddr_in adresse_locale, adresse_distante ;
	
	//construction de l'adresse locale du socket
	
                                                        //initialisation de l'adresse du socket local (envoie le message)
    adresse_locale.sin_family = AF_INET;             //domaine du socket                                    
    adresse_locale.sin_port = htons(port);                  //numéro de port
    
    //Affectation de l'adesse IP du socket
    struct hostent *hp;
    
    if ((hp = gethostbyname(host)) == NULL){
        printf("erreur de requête IP (gethostbyname) \n");
        exit(1);
    }
    memcpy((char*)&(adresse_locale.sin_addr.s_addr), hp->h_addr , hp->h_length);
	
	//Construction de l'@ distante du socket
                                                       //initialisation de l'adresse du socket distant (reçois le message)d'une adresse locale de type struct sockaddr_in
    adresse_distante.sin_family = AF_INET;             //domaine du socket                                    
    adresse_distante.sin_port = htons(port);                  //numéro de port
    
    //Affectation de l'adesse IP du socket
    
    if ((hp = gethostbyname(host)) == NULL){
        printf("erreur de requête IP (gethostbyname) \n");
        exit(1);
    }
    memcpy((char*)&(adresse_distante.sin_addr.s_addr), hp->h_addr , hp->h_length);
	                                                                            
    
	//Demande de connexion
	
	if (connect(socket_local,(struct sockaddr *)&adresse_distante,sizeof(adresse_distante))==-1)
	{
		printf("Erreur lors de la connexion, en attente de la tentative suivante \n");
        exit(1);
	}
	
	//Transfert des données
	
	int lg_msg = 30; //la longueur du message a envoyer
	char *message=malloc(sizeof(char)*lg_msg) ; //initialisation de la variable message
	printf("%d\n",nb_msgs);
	for (int i=1; i<=nb_msgs;i++)
	{
	    
		
		//construction du message
    	construire_message(message,'a',lg_msg,i);
      	printf("SOURCE : Envoi n°%d (%d) : [", i, lg_msg);
      	

      	if((sendto(socket_local,message,lg_msg,0,(struct sockaddr*)&adresse_distante,sizeof(adresse_distante)))==-1)   
      	{
      		printf("Erreur sendto\n");
      		exit(1);
      	}
      	
      	afficher_message(message,lg_msg);
        }

  	    if(close(socket_local)==-1) //fermeture + test erreur à la fermeture
        {
        printf("Echec à la destruction du socket\n");
        exit(1);
        }

		//Affichage du message
		afficher_message(message,lg_msg);

		//Envoi du message 
        int envoi=write(socket_local,message,lg_msg);
        
		if (envoi==-1)
		{
			printf("Echec de l'envoi du message (fonction write en défaut)\n");
			exit(1);
		}
	

	//Fermeture connexion

	if(shutdown(socket_local,2)==-1)
	{
		printf("Erreur à la fermeture de la connexion TCP \n");
		exit(1);
	}

	if (close(socket_local)==-1)
	{
		printf("Echec de la fermeture du socket distant");
		exit(1);
	}

	free(message);
	printf("Envoi effectué avec succes\n"); 
	}   

//puit TCP
void puit_tcp(int port,int nb_msgs, int taille){

    int sock , sock2; //sock bis local orienté échanges 
	struct sockaddr addr_distant;
	struct sockaddr_in addr_local;
	//struct hostent *hp;
	
	unsigned int lg_addr_distant=sizeof(addr_distant);
	int lg_addr_local=sizeof(addr_local);
	
	char motif;
	char * message=malloc(taille*sizeof(char)); //free en fin de programme pour libérer l'espace mémoire
	int lg_recv=-1;
	
	//Création socket local 	

	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Echec de la création d'un socket local\n");
		exit(1);
	}

	//Construction adresse socket local | Affectation port et domaine

	memset((char*)&addr_local, 0 , sizeof(addr_local));
	addr_local.sin_family=AF_INET;
	addr_local.sin_addr.s_addr=INADDR_ANY;
	addr_local.sin_port = htons(port);  //ntohs 
	printf("+++ Debug: %d\n", port);
	
	//Bind

	if (bind(sock,(struct sockaddr *)&addr_local, lg_addr_local)==-1)
	{
		printf("Echec du bind.\n");
		exit(1);
	}
	//Test connexions entrantes 

	if (listen(sock,100)<0)	
	{
		printf("Trop de connexions en attentes, échec de la demande\n");
		exit(1);
	}

	//Accept connexion
    
	if (nb_msgs==-1)
		nb_msgs=10;
    
	if ((sock2 = accept(sock,(struct sockaddr*)&addr_distant,&lg_addr_distant))==-1)
	{
		printf("Refus de connexion par le serveur\n");
		exit(1);
	}
    printf("accept\n");
    //Reception des messages au niveau du socket d'échange

    //Reception des données
    int i=1;
    while(lg_recv!=0)
    {

        if((lg_recv=read(sock2,message, taille))<0)
        {
            printf("Echec de la lecture du message entrant \n");
            exit(1);
        }
        if (lg_recv!=0)
        {
            printf("PUITS : Réception n°%d (%d) [" , i , taille);
            afficher_message(message, lg_recv);
            i++;
        }
    }

	//elimination du socket
	if(close(sock)==-1)
	{
		printf("Impossible de fermer le socket");
		exit(1);
	}
	free(message);
}


// R du BAL

void RBAL(int port, char* dest, int nBAL)
{
    //Déclarations
    int sock;
    struct sockaddr_in addr_distant;
    int lg_addr_distant = sizeof(addr_distant);
    struct hostent *hp;
    char *message; //Penser au free en fin de programme pour libérer l'espace mémoire
    int envoi = -1;
    int lg_pdu=50;
    int lg_recv=-1;
    int lg;
    int nb;
    char *pdu = malloc(lg_pdu*sizeof(char));

//---------------------------------------
//--------Etablissement connexion--------
//---------------------------------------

    sprintf(pdu,"1 %d",nBAL);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Erreur à l'ouverture du Socket Stream");
        exit(1);
    }

    //Construction adresse socket distant
    memset((char *) &addr_distant, 0, sizeof(addr_distant));
    addr_distant.sin_family = AF_INET;  //Internet
    addr_distant.sin_port = port;       //Numéro de Port

    //Affectation IP
    if ((hp = gethostbyname(dest)) == NULL) {
        printf("Erreur de requête IP.\n");
        exit(1);
    }

    memcpy((char *) &(addr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);

    //Demande de connexion

    if (connect(sock, (struct sockaddr *) &addr_distant, sizeof(addr_distant)) == -1) {
        printf("Erreur lors de la connexion, en attente de la tentative suivante \n");
        exit(1);
    }

//-----------------------------------------
//----------------Envoi PDU----------------
//-----------------------------------------

    if ((envoi = write(sock, pdu, lg_pdu)) == -1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
    {
        printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
        exit(1);
    }
    char*lgmsg=malloc(maxsize* sizeof(char));
    nb=10;
    int n=1;
    lg_recv=1;
    printf("             PUITS : Réception du contenu de la BAL n°%d\n",nBAL);
    printf("____________________________________________________________________\n\n");

    while(n<=nb)
    {
        if ((lg_recv=read(sock,lgmsg,lg_pdu))==-1)
        {
            printf("Erreur à la réception du PDU de longueur de message\n");
            exit(1);
        }
        sscanf(lgmsg,"%d %d", &lg , &nb);
        if (lg==-1)
        {
            printf("       ATTENTION : Pas de courrier à récupérer dans la BAL n°%d\n\n",nBAL);
            exit(0);
        }

        message=malloc(lg*sizeof(char));
        if ((lg_recv=read(sock,message,lg))==-1)
        {
            printf("Erreur à la réception du message\n");
            exit(1);
        }
        printf("PUITS : Réception de la lettre n°%d : [",n);
        afficher_message(message,lg);
        n++;
    }

    printf("Fermeture de la Connexion\n");
    //elimination du socket
    if(close(sock)==-1)
    {
        printf("Impossible de fermer le socket");
        exit(1);
    }
}



	  
int main (int argc, char **argv)
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
            proto = 0;
            printf("usage de protocole udp");
             
            break;
        
		case 'n':
			
      		nb_message = atoi(optarg);
      		//printf("%s\n",optarg);
      		//printf("%d\n",nb_message);
            if (nb_message>100000)
            {
              printf("Trop d'envois, n> 100 000\n");
              exit (1);
            }
      		break;

        case 'l':
            lg_message = atoi(optarg);
          break;
          
		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

    int port = atoi(argv[argc-1]);
    port =htons(port);
    
    if (port!=-1)
    {
        port=htons(port);
    }
    
    char *dest ;
    dest=argv[argc-2];
    
	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1) {
		printf("on est dans la source\n");
		//char * destination = argv[argc-2];
	 } else
		printf("on est dans le puit\n");

	if (nb_message != -1) {  //si le nbre de mesgs est different de -1
	    
	    if (source ==1){
	        printf("nb de tampons à envoyer : %d\n", nb_message);
	        
	    /*if(source == 1 && proto == 0){ //source udp
	    printf("nb de tampons à envoyer : %d\n", nb_message);
	    source_udp(port, &dest, nb_message, nb_message);
	    }
	    if (source =0 && proto == 0){ //puit udp
	    printf("nb de tampons à recevoir : %d\n", nb_message);
	    puit_udp(port,nb_message, lg_message);
	    }
	    if(source = 1 && proto == 1){ //source tcp
	    printf("nb de tampons à envoyer : %d\n", nb_message);
	    source_tcp(port, &dest, nb_message, nb_message);
	    }
	    if (source =1 && proto == 0){ //puit tcp
	    printf("nb de tampons à recevoir : %d\n", nb_message);
	    puit_tcp(port, nb_message, lg_message);
	    }
	    }
	    
	else{ //si le nbr de mesgs est 10
	    nb_message = 10 ;
	    
	    if(source == 1 && proto == 0){ //source udp
	    printf("nb de tampons à envoyer : %d\n", nb_message);
	    source_udp(port, dest, nb_message, nb_message);
	    }
	    if (source =0 && proto == 0){ //puit udp
	    printf("nb de tampons à recevoir : %d\n", nb_message);
	    puit_udp(port, dest, nb_message, lg_message);
	    }
	    if(source = 1 && proto == 1){ //source tcp
	    printf("nb de tampons à envoyer : %d\n", nb_message);
	    source_tcp(port, dest, nb_message, nb_message);
	    }
	    if (source =1 && proto == 0){ //puit tcp
	    printf("nb de tampons à recevoir : %d\n", nb_message);
	    puit_tcp(port, dest, nb_message, lg_message);
	    }
	    }*/
	 if (nb_message == -1)  
	     nb_message =10;
	 if(source == 1)
	     printf("nb de tampons à envoyer : %d\n", nb_message);
	 else
	     printf("nb de tampons à recevoir : %d\n", nb_message);
	  
	 if(source == 1 && proto == 0){
	  source_udp(port, dest, nb_message, nb_message); }
	 else if (source =0 && proto == 0){ //puit udp
	    puit_udp(port, nb_message, lg_message);
	    }
	 if(source = 1 && proto == 1){ //source tcp
	    source_tcp(port, dest, nb_message, nb_message);
	    }    
     else if(source =0 && proto == 1){ //puit tcp
	    puit_tcp(port,nb_message, lg_message);
	    }
     else
     {printf("erreur de configuration\n");
     exit(1);}
	    
	    
	   /* if(source == 1 && proto = 0)
		printf("nb de tampons à envoyer = 10 par défaut\n"); }
		
	if (source =0 && proto = 0){ //puit udp
	}
	if (source =0 && proto = 1){//puit tcp
	}
	if (source =1 && proto = 0){ //source udp
	}
	if (source = 1 && proto = 1){ //source tcp
	}
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} 
	else { 
		if (source == 1) {
			nb_message = 10 ;
		    printf("nb de tampons à envoyer = 10 par défaut\n");
		    if (proto == 0 ) 
			    source_udp(port, dest, nb_message, nb_message);
		    else 
		        source_tcp(port, dest, nb_message, nb_message); 
		                  }
		else{
		    printf("nb de tampons à envoyer = infini\n");
		    if (proto==0)
               {
               puit_udp(port, dest, nb_message, lg_message); //void puit_udp(int port, char * host, int nb_msgs, int taille)
               }
	        else
	           puit_tcp(port, nb_message, nb_message);      //puit_tcp(int port,int nb_msgs, int taille)

	        }
	
        }*/
     
}}
return 0;
}

