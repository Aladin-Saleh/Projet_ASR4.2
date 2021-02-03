#include <stdio.h>
#include <stdlib.h>
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
#include "erreur.h"


void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal

//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    struct carte* s_carte;
    key_t cle;
    int id;
    //int nombre_ust_req = 0;
   

    if (argc <= 5)
    {
       erreur("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>");
    }

    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],NULL,10);
    for (int i = s_carte->nombre_ustencil; i < argc; i++)
    {
        s_carte->nombre_ustencil_dispo_categorie[i] = strtol(argv[i],NULL,10);
    }
    
    afficher_carte(s_carte);    

    


    cle = ftok(FICHIER_CLE,'a');

    if (cle == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }

    if((id = shmget(cle,sizeof(struct carte),0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }

    if((s_carte = (struct carte*)shmat(id,NULL,SHM_R)) == NULL)
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









