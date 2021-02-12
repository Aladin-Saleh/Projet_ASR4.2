struct carte
{
    
    int nombre_specialite;
    int nombre_ustencil;
    int j;
    int i;
    int buf;
    int set;
    int test;
    
};
void afficher_carte(struct carte* s_carte,int array_spec[100][100]);//Affichage de la carte
void cree_carte(struct carte* s_carte,int* liste_ustencil);//Creation de la carte