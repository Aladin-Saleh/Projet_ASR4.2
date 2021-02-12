typedef struct {
	long type;	//1 pour client, 2 pour cuisinier
	int choix;
	pid_t expediteur;
} 
command_t;

typedef struct {
	long type;	//3 pour serveur, 2 pour cuisinier
	int choix;
	int client;
	pid_t expediteur;
}
commandcuiserv_t;

typedef struct {
	long type;
	int choix;
} 
commandcliserv_t;



#define FICHIER_CLE "cle.serv"

/* Couleurs dans term */
#define couleur(param) fprintf(stdout,"\033[%sm",param)

#define NOIR  "30"
#define ROUGE "31"
#define VERT  "32"
#define JAUNE "33"
#define BLEU  "34"
#define CYAN  "36"
#define BLANC "37"
#define REINIT "0"
