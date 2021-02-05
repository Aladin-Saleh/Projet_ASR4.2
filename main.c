#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "carte.h"
#include "types.h"


void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal
void erreur(char * texte_erreur);//Gere les erreurs

void cree_carte(carte* s_carte,char const *argv[],int argc,int nbr_spec,int nbr_ust)
{
    //int ind = 0;
    //int ust_necsr_spe = 0;
    s_carte->nombre_ustencil = argc - 5;

    printf("%d\n",s_carte->nombre_ustencil);
  
    //ust_necsr_spe = rand()%strtol(argv[i],0,10);
   

    //s_carte->liste_ustencil = malloc(sizeof(int*) * sizeof(int*));
    //s_carte->ustencil_pour_chaque_recette = malloc(sizeof(int**)*sizeof(int**));

    for (int i = 0; i < nbr_spec; i++)
    {
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
            printf("- %d\n",rand()%*(s_carte->liste_ustencil+j));
            *(s_carte->ustencil_pour_chaque_recette+i+j) = 5;//rand()%*(s_carte->liste_ustencil+j);

        }
        

    }
    
    
}

void afficher_carte(carte* s_carte)
{
    srand(time(NULL));
    int ust_req;

    for (int index_spec = 0; index_spec < s_carte->nombre_specialite; index_spec++)
    {
        printf("Spécialité n°%d : ",index_spec+1);
         for (int index_ustencil = 0; index_ustencil < s_carte->nombre_ustencil; index_ustencil++)
            {
                //ust_req = rand()%*(s_carte->liste_ustencil+index_ustencil);
                printf("ustencile n°%d = %d/%d |",index_ustencil+1,*(s_carte->ustencil_pour_chaque_recette+index_spec+index_ustencil),*(s_carte->liste_ustencil+index_ustencil));
            }
            printf("\n");
    }
}

//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    carte* s_carte = malloc(sizeof(carte) * sizeof(carte));
    key_t cle;
    int id;
    int ind = 0;
   srand(time(0));

    if (argc <= 5)
    {
       erreur("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>");
    }

    int nombre_specialite = strtol(argv[4],0,10);
    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        *(s_carte->liste_ustencil+ind)= strtol(argv[i],0,10);
        ind++;
    }
    s_carte->liste_ustencil = malloc(sizeof(int*) * sizeof(int*));
    s_carte->ustencil_pour_chaque_recette = malloc(sizeof(int**)*sizeof(int**));

    cree_carte(s_carte,argv,argc,nombre_specialite,argc-5);
    afficher_carte(s_carte);
    
    free(s_carte);
    free(s_carte->liste_ustencil);
    
    cle = ftok(FICHIER_CLE,'a');

    if (cle == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }

    if((id = shmget(cle,sizeof(carte),0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }

    if((s_carte = (carte*)shmat(id,NULL,SHM_R)) == NULL)
    { 
        erreur("Erreur shmat (le segment n'a pas pu être associé)...");
    }

    s_carte->nombre_specialite = 0;
    
    while (1)
    {
        //s_carte.nombre_specialite = strtol(argv[4],0,10);
    }
    printf("---\n");

   if (set_signal_handler(SIGINT,arreter_processus) != 0)
   {
       erreur("Erreur signal (set)...");
   }
   
  
   
    
    
    return 0;
}


void arreter_processus(int signal)
{
    printf("Signal d'arret recu ! \n");
    exit(EXIT_SUCCESS);
}

int set_signal_handler(int sig, void (*handler)(int))
{
    struct sigaction sgnl;
    sgnl.sa_handler = arreter_processus;
    sigemptyset(&sgnl.sa_mask);
    sgnl.sa_flags = 0;
    return sigaction(sig,&sgnl,NULL);
}



void erreur(char * texte_erreur)
{
    printf("%s\n",texte_erreur);
    exit(-1);
}







