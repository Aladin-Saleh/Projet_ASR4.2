struct carte
{
    
    int nombre_specialite;
    int nombre_ustencil;
    int *liste_ustencil;
    int **ustencil_pour_chaque_recette;
    int *ustencil_pour_chaque_recette2;
    
    
};

void afficher_carte(struct carte* s_carte);//Affichage de la carte
void cree_carte(struct carte* s_carte,char const *argv[],int argc);//Creation de la carte