/*Programme réaliser par Aladin SALEH & Paul MINGUET
**06/02/21
**IUT Sénart Fontainebleau - Année 2
*/
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

int id;
//carte* s_carte; //= malloc(sizeof(carte) * sizeof(carte));
int continuer = 1;



struct carte * s_carte;
void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal
void erreur(char * texte_erreur);//Gere les erreurs

void afficher_carte(struct carte* s_carte);
void cree_carte(struct carte* s_carte,char const *argv[],int argc,int nbr_ust);

//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    key_t cle;
    srand(time(0));

   // s_carte = malloc(sizeof(carte) * sizeof(carte));
    //s_carte->liste_ustencil = malloc(sizeof(int*) * sizeof(int*));
    //s_carte->ustencil_pour_chaque_recette = malloc(sizeof(int**)*sizeof(int**));

    if (argc <= 5)
    {
       erreur("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>");
    }



    cle = ftok(FICHIER_CLE,'a');

    if (cle == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }
    printf("La a était clé crée avec succès...\n");
    sleep(1);//Il sert à rien ce sleep, mais je trouvais juste stylé à l'execution :) 


    
    if((id = shmget(cle,sizeof(struct carte),0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }
    printf("id de la mémoire partagée récuperer avec succès...\n");
    sleep(1);




    if((s_carte = (struct carte*)shmat(id,NULL,SHM_R)) == NULL)
    { 
        erreur("Erreur shmat (le segment n'a pas pu être associé)...");
    }
    printf("Le segment mémoire a été attaché...\n");
    sleep(1);
    
    cree_carte(s_carte,argv,argc,argc-5);
    afficher_carte(s_carte);


    
    sleep(1);
    printf("Envoi...\n");

   if (set_signal_handler(SIGINT,arreter_processus) != 0)
   {
       erreur("Erreur signal (set)...");
   }
    
    while (continuer)
    {

    }
    
    free(s_carte->ustencil_pour_chaque_recette);
    free(s_carte->liste_ustencil);
    free(s_carte);
  
   
    
    
    return 0;
}


void arreter_processus(int signal)
{
     if (shmdt((char*)s_carte) == -1)
    {
        perror("Error shmdt");
        exit(-1);
    }

    if (shmctl(id,IPC_RMID,NULL) == -1)
    {
        perror("Error shmclt : lors de la suppression du segment mémoire");
        exit(-1);
        
    }  
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





/*Cette fonction vas crée la carte, elle devra être lancé dans le main*/
void cree_carte(struct carte* s_carte,char const *argv[],int argc,int nbr_ust)
{
    srand(time(NULL));
    int ind = 0;
    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],0,10);
    printf("nombre d'ustencil : %d\n",s_carte->nombre_ustencil);
  
    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        s_carte->liste_ustencil[ind]= strtol(argv[i],0,10);
        ind++;
    }

    for (int i = 0; i < s_carte->nombre_specialite; i++)
    {
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
            int buf = s_carte->liste_ustencil[j];
            s_carte->ustencil_pour_chaque_recette[i][j] = rand()%buf;
            
        }
    }
}

/*Cette fonction permet l'affichage de la carte
* Il est important que la carte soit deja crée avant l'affichage (logique ¯\_(ツ)_/¯)
*/
void afficher_carte(struct carte* s_carte)
{
    printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);
    for (int index_spec = 0; index_spec < s_carte->nombre_specialite; index_spec++)
    {
        printf("Spécialité n°%d : ",index_spec+1);
         for (int index_ustencil = 0; index_ustencil < s_carte->nombre_ustencil; index_ustencil++)
            {
                printf("ustencile n°%d = %d/%d |",index_ustencil+1,s_carte->ustencil_pour_chaque_recette[index_spec][index_ustencil],s_carte->liste_ustencil[index_ustencil]);
            }
            printf("\n");
    }
}

