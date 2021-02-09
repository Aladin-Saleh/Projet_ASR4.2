/*Programme réaliser par Aladin SALEH & Paul MINGUET
**06/02/21
**IUT Sénart Fontainebleau - Année 2
*/

#include <stdio.h>
#include <stdlib.h>
#include "erreur.h"
void erreur(char * texte_erreur)
{
    printf("%s\n",texte_erreur);
    exit(-1);
}

void debug(char * message_debug)
{
       printf ("\033[34;01m%s\033[00m\n",message_debug);
}