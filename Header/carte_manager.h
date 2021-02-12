struct carte
{
    
    int nombre_specialite;
    int nombre_ustencil;
    int spec_usten[512][512];
    
};
void afficher_carte(struct carte* s_carte);//Affichage de la carte
void cree_carte(struct carte* s_carte,int* liste_ustencil);//Creation de la carte
void cuisinier_prepare_specialite(struct carte* s_carte,int num_spe);
