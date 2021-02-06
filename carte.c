/*Programme réaliser par Aladin SALEH & Paul MINGUET
**06/02/21
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
#include "carte.h"
#include <time.h>


carte* s_carte;
void erreur(char * texte_erreur);
void afficher_carte(carte* s_carte);
void cree_carte(carte* s_carte,char const *argv[],int argc,int nbr_ust);
int main(int argc, char const *argv[])
{
    key_t cle;
    int id;

    //s_carte->nombre_specialite = strtol(argv[1],NULL,10);

    if ((cle = ftok(FICHIER_CLE,'a')) == -1)
    {
        erreur("Erreur lors de la création de la clé...");
    }
    
    if((id = shmget(cle,sizeof(carte),0)) == -1)
    {
        erreur("Erreur id = -1 ...");
    }

    if((s_carte = (carte*)shmat(id,NULL,0)) == NULL)
    { 
        erreur("Erreur shmat (le segment n'a pas pu être associé)...");
    }
    
    
    
        sleep(5);
        printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);
    

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

/*Cette fonction vas crée la carte, elle devra être lancé dans le main*/
void cree_carte(carte* s_carte,char const *argv[],int argc,int nbr_ust)
{
    srand(time(NULL));
    int ind = 0;
    s_carte->nombre_ustencil = argc - 5;
    s_carte->nombre_specialite = strtol(argv[4],0,10);
    printf("nombre d'ustencil : %d\n",s_carte->nombre_ustencil);
  
    for (int i = argc - s_carte->nombre_ustencil; i < argc; i++)
    {
        *(s_carte->liste_ustencil+ind)= strtol(argv[i],0,10);
        ind++;
    }

    for (int i = 0; i < s_carte->nombre_specialite; i++)
    {
        for (int j = 0; j < s_carte->nombre_ustencil; j++)
        {
            int buf = *(s_carte->liste_ustencil+j);
            *(s_carte->ustencil_pour_chaque_recette+i+j) = rand()%buf;
            
        }
    }
}

/*Cette fonction permet l'affichage de la carte
* Il est important que la carte soit deja crée avant l'affichage (logique ¯\_(ツ)_/¯)
*/
void afficher_carte(carte* s_carte)
{
    printf("nombre de spécialité : %d\n",s_carte->nombre_specialite);
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



void erreur(char * texte_erreur)
{
    printf("%s\n",texte_erreur);
    exit(-1);
}
