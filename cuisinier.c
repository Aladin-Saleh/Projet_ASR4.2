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
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <assert.h>
#include "./Header/types.h"
#include "./Header/erreur.h"
#include <sys/stat.h>



int id_sem,id_shm;
int file_mess_serv; 				/* ID de la file, necessairement global pour pouvoir la supprimer a la terminaison */
key_t cle_serv; 						/* cle de la file     */
union semun u;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

struct sembuf p = { 0, -1, SEM_UNDO};
struct sembuf v = { 0, +1, SEM_UNDO};


int main (int argc, char *argv[]){
	couleur(VERT);
	commandcuiserv_t commande2Serv, commandeFromServ;
	//int num_ordre = (int) strtol(argv[1], NULL, 0);
	pid_t pid = getpid();
	int *num_spe;


	/* cacul de la cle de la file    */

	cle_serv = ftok(FICHIER_CLE, 'a');


	assert(cle_serv != -1);

	if ((id_sem = semget(cle_serv,1,0666|IPC_CREAT)) == -1)
	{
		erreur("Erreur lors de la creation de la semaphore...");
	}



	/* Creation file de message :    */

	file_mess_serv = msgget(cle_serv, 0);
	//printf("file_mess_serv = %d\n",file_mess_serv);
	//sleep(10);

	if ((id_sem = semget(cle_serv,1,0666|IPC_CREAT)) == -1)
	{
		erreur("Erreur lors de la creation de la semaphore...");
	}


	


	assert( file_mess_serv != -1);
	*num_spe = commandeFromServ.choix;
	u.val = 1;

	if(semctl(id_sem, 0, SETVAL, u) < 0)
    {
        erreur("Erreur semclt....");
    }

	while(1) { 

		couleur(VERT);
		fprintf(stdout, "\t\tCuisinier n° %d attend une commande\n", pid);

		couleur(REINIT);

		/* serveur attend des commandes de clients, de type 1 :        */
	 	
		if (msgrcv(file_mess_serv, &commandeFromServ, sizeof(commandcuiserv_t) - sizeof(long), 3, 0) == -1) {
			perror("Erreur lors de la reception...");
			exit(-1);
		}

		couleur(BLANC);
		
		fprintf(stdout, "\t\tCuisinier n° %d a reçu la commande du serveur %d pour le client %d\n", pid, commandeFromServ.expediteur, commandeFromServ.client);

		/* traitement de la requete : */
		printf("\t\tCuisinier n°%d est en train de préparer la spécialité n°%d \n",pid,commandeFromServ.choix);
		printf("\t\tPréparation en cours par le cuisinier n°%d...\n",pid);
		sleep(rand() % 5);


		///fprintf(stdout, "numCo = %d\n", commandeFromServ.choix);

       
		/* Préparer le plat */
		couleur(REINIT);


		
		/* Prépare la commande du serveur */

		commande2Serv.type = commandeFromServ.expediteur;
		commande2Serv.choix = commandeFromServ.choix;
		commande2Serv.client = commandeFromServ.client;
		commande2Serv.expediteur = pid;


		/* Envoie la commande au serveur */

		if(msgsnd(file_mess_serv, &commande2Serv, sizeof(commandcuiserv_t) - sizeof(long), IPC_NOWAIT) == -1) {
			perror("Erreur lors de l'envoi de la commande ");
			exit(-1);
		}

		couleur(VERT);

		printf("\t\tCuisinier %d envoie la commande du client %d au serveur %d\n", pid, commande2Serv.client, commandeFromServ.expediteur);

		couleur(REINIT);
	}
	return EXIT_SUCCESS;
}
