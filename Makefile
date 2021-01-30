CC = gcc -Wall 

all : main client serveur cuisinier

cuisinier : cuisinier.c 
	$(CC) cuisinier.c -o cuisinier

serveur : serveur.c 
	$(CC) serveur.c -o serveur

client : client.c
	$(CC) client.c -o client

main : main.c 
	$(CC) main.c -o main
	touch cle.serv
clean :
	rm -f client main serveur cuisinier cle.serv