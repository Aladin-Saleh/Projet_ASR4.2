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
#include "carte_manager.h"
#include <time.h>
#include "erreur.h"

/*Cette fonction vas crée la carte, elle devra être lancé dans le main*/
void cree_carte(struct carte* s_carte,char const *argv[],int argc)
{
    debug("DEBUG");
    s_carte->liste_ustencil = malloc(sizeof(int*) * sizeof(int*));
    s_carte->ustencil_pour_chaque_recette = malloc(sizeof(int**)*sizeof(int**));

    srand(time(NULL));
    int ind = 0;
    int buf = 0;
    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],0,10);
    printf("nombre d'ustencil : %d\nnombre specialité : %d \n",s_carte->nombre_ustencil,s_carte->nombre_specialite);
  
    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        *(s_carte->liste_ustencil+ind)= strtol(argv[i],0,10);
        ind++;
    }
    
    for (int i = 0; i < s_carte->nombre_specialite; i++)
    {
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
           buf = s_carte->liste_ustencil[j];
           printf("%d\n",s_carte->liste_ustencil[j]);
           *(s_carte->ustencil_pour_chaque_recette+i+j) = rand()%buf;
           printf("%ls\n",*(s_carte->ustencil_pour_chaque_recette+i+j));
            
        }
    }

  
}

/*Cette fonction permet l'affichage de la carte
* Il est important que la carte soit deja crée avant l'affichage (logique ¯\_(ツ)_/¯)
*/
void afficher_carte(struct carte* s_carte)
{
     for (int index_spec = 0; index_spec < s_carte->nombre_specialite; index_spec++)
    {
        printf("Spécialité n°%d : ",index_spec+1);
         for (int index_ustencil = 0; index_ustencil < s_carte->nombre_ustencil; index_ustencil++)
            {
                printf("ustencile n°%d = %ls/%d |",index_ustencil+1,*(s_carte->ustencil_pour_chaque_recette+index_spec+index_ustencil),*(s_carte->liste_ustencil+index_ustencil));
            }
            printf("\n");
    }
}
