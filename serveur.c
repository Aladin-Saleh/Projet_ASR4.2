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
#include <sys/stat.h>
#include "./Header/carte_manager.h"
#include "./Header/types.h"
#include "./Header/erreur.h"



int file_mess_cui, pls_lng_file, file_mess; 	/* ID de la file, necessairement global pour pouvoir la supprimer a la terminaison */
key_t cleCui, cle; 				/* cle de la file     */



int main (int argc, char *argv[]){
	command_t commande;
	commandcuiserv_t commande2Cui, commandeFromCui;
	commandcliserv_t commande2Cli;
	struct msqid_ds buff, buf;
	char cleVal = 'b';
	char *tmp = NULL;
	//int numOrdre = (int) strtol(argv[1], NULL, 0);
	pid_t pid = getpid();


	/* cacul de la cle de la file    */

	cleCui = ftok(FICHIER_CLE, 'a');


	assert(cleCui != -1);


	file_mess_cui = msgget(cleCui, 0666|IPC_CREAT);
	
	assert( file_mess_cui != -1);

	tmp = (char *)malloc((4 + 20) * sizeof(char));
	sprintf(tmp, "c%c.serv", cleVal);
	cle = ftok(tmp, cleVal);

	/* Recuperation file de message :    */

	pls_lng_file = msgget(cle, 0);
	assert(pls_lng_file != -1);

	while(1) {

		cleVal = 'b';

		/* Séléction du serveur le moins occupé */

		while(1) {
			sprintf(tmp, "c%c.serv", cleVal);
			//printf("tmp = %s\ncleVal = %c\n", tmp, cleVal);
			cle = ftok(tmp, cleVal);
			// printf("cle = %d\n", cle);
			if(cle == -1) {
				file_mess = pls_lng_file;
				break;
			}
			file_mess = msgget(cle, 0);
			msgctl(file_mess, IPC_STAT, &buf);
			msgctl(pls_lng_file, IPC_STAT, &buff);
			// printf("file_mess qnum = %ld\npls_lng_file qnum = %ld\n\n", buf.msg_qnum, buff.msg_qnum);

			if(buff.msg_qnum < buf.msg_qnum)
				pls_lng_file = file_mess;
			cleVal++;
		}

		couleur(ROUGE);

		printf("\tLe serveur %d a choisi la file ", pid);

		couleur(BLANC);

		printf("%d\n", file_mess);

		couleur(REINIT);

		couleur(ROUGE);

		fprintf(stdout, "\tServeur n° %d attend une commande\n", pid);

		couleur(REINIT);

		/* serveur attend des commandes de clients, de type 1 :        */
	 	
		if (msgrcv(file_mess, &commande, sizeof(command_t) - sizeof(long), 1, 0)== -1 ) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}

		couleur(ROUGE);
		
		fprintf(stdout, "\tServeur n° %d a reçu la commande du client %d\n", pid, commande.expediteur);

		couleur(REINIT);

		/* traitement de la requete : */

		//fprintf(stdout, "numCo = %d\n", commande.choix);

		
		/* Prépare la commande du cuisinier */

		commande2Cui.type = 3;
		commande2Cui.choix = commande.choix;
		commande2Cui.client = commande.expediteur;
		commande2Cui.expediteur = pid;


		/* Envoie la commande au cuisinier */

		if(msgsnd(file_mess_cui, &commande2Cui, sizeof(commandcuiserv_t) - sizeof(long), IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}

		couleur(ROUGE);

		fprintf(stdout, "\tServeur n° %d envoie la commande du client %d\n", pid, commande2Cui.client);

		couleur(REINIT);


		/* Attend la réponse du cuisinier */

		if (msgrcv(file_mess_cui, &commandeFromCui, sizeof(commandcuiserv_t) - sizeof(long), (long) pid, 0)== -1 ) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}

		couleur(ROUGE);
		
		fprintf(stdout, "\tServeur n° %d a reçu le plat du cuisinier %d\n", pid, commandeFromCui.expediteur);

		couleur(REINIT);


		/* Renvoie au client */

		commande2Cli.type = commande.expediteur;
		commande2Cli.choix = commandeFromCui.choix;


		/* Attente aleatoire */



		/* envoi du prix : */

		/*if(msgsnd(file_mess,&commande2Cli, sizeof(commandcliserv_t) - sizeof(long),IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}*/


		/* attente du paiement */

		/*if (msgrcv(file_mess, &commande, sizeof(command_t) - sizeof(long), 1, 0)== -1 ) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}

		couleur(ROUGE);
		
		fprintf(stdout, "\tServeur n° %d a reçu le paiement du client %ld\n", pid, commande2Cli.type);

		couleur(REINIT);*/



		/* envoi de la commande */

		if(msgsnd(file_mess,&commande2Cli, sizeof(commandcliserv_t) - sizeof(long),IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}

		couleur(ROUGE);

		fprintf(stdout, "\tServeur n° %d envoie la commande au client %ld\n", pid, commande2Cli.type);

		couleur(REINIT);


	}
	return EXIT_SUCCESS;
}