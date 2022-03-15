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
    
//----------------------------------------------------
//------------------PRINTBUFFER2---------------------- : Modifie la variable message pour écrire le buffer ----# avec # le numéro d'envoi
//----------------------------------------------------
void ecrire_buffer(int n , char*message)
{
	if (n>=100000)
    {
        printf("Trop de messages à envoyer (n>1000000 \n");
        exit(1);
    }

    if (n<10)
    {
        for (int i=0 ; i<4 ; i++)
        {
            *(message+i)='-';
        }
        //char nb[1];      
        *(message+4)= n + '0';
    }
    if (n>=10 & n<100)
    {
    	char nb[2];

        for (int i=0 ; i<3 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=100 & n<1000)
    {
        char nb[3];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=1000 & n<10000)
    {
    	int j=1000;

        char nb[4];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }

        sprintf(nb,"%d",n);
        *(message+1)=*nb;
        n=n%1000;
        sprintf(nb,"%d", n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
    }
    if (n>=10000 & n<100000)
    {
        char nb[4];
        for (int i=0 ; i<2 ; i++)
        {
            *(message+i)='-';
        }
        sprintf(nb,"%d",n);
        *(message)=*nb;
        n=n%10000;
        sprintf(nb,"%d", n);
        *(message+1)=*nb;
        n=n%1000;
        sprintf(nb,"%d", n);
        *(message+2)=*nb;
        n=n%100;
        sprintf(nb,"%d", n);
        *(message+3)=*nb;
        n=n%10;
        sprintf(nb,"%d", n);
        *(message+4)=*nb;
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
    bal->nb=(bal->nb)+1;                            //mise à jour du nbr de lettres existantes dans la BAL
	LETTRE* nouv;                                   //pointeur de la nouvelle lettre ajoutée
	nouv=(LETTRE*)malloc(sizeof(LETTRE));           //Allocation mémoire de la nouvelle lettre
	nouv->num=n+1;      //le numéro de la nouvelle lettre 
	nouv->lg=lg;        //la longueur de la nouvelle lettre
	nouv->suiv=NULL;    //le pointeur tail (queue) NULL
    
    //cas où c'est la première lettre dans la BAL_i
	if (bal->l_first==NULL)
	{
        bal->l_first=nouv;
        bal->l_last=nouv;
        bal->l_current=nouv;
    }
    
    //cas où il existe déjà d'autres lettres dans la meme BAL
	else
    {
	    bal->l_last->suiv=nouv;
        bal->l_last=bal->l_last->suiv;
    }
    
    
    nouv->message=malloc(lg* sizeof(char)); //Allocation mémoire du message de la nouvelle lettre
    for (int i=0 ; i<lg ; i++)
        nouv->message[i] = mess[i];     //corespondance du message
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

//-----------------------------------------------------------//
//-------------------Service EMETTEUR------------------------//
//-----------------------------------------------------------//

void EBAL(int port, char* dest, int nb_message, int lg_msg, int nBAL)
{
    //Déclarations
    int sock;
    struct sockaddr_in addr_distant ;
    int lg_addr_distant=sizeof(addr_distant);
    struct hostent *hp;
    char motif;
    char * message=malloc(lg_msg*sizeof(char));
    int envoi=-1;
    int lg_pdu=50;
    int lg_recv;
    char*pdu=malloc(lg_pdu*sizeof(char));

//---------------------------------------
//--------Etablissement connexion--------
//---------------------------------------
    printf("            SOURCE : Emission de lettres pour la BAL n°%d\n",nBAL);
    printf("____________________________________________________________________\n\n");

    sprintf(pdu,"0 %d %d %d",nBAL, nb_message,lg_msg);

    //Création socket
        if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("Erreur à l'ouverture du Socket Stream");
        exit(1);
    }
    //Construction adresse socket distant
    memset((char*)&addr_distant,0,sizeof(addr_distant));
    addr_distant.sin_family=AF_INET;  //Internet
    addr_distant.sin_port=port;       //Numéro de Port

    //Affectation IP
    if((hp=gethostbyname(dest))==NULL)
    {
        printf("Erreur de requête IP.\n");
        exit(1);
    }

    memcpy((char*)&(addr_distant.sin_addr.s_addr), hp->h_addr , hp->h_length);

    //Demande de connexion

    if (connect(sock,(struct sockaddr *)&addr_distant,sizeof(addr_distant))==-1)
    {
        printf("Erreur lors de la connexion, attente de l'essaie suivant \n");
        exit(1);
    }
//-----------------------------------------
//----------------Envoi PDU----------------
//-----------------------------------------

    if ((envoi=write(sock,pdu,lg_pdu))==-1)
    {
        printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
        exit(1);
    }
//-----------------------------------------
//----------TRANSFERT DE DONNEES-----------
//-----------------------------------------



    for (int i=1; i<=nb_message;i++)
    {
        printf("SOURCE : lettre n°%d (%d) [", i,lg_msg);

        //Création du message
        construire_message(message,motif,lg_msg,i);
        ecrire_buffer(nBAL,message);
        afficher_message(message,lg_msg);

        //Envoi du message

        if ((envoi=write(sock,message,(lg_msg)/*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/))==-1)
        {
            printf("Echec de l'envoi du message \n");
            exit(1);
        }

    }



    //Fermeture connexion

    if(shutdown(sock,2)==-1)
    {
        printf("Erreur à la fermeture de la connexion TCP \n");
        exit(1);
    }

    //Fermeture Socket
    if (close(sock)==-1)
    {
        printf("Echec de la fermeture du socket distant");
        exit(1);
    }

    free(message);
    free(pdu);
    printf("Envoi avec succès\n");
}

//--------------------------------------------------------//
//-------------------Service BAL--------------------------//
//--------------------------------------------------------//

void SBAL(int port, char*dest){
    //Déclarations
    int sock , sock2; //sock bis local orienté échanges
    struct sockaddr* addr_distant;
    struct sockaddr_in addr_local;
    int lg_addr_distant=sizeof(addr_distant);
    int lg_addr_local=sizeof(addr_local);
    struct hostent *hp;
    char motif;
    char *message;
    int lg_recv=-1;
    int lg_sent=-1;
    int lg_pdu=50;
    int type=-1;
    int nb;
    int lg;
    int n=1;
    int nBAL;
    BAL*bal=malloc(sizeof(struct BAL));
    char *pdu;//=malloc(sizeof(char));
    LISTE_BAL* liste;
//----------------------------------
//------------Connexion ------------
//----------------------------------

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
    addr_local.sin_port=port;

    //Bind
    if (bind(sock,(struct sockaddr *)&addr_local, lg_addr_local)==-1)
    {
        printf("Echec du bind.\n");
        exit(1);
    }
    //Check connexions entrantes
    if (listen(sock,100)<0)
    {
        printf("Trop de connexions en attentes, échec de la demande\n");
        exit(1);
    }
    liste=init_BAL();
    while (1)
    {
        if ((sock2=accept(sock,(struct sockaddr*)&addr_distant,&lg_addr_distant))==-1)
        {
            printf("Refus de connexion par le serveur\n");
            exit(1);
        }

        pdu=malloc(50*sizeof(char));
         if((lg_pdu=read(sock2,pdu, lg_pdu))<0)
        {
            printf("Echec de lecture du PDU entrant\n");
            exit(1);
        }
        sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
        //GESTION EMETTEUR
        if (atoi(pdu)==0)//(atoi(pdu)==-1) 
        {
            printf("        ||||||| Réception des lettres pour la BAL n°%d |||||\n\n",nBAL);
            message=malloc(lg*sizeof(char));
            int n=0;
            sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
            bal=find_BAL(liste,nBAL);

            while (n!=nb)
            {
                message = malloc(lg* sizeof(char));
                if ((lg_recv = read(sock2, message, lg)) == -1)
                {
                    printf("Erreur de lecture\n");
                    exit(1);
                }
                if (lg_recv>0)
                {
                    add_LETTRE(n,lg, bal,message);
                }
                n++;
            }

            printBAL(bal,lg);
        }
        //GESTION RECEPTEUR
        else if (atoi(pdu)==1)
        {
            sscanf(pdu, "%d %d", &type, &nBAL);
            printf("        ||||||| Restitution des lettres de la BAL n°%d |||||||\n\n",nBAL);
            lg=find_BALR(liste,nBAL);
            if (lg==-1) // Gestion du cas ou la BAL est vide, on envoie un PDU qui sera analysé par le récepteur.
            {
                printf("        BAL inexistante, PDU=0 pour informer le récepteur\n\n");
                sprintf(pdu,"%d %d",lg,nb);
                //printf ("PDU à envoyer : %d\n",lg);
                int lg_sent=-1;
                nb=1;
                if ((lg_sent=write(sock2,pdu,lg_pdu))==-1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
                {
                    printf("Echec de l'envoi du PDU \n");
                    exit(1);
                }
            }
            else
            {
                bal=find_BAL(liste,nBAL);
                bal->l_current=bal->l_first;

                while(bal->l_current!=NULL)
                {
                    lg=bal->l_current->lg;
                    nb=bal->nb;
                    sprintf(pdu,"%d %d",lg,nb);

                    if ((lg_sent=write(sock2,pdu,lg_pdu))==-1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
                    {
                        printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
                        exit(1);
                    }
                    message=malloc(lg*sizeof(char));
                    message=bal->l_current->message;

                    if ((lg_sent=write(sock2,message,lg))==-1)
                    {
                        printf("Erreur lors de l'envoi du message n°%d\n",n);
                        exit(1);
                    }
                    printf("BAL n°%d : Restitution de la lettre n°%d (%d) [",nBAL,n,lg);
                    afficher_message(message,lg);
                    bal->l_current=bal->l_current->suiv;
                    n++;
                }
                
                if ((shutdown(sock2 , 2))==-1)
                {
                    printf("Erreur à la fermeture de la connexion : shutdown\n");
                    exit(1);
                }
            }
        }
        else
        {
            printf("PDU non reconnu, on quitte par sécurité\n");
            exit(1);
        }
        printLISTE(liste);
        free(pdu);
        free(message);
    }
}

//--------------------------------------------------------//
//------------------Service RECEPTEUR---------------------//
//--------------------------------------------------------//

void RBAL(int port, char* dest, int nBAL){
    printf("+++ Debug 1 \n");

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
    //Variable qui donne la longueur max d'un message
    int maxsize=9999;
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
    
    if(close(sock)==-1)
    {
        printf("Impossible de fermer le socket");
        exit(1);
    }
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
	int bal=-1;     /* 1 si l'on utilise le service BAL ; 0 si l'on utilise le service classique*/
	int recep=-1;   /* 0 si l'on est emetteur et 1 si l'on est recepteur*/
	int nb_lettres=3;
    int nBAL=-1;
	int lg =30;
	int maxsize=9999;
	
	while ((c = getopt(argc, argv, "pn:sul:be:r:")) != -1) { //Ajout de l'ption 'u' dans le programme
	
		switch (c) {
		
		case 'p':   //option du puit
			if (source == 1) { 
			    //erreur on ne peut pas etre source et puit en meme temps
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0; // on remet la source à 0 pour utiliser le puit
			break;

		case 's':   //option de la source
			if (source == 0) {
			    //erreur on ne peut pas etre source et puit en meme temps
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1; // on remet la source à 1 pour utiliser la source
			break;
			
	    case 'b' :  // option du BAL
                if (source!=-1)         //Test: service classique ( -s|-p ) ou service du BAL
                {
                    printf("erreur :\n On ne peut pas utiliser le service de Boite aux lettres et l'usage classique de TCP/UDP en même temps.\n Utiliser l'une des options -b ou -s|-p\n\n");
                    exit(1);
                }
                bal=1;  // on remet le service BAL à 1 pour l'utiliser
                break;
                
            case 'e' :  // option de l'emetteur
                if (source!=-1)                 // Test: service classique ( -s|-p ) ou service du BAL
                {
                    printf("ATTENTION :\nOn ne peut utiliser le service de Boite aux lettres et l'usage classique de TCP/UDP en même temps.\nUtiliser l'une des options -e ou -s|-p\n\n");
                    exit(1);
                }
                
                //Test: service emetteur ou recepteur
                if (recep==1)   // 0 pour emetteur et 1 si l'on est recepteur
                {
                    printf("On ne peut pas être émetteur et récepteur, veuillez utiliser -r ou -e\n");
                    exit(1);
                }
                bal=1;      // on remet le bal à 1 pour utiliser le service du BAL
                recep=0;    // on remet le recep à 0 pour travailler étant emetteur
                nBAL = atoi(optarg);
                break;
                
            case 'r' :  // option du recepteur
                //Test: service emetteur ou recepteur
                if (recep==0)   // 0 pour emetteur et 1 si l'on est recepteur
                {
                    printf("On ne peut être émetteur et récepteur en même temps, veuillez utiliser -r ou -e\n");
                    exit(1);
                }
                bal=1;      // on remet le bal à 1 pour utiliser le service du BAL
                recep=1;    // on remet le recep à 0 pour travailler étant emetteur
                nBAL = atoi(optarg);
                break;		
	    
		//Ajout de l'option u du protocole UDP
        case 'u':
            proto = 0;
            printf("usage de protocole udp");
             
            break;
        
        //l'option qui controle le nbr de messgs à envoyer
		case 'n':
			
      		nb_message = atoi(optarg);
      		//printf("%s\n",optarg);
      		//printf("%d\n",nb_message);
            if (nb_message>100000)
            {
              printf("Trop d'envois, n> 100 000\n");
              exit (1);
            }
            if (recep==1)
                {
                    printf("NB :\nVous devez récupérer l'intégralité du contenu de la BAL. Vous ne pouvez donc pas fixer n\n\n");
                    exit(1);
                }
      		break;
        
        //l'option qui controle la longueur des messages à envoyer
        case 'l':
            lg_message = atoi(optarg);
            if (lg>maxsize)
                {
                    printf("Taille des messages supérieure à la limite fixée à %d\n",maxsize);
                }
                if (recep==1)
                {
                    printf("NB :\nVous devez récupérer l'intégralité du contenu de la BAL. Vous ne pouvez donc pas fixer l\n\n");
                    exit(1);
                }
          break;
          
		default:
			printf("usage: cmd [-p|-s] ou [-b|-e|-r] [-n ##]\n");
			break;
		}
	}
    
    
    // la gestion du port
    int port = atoi(argv[argc-1]);
    port =htons(port);
    
    if (port!=-1)
    {
        port=htons(port);
    }
    
    // la destination 
    char *dest ;
    dest=argv[argc-2];
    
    
/* 	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]++++\n");
		exit(1) ;
	} */

//------------------------------------------------------//
//-----UTilisation du service classique / Partie 1------//
//------------------------------------------------------//

    if (bal == -1)          // on utilise le service classique
    {
    //cas où l'on est source
	if (source == 1) {
		printf("on est dans la source\n");
		//char * destination = argv[argc-2];
	 } else 
	    // cas où l'on est puit
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
	  
	 //source udp 
	 if(source == 1 && proto == 0){
	  source_udp(port, dest, nb_message, nb_message); }
	 //puit udp 
	 else if (source =0 && proto == 0){ 
	    puit_udp(port, nb_message, lg_message);
	    }
	 //source tcp   
	 if(source = 1 && proto == 1){ 
	    source_tcp(port, dest, nb_message, nb_message);
	    }    
	 //puit tcp   
     else if(source =0 && proto == 1){ 
	    puit_tcp(port,nb_message, lg_message);
	    }
     else
     {printf("erreur de configuration\n");
     exit(1);}
	    
	    
	 
     
}}}

//-----------------------------------------------------------//
//--------------Utilsation du service du BAL / Partie 2------//
//-----------------------------------------------------------//

else // bal != -1 / 
    {
        if (nb_message == -1)  //Attribution du nombre de message à envoyer/recevoir si absence de -n XX
        {
            if (recep == 0)         // recep = 0 => EMETTEUR
                nb_message = 10;    //nbr de message à envoyer = 10

        }
        if (bal==1 &recep==-1)  // recep = -1 => par défaut BAL 
        {
            printf("____________________________________________________________________\n\n");
            printf("                             SERVEUR BAL                            \n");
            printf("____________________________________________________________________\n\n");
            SBAL(port, dest);
        }
        
        else if (bal==1 & recep ==0)    // recep = 0 => Service EMETTEUR
        {
            printf("____________________________________________________________________\n\n");
            printf("                             EMETTEUR BAL                           \n");
            printf("____________________________________________________________________\n\n");
            EBAL(port, dest,nb_message,lg, nBAL);
        }

        else if(bal==1 &recep == 1)     // recep = 1 => Service Recepteur
        {
            printf("____________________________________________________________________\n\n");
            printf("                            RECEPTEUR BAL                           \n");
            printf("____________________________________________________________________\n\n");
            RBAL(port,dest,nBAL);
        }
    }
    
return 0;
}



    
    
    
    
    
    
    
