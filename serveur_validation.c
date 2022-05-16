#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lectureEcriture.h"
#include "message.h"

int main(int argc, char *argv[]){

    int writer = atoi(argv[1]);
    int listener = atoi(argv[2]);

    char * msgNull = message("0000000000000000", "demande", "0000-0-0");

    while(1){

        char emeteur[255], type[255], valeur[255];
        int decoupeOk;
        char * msg = litLigne(listener);

        // slicing error detection
        decoupeOk = decoupe(msg, emeteur, type, valeur);
            
        if (!decoupeOk) {
            printf("Erreur de découpage!!\n");
            exit(0);
        } // if

        // end the program if we reach the end of the file
        if (strcmp (msg, msgNull) == 0){
            return 0;
        } // if
        
        char valeurBuf[255];
        strcpy(valeurBuf, valeur);

        // verification
        printf("Validation La test PCR est : %s\n", emeteur);
        printf("Validation Le type du message est : %s\n", type);
        printf("Validation La valeur est : %s\n", valeurBuf); 

        FILE *fD;
        fD = fopen("3000.txt", "r");

        if (fD == NULL) 
        {   
            printf("Error! Could not open file\n"); 
            exit(1);
        } // error

        char value[255];
        char line[255];
        const char * separators = ",\n-";
        char * strToken1;
        char * strToken2;

        struct tm tAp;
        struct tm tDe;
        time_t tApS;
        time_t tDeS;
        time_t tDiff;

        // 5 days
        int tLimit = 86400*5;

        strToken1 = strtok(valeur, separators);
        tAp.tm_year = atoi(strToken1)-1900;
        strToken1 = strtok(NULL, separators);
        tAp.tm_mon = atoi(strToken1);
        strToken1 = strtok(NULL, separators);
        tAp.tm_mday = atoi(strToken1);
        tAp.tm_hour = 0;
        tAp.tm_min = 0;
        tAp.tm_sec = 0;
        tAp.tm_isdst = 0;
        tApS = mktime(&tAp);

        while (fgets(line, sizeof(line), fD) != NULL){
            strToken2 = strtok (line, separators);
            
            if (strcmp (strToken2, emeteur) == 0){

                strToken2 = strtok (NULL, separators);
                tDe.tm_year = atoi(strToken2)-1900;
                strToken2 = strtok (NULL, separators);
                tDe.tm_mon = atoi(strToken2);
                strToken2 = strtok (NULL, separators);
                tDe.tm_mday = atoi(strToken2);
                tDe.tm_hour = 0;
                tDe.tm_min = 0;
                tDe.tm_sec = 0;
                tDe.tm_isdst = 0;
                tDeS = mktime(&tDe);
                tDiff = tApS - tDeS;

                // check whether the PCR test is still valid or not
                if (tDiff <= tLimit){
                    strcpy(value, " (valide)");
                    break;
                } // if
                else{
                    strcpy(value, " (invalide)");
                    break;
                } // else
            }
            else{
                strcpy(value, " (inconnue)");
            } // else
        }

        fclose(fD);

        char type2[255];
        char buf[255] = "réponse";

        strcat(strcpy(type2, buf), value);

        msg = message(emeteur, type2, valeurBuf);

        // slicing error detection
        decoupeOk = decoupe(msg, emeteur, type2, valeurBuf);

        if (!decoupeOk) {
            printf("Erreur de découpage!! \n");
            exit(0);
        } // if
        
        // verification
        printf("Validation-Retour La test PCR est : %s\n", emeteur);
        printf("Validation-Retour  Le type du message est : %s\n", type2);
        printf("Validation-Retour  La valeur est : %s\n", valeurBuf); 

        ecritLigne(writer, msg);
    }

    return 0;
}

