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
            //s_carte->buf = rand()%liste_ustencil[s_carte->j];
            //printf("Main[%d][%d] : %d\n",i,j,s_carte->buf);
            //s_carte->i = i;
            //s_carte->j = j;
            //sleep(1);

            s_carte->spec_usten[i][j] = rand()%liste_ustencil[j];
       }
       
   }


  
}

/*Cette fonction permet l'affichage de la carte
* Il est important que la carte soit deja crée avant l'affichage (logique ¯\_(ツ)_/¯)
*/
void afficher_carte(struct carte* s_carte)
{   
     for (int i = 0; i < s_carte->nombre_specialite; i++)
    {
        printf("Spécialité n°%d a besoin de : ",i+1);
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
            printf("ustencile n°%d = %d |",j+1,s_carte->spec_usten[i][j]);
        }
        printf("\n");
        
    }
}


void cuisinier_prepare_specialite(struct carte* s_carte,int num_spe)
{
    printf("Le cuisinier prepare une spécialité...\nIl prend la spécialité %d\n",num_spe);
    for (int i = 0; i < s_carte->nombre_ustencil; i++)
    {
       printf("necessite %d ustencile %d \n",s_carte,s_carte->spec_usten[num_spe][i],i);
    }
    sleep(2);

}
