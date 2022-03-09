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


//_________________________PARTIE 2 BAL________________________//
//_____________________________________________________________//


//Définitions des types

typedef struct BAL 
{
    int num ;       //numéro de la BAL
    int nb;         //nombre de lettres(BAL) existantes
    struct LETTRE * l_first ;
    struct LETTRE * l_last ;
    struct LETTRE * l_current ;
    struct BAL * suiv ;
}BAL ;

typedef struct LETTRE 
{
    int num ;       //numéro de la lettre afin de faire la correspondance
    int lg;         // longueur ou la taille de la lettre
    char *message ; //message de la lettre
    struct LETTRE * suiv ;  //pointeur de la liste des lettres pour qu'on puisse enchainer la lecture|recuperation
}LETTRE ;

typedef struct LISTE_BAL
{   //pointeur sur la liste des BAL pour bien manipuler
    struct BAL * first ;
    struct BAL * last ;
    struct BAL * current ;
    int nb;
}LISTE_BAL;

//---------------------------------------------------
//---------Déclaration des fonctions-----------------
//---------------------------------------------------

LISTE_BAL* init_BAL();
void add_BAL(int num , LISTE_BAL* liste);       //Ajout d'une BAL
void add_LETTRE (int n, int lg, BAL* bal , char* mess);
void printLISTE(struct LISTE_BAL* liste);
void printBAL(BAL* bal,int lg);
BAL* find_BAL(LISTE_BAL*liste, int num);
int  find_BALR(LISTE_BAL*liste, int num);

void EBAL(int port, char* dest, int nb_message, int lg_msg, int nBAL);
void SBAL(int port, char* dest);
void RBAL(int port, char* dest, int nBAL);


//---------------------------------------------------
//--------------------GESTION BAL--------------------
//---------------------------------------------------

LISTE_BAL* init_BAL()
{
    LISTE_BAL* liste =(LISTE_BAL*)malloc(sizeof(struct LISTE_BAL));
    liste->first = NULL;
    liste->last=NULL;
    liste->current = NULL;
    liste->nb=0;
    return liste ;
}

//------------------------------------------------
//----Afficher le contenu d'une Liste de BAL------
//------------------------------------------------
void printLISTE(struct LISTE_BAL* liste)
{
    printf("              __________________________________________\n");
    printf("                verification générale des BAL de notre liste :\n\n");
    printf("                        %d BAL exist dans notre liste       \n\n",liste->nb);
    liste->current=liste->first;
    while (liste->current!=NULL)
    {
    printf("                        BAL n°%d : %d Lettres          \n",liste->current->num,liste->current->nb);
        liste->current=liste->current->suiv;
    }
    printf("              __________________________________________\n\n");

}


//------------------------------------------------
//--------Afficher le contenu d'une BAL-----------
//------------------------------------------------
void printBAL(BAL* bal,int lg)
{
     printf("Contenu de la BAL n°%d qui contient %d lettres \n",bal->num,bal->nb) ;
    bal->l_current=bal->l_first;
    printf("\n");
    int n=1;
    while(bal->l_current!=NULL)
    {
        printf("BAL n°%d | %d Lettres, lettre n°%d : [",bal->num,bal->nb,n);
        afficher_message(bal->l_current->message,lg);  //void afficher_message(char *message, int lg) et bal->l_current->message de type char *
        bal->l_current=bal->l_current->suiv;
        n++;
    }
    printf("\n\n");

}

//------------------------------------------------
//----------------Ajouter une BAL-----------------
//------------------------------------------------

