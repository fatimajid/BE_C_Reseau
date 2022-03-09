
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


