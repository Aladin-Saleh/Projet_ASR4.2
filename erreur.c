#include <stdio.h>
#include <stdlib.h>
#include "erreur.h"

void erreur(char * texte_erreur);

int main(int argc, char const *argv[])
{
    
    return 0;
}


void erreur(char * texte_erreur)
{
    printf("%s\n",texte_erreur);
    exit(-1);
}