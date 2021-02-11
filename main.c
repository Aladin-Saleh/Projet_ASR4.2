/*Programme réaliser par Aladin SALEH & Paul MINGUET
**10/02/21
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
#include "carte_manager.h"
#include "types.h"
#include "erreur.h"

int id;
int continuer = 1;
struct carte * s_carte;
struct shmid_ds shmid_ds;
void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal


//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    key_t cle;
    int ind = 0;

    srand(time(0));
    s_carte = malloc(sizeof(struct carte) * sizeof(struct carte));
    

    if (argc <= 5)
    {
       erreur("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>");
    }



    cle = ftok(FICHIER_CLE,'a');

    if (cle == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }
    debug_succes("La a était clé crée avec succès...");
    sleep(1);//Il sert à rien ce sleep, mais je trouvais juste stylé à l'execution :) 


    
    if((id = shmget(cle,sizeof(struct carte),0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }
    debug_succes("id de la mémoire partagée récuperer avec succès...");
    sleep(1);

    if ((s_carte = (struct carte*)shmat(id,NULL,0)) == (void*)-1)
    {
        erreur("Erreur lors de la jointure du segment de mémoires...");
    }
    debug_succes("La jointure du segment mémoire a été faites...");
    
    int liste_ustencil[argc-5];
    s_carte->set = 1;
    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],0,10);

    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        liste_ustencil[ind]= strtol(argv[i],0,10);
        ind++;
    }

    cree_carte(s_carte,liste_ustencil);
    
    s_carte->set = 0;





  

    
    sleep(1);
    debug_succes("Element envoyer !");
    debug_succes("Création de la carte terminer !");

    s_carte->test = 0;

   if (set_signal_handler(SIGINT,arreter_processus) != 0)
   {
       erreur("Erreur signal (set)...");
   }
    
  
    while (continuer)
    {
       
        
    }

    
    
    return 0;
}



void arreter_processus(int signal)
{

    if (shmdt((char*)s_carte) == -1)
    {
        erreur("Error shmdt");
        exit(-1);
    }

    if (shmctl(id,IPC_RMID,NULL) == -1)
    {
        erreur("Error shmclt : lors de la suppression du segment mémoire");
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

