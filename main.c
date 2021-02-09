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
#include "carte_manager.h"
#include "signal.h"
#include "types.h"
#include "erreur.h"

int id;
int continuer = 1;
struct carte * s_carte;
void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal


//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    key_t cle;
    
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
    printf("La a était clé crée avec succès...\n");
    sleep(1);//Il sert à rien ce sleep, mais je trouvais juste stylé à l'execution :) 


    
    if((id = shmget(cle,sizeof(struct carte),0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }
    printf("id de la mémoire partagée récuperer avec succès...\n");
    sleep(1);
  
    
    cree_carte(s_carte,argv,argc);
    //afficher_carte(s_carte);

    
    sleep(1);
    printf("Envoi...\n");

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
    continuer = 0;
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

