/*Programme réaliser par Aladin SALEH & Paul MINGUET
**06/02/21
**IUT Sénart Fontainebleau - Année 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "./Header/types.h"
#include "./Header/erreur.h"
#include <time.h>

int main(int argc, char const *argv[]) {

	key_t cle;			/* cle de la file     */
	int file_mess, pls_crt_file;		/* ID de la file    */
	struct msqid_ds buff, buf;
	command_t commande;
	commandcliserv_t cmdRecue;
	int nbSpe = (int) strtol((argv[1]), NULL, 0);
	char cleVal = 'b';
	char *tmp = NULL;
	int choixSpe = -1;
	pid_t pid = getpid();

	tmp = (char *)malloc((4 + 20) * sizeof(char));
	sprintf(tmp, "t%c.serv", cleVal);
	cle = ftok(tmp, cleVal);

	/* Recuperation file de message :    */

	pls_crt_file = msgget(cle, 0);
	assert(pls_crt_file != -1);

	/* Séléction du serveur le moins occupé */

	while(1) {
		sprintf(tmp, "t%c.serv", cleVal);
		cle = ftok(tmp, cleVal);
		if(cle == -1) {
			file_mess = pls_crt_file;
			break;
		}
		file_mess = msgget(cle, 0);
		msgctl(file_mess, IPC_STAT, &buf);
		msgctl(pls_crt_file, IPC_STAT, &buff);

		if(buff.msg_qnum > buf.msg_qnum)
			pls_crt_file = file_mess;
		cleVal++;
	}

	/* Choix de la spécialité */

	srand(pid);
	
	choixSpe = (rand() % nbSpe) + 1;


	
	/* creation de la requete : */

	commande.type = 1;
	commande.choix = choixSpe;
	commande.expediteur = getpid();


	/* Envoie la commande au serveur */

	if(msgsnd(file_mess, &commande, sizeof(command_t)-sizeof(long), 0) == -1) {
		perror("Erreur lors de l'envoi de la commande ");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d envoie la commande %d sur la file ", pid, commande.choix);

	couleur(BLANC);

	printf("%d\n", file_mess);

	couleur(REINIT);


	/* attente de la reponse : et paie */

	if (msgrcv(file_mess, &cmdRecue, sizeof(commandcliserv_t) - sizeof(long), (long) getpid(), 0) == -1) {	// On attend un message de type 3 (serveur)
		perror("Erreur lors de la recuperation de la commande ");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d paye\n", pid);

	couleur(REINIT);


	/*if(msgsnd(file_mess, &commande, sizeof(command_t)-sizeof(long), 0) == -1) {
		perror("Erreur lors de l'envoi de la commande");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d a payé\n", pid);

	couleur(REINIT);*/

	/* attente de la commande */

	/*if (msgrcv(file_mess, &cmdRecue, sizeof(commandcliserv_t) - sizeof(long), (long) getpid(), 0) == -1) {	// On attend un message de type 3 (serveur)
		perror("Erreur lors de la recuperation de la commande ");
		exit(-1);
	}*/


	couleur(CYAN);

	printf("Le client %d reçoit la commande %d\n",pid, cmdRecue.choix);

	couleur(REINIT);
}