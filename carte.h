
typedef struct
{
    
    int nombre_specialite;
    int nombre_ustencil;
    int *liste_ustencil;
    int **ustencil_pour_chaque_recette;
    
}
carte;

void afficher_carte(carte* s_carte);
void cree_carte(carte* s_carte,char const *argv[],int argc,int nbr_ust);