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
#include <sys/msg.h>
#include <assert.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "./Header/carte_manager.h"
#include "./Header/types.h"
#include "./Header/erreur.h"

int id;
int continuer = 1;
int *file_mess;
int nbServ;
int id_sem;
struct carte * s_carte;
struct shmid_ds shmid_ds;
void arreter_processus(int signal);//Arrete tout les processus à la réception du signal
int set_signal_handler(int sig, void (*handler)(int));//Set le signal

struct sembuf p[] = {{ 0, -1, SEM_UNDO}};
struct sembuf v[] = {{ 0, +1, SEM_UNDO}};



//argument nb_serveurs nb_cuisiniers nb_term nb_spec nb_1 nb_2 ... nb_k
int main(int argc, char const *argv[])
{
    key_t cle;
    int ind = 0;
    nbServ = (int) strtol((argv[1]), NULL, 0);
    int nbCui = (int) strtol((argv[2]), NULL, 0);
    //int nbTerm = (int) strtol((argv[3]), NULL, 0);
    char *fichierCle;
    char cleVal = 'b';
    char *tmp = NULL;
    int shmid;
    char buffer[10];

    //s_carte = malloc(sizeof(struct carte) * sizeof(struct carte));
    
    if (argc <= 5)
    {
       erreur("Usage : <nb_serveurs> <nb_cuisiniers> <nb_term> <nb_spec> <nb_1> <nb_2> ... <nb_k>");
    }

    if (strtol(argv[1],NULL,10) <= strtol(argv[3],NULL,10))
    {
        erreur("Le nombre de terminal doit être inferieur au nombre de serveur");
    }

    cle = ftok(FICHIER_CLE,'a');

    if (cle == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }
    debug_succes("La clé a été créée avec succès...");
    sleep(1);//Il sert à rien ce sleep, mais je trouvais juste stylé à l'execution :) 
    
    if((id = shmget(cle,sizeof(struct carte),IPC_CREAT|0666)) == -1)
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

    if ((id_sem = semget(cle,1,0666|IPC_CREAT)) == -1)
    {
        erreur("Erreur lors de la creation de la semaphore...");
    }
    
    int liste_ustencil[argc-5];
    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],0,10);

    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        liste_ustencil[ind]= strtol(argv[i],0,10);
        ind++;
    }

    cree_carte(s_carte,liste_ustencil);
    sleep(4);

    key_t k = ftok("/tmp",1);
    assert(k!=-1);

    sleep(1);
    debug_succes("Element envoyer !");
    debug_succes("Création de la carte terminer !");

       pid_t p = fork();
        assert( p != -1);

        if (p==0) {
            		execl("./carte","./carte",NULL,NULL);

            assert(0);
        }
   

    sleep(5);    

    shmid=shmget(k, sizeof(int), IPC_CREAT|0666);
    assert(shmid >= 0);

    system("touch cle.serv");

    file_mess = (int *)malloc((nbServ+1) * sizeof(int));
    fichierCle = (char * )malloc((nbServ+1) * sizeof(char));
    tmp = (char *)malloc((nbServ+1) * sizeof(char));

    file_mess[0] = msgget(cle, IPC_CREAT | 0666);

    for(int i = 1; i <= nbServ; i++) {
        sprintf(fichierCle, "t%c.serv", cleVal);
        sprintf(tmp, "touch t%c.serv", cleVal);
        system(tmp);
        cle = ftok(fichierCle, cleVal);
        file_mess[i] = msgget(cle, IPC_CREAT | 0666);
        cleVal++;
    }



    for (int i = 1; i <= 10; i++) {
        pid_t p = fork();
        assert( p != -1);

        if (p==0) {
            snprintf(buffer, sizeof(buffer), "%d", s_carte->nombre_specialite);
            execl("./client", "./client", buffer, NULL);
            assert(0);
        }
    }

    printf("Les clients ont été créés\n");

    for (int i = 1; i <= nbServ; i++) {
        pid_t p = fork();
        assert( p != -1);

        if (p==0) {
            snprintf(buffer, sizeof(buffer), "%d", i);
            execl("./serveur", "./serveur", buffer, NULL);
            assert(0);
            sleep(1);
        }
    }

    printf("Les serveurs ont été créés\n");

    for (int i = 1; i <= nbCui; i++) {
        pid_t p = fork();
        assert( p != -1);

        if (p==0) {
            snprintf(buffer, sizeof(buffer), "%d", i);
            execl("./cuisinier", "./cuisinier", buffer, NULL);
            assert(0);
        }
    }

    printf("Les cuisiniers ont été créés\n");

    if (set_signal_handler(SIGINT,arreter_processus) != 0)
    {
        erreur("Erreur signal (set)...");
    }

    while(1){}
    
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
    
    for(int i = 0; i <= nbServ; i++) {
        if (msgctl(file_mess[i], IPC_RMID, NULL) == -1) {
            erreur("Error shmclt : lors de la suppression des files");
            exit(-1);
        }
    }

    if(semctl(id_sem, IPC_RMID, 0) == -1) {
        
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

