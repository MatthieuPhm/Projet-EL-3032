#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "lectureEcriture.h"
#include "message.h"

int main(int argc, char *argv[]){

    int writer = atoi(argv[1]);
    int listener = atoi(argv[2]);
    int decoupeOk;

    char line[255];
    const char * separators = ",\n";
    char * strToken;

    char emeteur2[255], type2[255], valeur2[255];

    FILE *f;
    f = fopen("demandes.txt", "r");

    if (f == NULL) 
    {   
          printf("Error! Could not open file\n"); 
          exit(1);
    } // error

    while (fgets(line, sizeof(line), f) != NULL){
        strToken = strtok (line, separators);
        strcpy(emeteur2, strToken);
        strToken = strtok (NULL, separators);
        strcpy(type2, strToken);
        strToken = strtok (NULL, separators);
        strcpy(valeur2, strToken);

        char * msg = message(emeteur2, type2, valeur2);

        // slicing error detection
        decoupeOk = decoupe(msg, emeteur2, type2, valeur2);

        if (!decoupeOk){
            printf("Erreur de découpage!! \n");
            exit(0);
        } // if

        // verification
        printf("Terminal La test PCR est : %s\n", emeteur2);
        printf("Terminal Le type du message est : %s\n", type2);
        printf("Terminal La valeur est : %s\n", valeur2); 

        ecritLigne(writer, msg);

        char emeteur[255], type[255], valeur[255];
        char * msg2 = litLigne(listener);

        // slicing error detection
        decoupeOk = decoupe(msg2, emeteur, type, valeur);

        if (!decoupeOk){
        printf("Erreur de découpage!! \n");
        exit(0);
        } // if

        // verification
        printf("Terminal-Listener La test PCR est : %s\n", emeteur);
        printf("Terminal-Listener Le type du message est : %s\n", type);
        printf("Terminal-Listener La valeur est : %s\n", valeur);

    } // while

    fclose(f);

    return 0;
}

