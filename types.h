typedef struct {
	int type;	//1 pour client, 2 pour cuisinier
	int serveur;
	int choix;
	pid_t expediteur;
} 
command_t;

typedef struct {
	int type;	//3 pour serveur, 2 pour cuisinier
	int choix;
	pid_t expediteur;
}
commandcuiserv_t;

typedef struct {
	int type;
	int choix;
} 
commandcliserv_t;



#define FICHIER_CLE "cle.serv"
