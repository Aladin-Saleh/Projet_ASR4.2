/*Programme réaliser par Aladin SALEH & Paul MINGUET
**09/02/21
**IUT Sénart Fontainebleau - Année 2
*/
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
#include <time.h>
#include "carte_manager.h"


struct carte* s_carte;
int main(int argc, char const *argv[])
{
    key_t cle;
    int id;
    //int **buf;
    struct carte* s_carte;

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
    
    
        sleep(5);
        printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);

    



    return 0;
}