//void add_BAL(int num , LISTE_BAL* liste);  
void add_BAL(int n, LISTE_BAL * liste)      //Ajouter une BAL
{
    BAL *nouv =malloc(sizeof(struct BAL)); //création du pointeur de la nouvelle BAL
    nouv->num=n;                           //le numéro (n) de la nouvelle BAL 
    nouv->nb=0;                            //le nombre de lettres que contient cette BAL = 0 au début
    //les pointeurs de cette nouvelle BAL sont tous NULL au début
    nouv->l_first=NULL;                     
    nouv->l_last=NULL;
    nouv->l_current=NULL;
    nouv->suiv=NULL;
    //cas où c'est la première BAL créé
    if (liste->first == NULL)  
    {
         liste->first = nouv ;
        liste->last = nouv ;
    }
    //cas où existe déjà d'autres BAL, la nouvelle créé s'ajoute au dernier
    else
    {
        liste->last->suiv= nouv ;
        liste->last=nouv ;
    }
    liste->nb++;
}


//-------------------------------------------------//
//              Ajout d'une lettre dans une BAL     //
//-------------------------------------------------//

void add_LETTRE (int n, int lg, BAL* bal , char* mess)
{
    bal->nb=(bal->nb)+1;
	LETTRE* nouv;
	nouv=(LETTRE*)malloc(sizeof(LETTRE));
	nouv->num=n+1;
	nouv->lg=lg;
	nouv->suiv=NULL;

	if (bal->l_first==NULL)
	{
        bal->l_first=nouv;
        bal->l_last=nouv;
        bal->l_current=nouv;
    }

	else
    {
	    bal->l_last->suiv=nouv;
        bal->l_last=bal->l_last->suiv;
    }

    nouv->message=malloc(lg* sizeof(char));
    for (int i=0 ; i<lg ; i++)
        nouv->message[i] = mess[i];
}


//--------------------------------------------------//
//-----------Fonction qui permet de rouver la BAL---//
//--------------------------------------------------//

BAL* find_BAL(LISTE_BAL*liste, int num)
{
    BAL* bal=malloc(sizeof(struct BAL));
    liste->current=liste->first;
    if (liste->first==NULL)
    {
        add_BAL(num,liste);
        bal=liste->first;
            }
    else
    {
        liste->current=liste->first;

        if (liste->first==liste->last)
        {
            if (liste->first->num==num)
                bal=liste->current;
            else
            {
                add_BAL(num,liste);
                bal=liste->last;
            }
        }
        else if (liste->first->num==num)
            bal=liste->first;
        else
        {
            int var=0;
            while(var==0)
            {
                if (liste->current->suiv==NULL)
                    var=-1;
                else
                {
                    liste->current=liste->current->suiv;
                    if (liste->current->num==num)
                        var=1;
                    if (liste->current==NULL)
                        var=-1;
                }

            }
            if (var==1)
                bal=liste->current;
            else
            {
                add_BAL(num,liste);
                bal=liste->last;
            }
        }
    }
    return bal;
}


//----------------------------------------------------------------//
//------------Retourne -1 si BAL inexistante ou BAL Vide----------//
//----------------------------------------------------------------//

int find_BALR(LISTE_BAL*liste, int num)
{
    int ret;
    if (liste->first==NULL)
    {
        ret=-1;
    }
    else
    {
        liste->current=liste->first;
        if (liste->current==liste->last)
        {
            if (liste->current->num==num)
            {
                if (liste->current->l_first==NULL)
                    ret=-1;
                else
                    ret=liste->current->l_first->lg;
            }
            else
                ret=-1;
        }
        else if (liste->first->num==num)
        {
            if (liste->current->l_first==NULL)
                ret=-1;

            else
                ret=liste->current->l_first->lg;
        }
        else
        {
            int var=0;
            while(var==0)
            {
                if (liste->current->suiv==NULL)
                    var=-1;
                else
                {
                    liste->current=liste->current->suiv;
                    if (liste->current->num==num)
                        var=1;
                    if (liste->current==NULL)
                        var=-1;
                }

            }
            if (var==1)
            {
                if (liste->current->l_first==NULL)
                    ret=-1;
                else
                    ret=liste->current->l_first->lg;
            }
            else
                ret=-1;
        }

    }
    return ret;
}


//________________________________________________________//
//____________________Main________________________________//
//________________________________________________________//


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
	    
	    
	 
     
}}
return 0;
}



    
    
    
    
    
    
    
