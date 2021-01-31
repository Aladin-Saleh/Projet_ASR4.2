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

int main(int argc, char const *argv[]) {
    
    key_t cle;			/* cle de la file     */
    int file_mess;		/* ID de la file    */
    command_t commande;
    cmdRecue_t cmdRecue;
    int choixSpe = -1;
    int occupationServeur = -1;
    int plusCourtServ = -1;
    int taillePlusCourtServ = 99; 	// On initialise le serveur le plus occupé 
    int[] serveurs;					// Tableau contenant les numéros d'ordre des serveurs
    pid_t pid = getpid();


    /* Calcul de la cle	                 */

	cle = ftok(FICHIER_CLE,'a');
    
	
	assert(cle != -1);

	
	/* Recuperation file de message :    */

	file_mess = msgget(cle,0);
	assert(file_mess != -1);


    /* Séléction de la carte
     * key_t k = ftok("/tmp",1);
	 * assert(k!=-1);
	 * 
	 * shmid=shmget(k, 0, 0);
	 * assert(shmid >= 0);
	 * 
	 * choixSpe = rand(0, (int*)shmat(shmid,NULL,0));
     */


    /* Selection du serveur le moins occupé */

    for(int i = 0; i < serveurs.length; i++) {
    	occupationServeur = msg_qnum(serveurs[i]);		// récupère la taille de la file du serveur
    	if(occupationServeur < taillePlusCourtServ) {	// Si le serveur actuel a une occupation plus courte que le moins occupé
    		taillePlusCourtServ = occupationServeur;	// On met à jour la plus petite taille
    		plusCourtServ = serveurs[i];				// Il devient le moins occupé
    	}
    }

    /* creation de la requete : */

	commande.serveur = plusCourtServ;
	commande.choix = choixSpe;
	commande.expediteur = getpid();


    /* Envoie la commande au serveur */

    if(msgsnd(file_mess, &commande, sizeof(command_t)-sizeof(int), 0) == -1) {
		perror("Erreur lors de l'envoi de la commande ");
		exit(-1);
	}


	/* attente de la reponse : */
	
	cmdRecue.type /* ou type structure de la spécialité */ = getpid();
	if (msgrcv(file_mess, &cmdRecue /* ou struct spé */, sizeof(cmdRecue_t /* ou struct spé */)-sizeof(long), cmdRecue.type /* ou struct spé */, 0) == -1) {
		perror("Erreur lors de la recuperation de la commande ");
		exit(-1);
	}

	printf("Le client %d recoit la commande %d\n",pid, cmdRecue.choix);

    return 0;
}
