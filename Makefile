CC = gcc -Wall 

all : main serveur cuisinier carte erreur client

erreur : erreur.c
	$(CC) erreur.c -o erreur

carte : carte.c types.h
	$(CC) carte.c -o carte

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
	rm -f client main serveur cuisinier carte cle.serv