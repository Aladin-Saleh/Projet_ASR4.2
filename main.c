#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FICHIER_CLE "cle.serv"

void arreter_processus(int signal);
int set_signal_handler(int sig, void (*handler)(int));

//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    
    key_t cle;

    cle = ftok(FICHIER_CLE,'a');

   if (set_signal_handler(SIGINT,arreter_processus) != 0)
   {
       perror("Erreur signal (set)...");
       exit(-1);
   }
   

    if (argc <= 5)
    {
        printf("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>\n");
        exit(-1);
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









