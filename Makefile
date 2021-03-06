main : main.o erreur.o carte_manager.o signal.o carte.o client.o serveur.o cuisinier.o
	gcc -o main main.o erreur.o carte_manager.o signal.o && gcc -o carte carte.o erreur.o carte_manager.o signal.o && gcc -o client client.o && gcc -o serveur serveur.o && gcc -o cuisinier cuisinier.o erreur.o carte_manager.o

carte_manager : carte_manager.c
	gcc -o carte_manager.o -c carte_manager.c -Wall

main.o : main.c
	gcc -o main.o -c main.c -Wall
	touch cle.serv

carte.o : carte.c
	gcc -o carte.o -c carte.c -Wall

client.o : client.c
	gcc -o client.o -c client.c -Wall

cuisinier.o : cuisinier.c
	gcc -o cuisinier.o -c cuisinier.c -Wall

serveur.o : serveur.c
	gcc -o serveur.o -c serveur.c -Wall

erreur.o : erreur.c
	gcc -o erreur.o -c erreur.c -Wall

signal.o : signal.c
	gcc -o signal.o -c signal.c -Wall

clean :
	rm -f client main serveur cuisinier cle.serv carte erreur *.o *.serv && ipcrm --all

git : 
	git add * && git commit -m "gitpush" && git push

gitrm :
	git rm client *.o main serveur cuisinier carte *.serv && git commit -m "rm" && git push

run :
	./main 3 3 2 4 5 8 6 3 4 2