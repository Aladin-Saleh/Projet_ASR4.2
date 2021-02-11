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
#include "carte_manager.h"
#include <time.h>
#include "erreur.h"

/*Cette fonction vas crée la carte, elle devra être lancé dans le main*/
void cree_carte(struct carte* s_carte,int* liste_ustencil)
{
       debug_info("Les données sont envoyé pour créée la carte...");
    sleep(2);
    debug_info("Chargement...");
   for (int i = 0; i <=  s_carte->nombre_specialite; i++)
   {
       for (int j = 0; j <= s_carte->nombre_ustencil; j++)
       {
            s_carte->buf = rand()%liste_ustencil[s_carte->j];
            //printf("Main[%d][%d] : %d\n",i,j,s_carte->buf);
            s_carte->i = i;
            s_carte->j = j;
            //sleep(1);
       }
       
   }

  
}

/*Cette fonction permet l'affichage de la carte
* Il est important que la carte soit deja crée avant l'affichage (logique ¯\_(ツ)_/¯)
*/
void afficher_carte(struct carte* s_carte,int array_spec[100][100])
{   
     for (int i = 0; i < s_carte->nombre_specialite; i++)
    {
        printf("Spécialité n°%d a besoin de : ",i+1);
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
             printf("ustencile n°%d = %d |",j+1,array_spec[i][j]);
        }
        printf("\n");
        
    }
}
