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
#include "types.h"
#include "erreur.h"
#include <time.h>

int main(int argc, char const *argv[]) {

	key_t cle;			/* cle de la file     */
	int file_mess;		/* ID de la file    */
	command_t commande;
	commandcliserv_t cmdRecue;
	int nbSpe = (int) strtol((argv[1]), NULL, 0);
	//printf("nbSpe = %d\n", nbSpe);
	int *nbServeurs, shmid;

	int choixSpe = -1;
	int occupationServeur = -1;
	int plusCourtServ = -1;
	int taillePlusCourtServ = 99; 	// On initialise le serveur le plus occupé 
	int serveurs[10];					// Tableau contenant les numéros d'ordre des serveurs
	pid_t pid = getpid();


	/* Calcul de la cle	                 */

	cle = ftok(FICHIER_CLE,'a');
	//printf("cle = %d\n", cle);
	assert(cle != -1);


	/* Recuperation file de message :    */

	file_mess = msgget(cle, 0666 | IPC_CREAT);
	assert(file_mess != -1);


	/* Séléction de la carte */
	key_t k = ftok("/tmp",1);
	assert(k!=-1);

	shmid=shmget(k, 0, 0);
	assert(shmid >= 0);

	nbServeurs = (int*)shmat(shmid,NULL,0);
	assert(nbServeurs != (void *)-1);

	srand(time(NULL));
	
	choixSpe = rand() % (nbSpe + 1);

	/* Selection du serveur le moins occupé */

	for(int i = 0; i < *nbServeurs; i++) {

		/************************************************************** PAS ENCORE DE RECUP DES SERVEURS **************************************************************/

		//occupationServeur = msg_qnum(serveurs[i]);		// récupère la taille de la file du serveur
		if(occupationServeur < taillePlusCourtServ) {	// Si le serveur actuel a une occupation plus courte que le moins occupé

			taillePlusCourtServ = occupationServeur;	// On met à jour la plus petite taille
			plusCourtServ = serveurs[i];				// Il devient le moins occupé

		}

	}


	/* creation de la requete : */

	commande.type = 1;
	commande.serveur = plusCourtServ;
	commande.choix = choixSpe;
	commande.expediteur = getpid();


	/* Envoie la commande au serveur */

	if(msgsnd(file_mess, &commande, sizeof(command_t)-sizeof(long), 0) == -1) {
		perror("Erreur lors de l'envoi de la commande ");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d envoie la commande %d au serveur %d\n", pid, commande.choix, commande.serveur);

	couleur(REINIT);


	/* attente de la reponse : et paie */

	if (msgrcv(file_mess, &cmdRecue, sizeof(commandcliserv_t) - sizeof(long), (long) getpid(), 0) == -1) {	// On attend un message de type 3 (serveur)
		perror("Erreur lors de la recuperation de la commande ");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d reçoit le prix\n", pid);

	couleur(REINIT);


	if(msgsnd(file_mess, &commande, sizeof(command_t)-sizeof(long), 0) == -1) {
		perror("Erreur lors de l'envoi de la commande");
		exit(-1);
	}

	couleur(CYAN);

	printf("Le client %d a payé\n", pid);

	couleur(REINIT);

	/* attente de la commande */

	if (msgrcv(file_mess, &cmdRecue, sizeof(commandcliserv_t) - sizeof(long), (long) getpid(), 0) == -1) {	// On attend un message de type 3 (serveur)
		perror("Erreur lors de la recuperation de la commande ");
		exit(-1);
	}


	couleur(CYAN);

	printf("Le client %d reçoit la commande %d\n",pid, cmdRecue.choix);

	couleur(REINIT);
}