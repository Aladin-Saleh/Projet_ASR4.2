CC = gcc -Wall 

all : main serveur cuisinier carte client

OFILES = carte.o main.o

carte : carte.c
	$(CC) carte.c -o carte erreur.c

cuisinier : cuisinier.c types.h
	$(CC) cuisinier.c -o cuisinier

serveur : serveur.c types.h
	$(CC) serveur.c -o serveur

client : client.c types.h
	$(CC) client.c -o client

main : main.c types.h carte.h
	$(CC) main.c -o main 
	touch cle.serv
	
clean :
	rm -f client main serveur cuisinier cle.serv carte erreur