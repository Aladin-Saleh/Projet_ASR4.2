/*Programme réaliser par Aladin SALEH & Paul MINGUET
**06/02/21
**IUT Sénart Fontainebleau - Année 2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <assert.h>
#include "types.h"
#include <sys/stat.h>



int file_mess_cli, file_mess_cui; 	/* ID de la file, necessairement global pour pouvoir la supprimer a la terminaison */
key_t cleCli, cleCui; 				/* cle de la file     */



int main (int argc, char *argv[]){
	command_t commande;
	commandcuiserv_t commande2Cui, commandeFromCui;
	commandcliserv_t commande2Cli;
	int numOrdre = (int) strtol(argv[1], NULL, 0);
	pid_t pid = getpid();


	/* cacul de la cle de la file    */

	cleCli = ftok(FICHIER_CLE,'a');
	cleCui = ftok(FICHIER_CLE, 'c');


	assert(cleCli != -1);
	assert(cleCui != -1);


	/* Creation file de message :    */

	file_mess_cli = msgget(cleCui,0666|IPC_CREAT);
	file_mess_cui = msgget(cleCui, 0666|IPC_CREAT);
	printf("file_mess_cli = %d, file_mess_cui = %d \n",file_mess_cli, file_mess_cui);
	//sleep(10);
	
	assert( file_mess_cli != -1);
	assert( file_mess_cui != -1);

	while(1) { 

		/* serveur attend des commandes de clients, de type 1 :        */
	 	
		if (msgrcv(file_mess_cli, &commande, sizeof(command_t) - sizeof(int), 1, 0)== -1 ) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}
		
		fprintf(stdout, "Serveur n° %d a reçu la commande du client %d\n", numOrdre, commande.expediteur);


		/* traitement de la requete : */

		fprintf(stdout, "numCo = %d\n", commande.choix);

		
		/* Prépare la commande du cuisinier */

		commande2Cui.type = 3;
		commande2Cui.choix = commande.choix;
		commande2Cui.expediteur = pid;


		/* Envoie la commande au cuisinier */

		if(msgsnd(file_mess_cui, &commande2Cui, sizeof(commandcuiserv_t) - sizeof(int), IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}

		printf("Le serveur %d envoie la commande du client %d\n", commande2Cui.expediteur, commande2Cui.choix);


		/* Attend la réponse du cuisinier */

		if (msgrcv(file_mess_cui, &commandeFromCui, sizeof(commandcuiserv_t) - sizeof(int), 1, 0)== -1 ) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}
		
		fprintf(stdout, "Serveur n° %d a reçu le plat du cuisinier %d\n", numOrdre, commandeFromCui.expediteur);


		/* Renvoie au client */

		commande2Cli.type = commande.expediteur;
		commande2Cli.choix = commandeFromCui.choix;


		/* Attente aleatoire */

		sleep(5);


		/* envoi de la reponse : */

		if(msgsnd(file_mess_cli,&commande2Cli, sizeof(commandcliserv_t) - sizeof(int),IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}

	}
	return EXIT_SUCCESS;
}