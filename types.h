typedef struct {
	int serveur;
	int choix;
	pid_t expediteur;
} 
command_t;

typedef struct {
	int type;
	int choix;
} 
cmdRecue_t;


#define FICHIER_CLE "cle.serv"

struct carte
{
    int nombre_specialite;
    int nombre_ustencil;
	int nombre_ustencil_categorie[];

};


void erreur(char * texte_erreur);//Gère les erreurs

void erreur(char * texte_erreur)
{
    printf("%s\n",texte_erreur);
    exit(-1);
}
