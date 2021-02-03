#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "types.h"
#include "carte.h"
#include <time.h>


struct carte* s_carte;
void afficher_carte(struct carte* s_carte);

int main(int argc, char const *argv[])
{
    key_t cle;
    int id;

    //s_carte->nombre_specialite = strtol(argv[1],NULL,10);

    if ((cle = ftok(FICHIER_CLE,'a')) == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }
    
    if((id = shmget(cle,sizeof(struct carte),0)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }

    if((s_carte = (struct carte*)shmat(id,NULL,0)) == NULL)
    { 
        erreur("Erreur shmat (le segment n'a pas pu être associé)...");
    }
    
    while(1)
    {
        sleep(5);
        printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);
    }

      if (shmdt((char*)s_carte) == -1)
    {
        perror("Error shmdt");
        exit(-1);
    }
    /*On supprime le segment de mémoire*/
    /*shmctl permet de gerer les action sur la memoire partagé*/
    /*int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf)*/
    if (shmctl(id,IPC_RMID,NULL) == -1)
    {
        perror("Error shmclt : lors de la suppression du segment mémoire");
        exit(-1);
        
    }


    return 0;
}
/*
struct carte
{
    int nombre_specialite;
    int **nombre_ustencil;
	int nombre_ustencil_categorie[];

};
*/
void creation_carte(int nbr_spclt)
{
    srand(time(NULL));
    




}




void afficher_carte(struct carte* s_carte)
{


    for (int index_spec = 0; index_spec < strtol(s_carte->nombre_specialite,0,10); index_spec++)
    {
        printf("Spécialité n°%d : \n\n\n",index_spec+1);
         for (int index_ustencil = 0; index_ustencil < s_carte->nombre_ustencil; index_ustencil++)
            {
                printf("ustencile n°%d - nombre nécessaire d'ustencil = %d/%d  ,",index_ustencil+1,0,s_carte->nombre_ustencil_dispo_categorie[index_ustencil]);
            }
            printf("\n");
    }
}