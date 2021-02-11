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
#include "types.h"
#include <time.h>
#include "carte_manager.h"
#include "erreur.h"


struct carte* s_carte;
int main(int argc, char const *argv[])
{
    key_t cle;
    int id;
    int array_spec[100][100];//Des que l'on aura mis l'exec multiplier 100 par le nombre de spécialité
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
    printf("nombre d'ustencile : %d\nset : %d\n",s_carte->nombre_ustencil,s_carte->set);
    
    while(s_carte->set){
    printf("BUF : %d \n",s_carte->buf);
    //printf("i : %d \n",s_carte->i);
    //printf("j : %d \n",s_carte->j);
    array_spec[s_carte->i][s_carte->j] = s_carte->buf;
    //sleep(1);
    }
    
    debug_info("Chargement de l'affichage de la carte...");
    sleep(3);
    afficher_carte(s_carte,array_spec);
    printf("Le client commande une spécialité...\nIl prend la spécialité %d\n",s_carte->test);
    for (int i = 0; i < s_carte->nombre_ustencil; i++)
    {
       printf("necessite %d ustencile %d \n",array_spec[s_carte->test][i],i);
    }
    

    



    return 0;
}



