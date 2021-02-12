/*Programme réaliser par Aladin SALEH & Paul MINGUET
**10/02/21
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
#include <time.h>
#include "./Header/carte_manager.h"
#include "./Header/types.h"
#include "./Header/erreur.h"


struct carte* s_carte;
int num_spe;
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

    
    sleep(1);
    printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);
    printf("nombre d'ustencile : %d\n",s_carte->nombre_ustencil);
    
  
    debug_info("Chargement de l'affichage de la carte...");
    sleep(3);
    afficher_carte(s_carte);
    couleur(BLANC);




    



    return 0;
}



