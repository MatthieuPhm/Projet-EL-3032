#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "lectureEcriture.h"
#include "message.h"

int main(){

    // creating terminal-serveur_acquisition pipes
    int fd[2];
    int fd2[2];
    pipe(fd);
    pipe(fd2);

    // creating serveur_acquisition-serveur_validation pipes
    int fd3[2];
    int fd4[2];
    pipe(fd3);
    pipe(fd4);

    pid_t cT, cSV;

    cT = fork();

    if(cT == -1){
        printf("Erreur dans le premier fork \n");
    } // error

    else if (cT == 0){
        char firstPipe[255];
        char secondPipe[255];

        sprintf(firstPipe, "%d", fd[1]);
        sprintf(secondPipe, "%d", fd2[0]);

        //  create child process: terminal
    	execlp("./terminal","./terminal",firstPipe,secondPipe,NULL);
    } // cT
    
        cSV = fork();

        if (cSV == -1){
            printf("Erreur dans le second fork \n");
        } // error

        else if (cSV== 0){
            char thirdPipe[255];
            char fourthPipe[255];

            sprintf(thirdPipe, "%d", fd3[1]);
            sprintf(fourthPipe, "%d", fd4[0]);

            //  create child process: serveur_validation
            execlp("./serveur_validation","./serveur_validation",thirdPipe,fourthPipe,NULL);
        } // cSV 

        char verif[255] = "3000";
        int count = 0;
        int i;

        char * msgNull = message("0000000000000000", "null", "0000-0-0");

        while(1){
            char emeteur[255], type[255], valeur[255];
            int decoupeOk;

            char * message = litLigne(fd[0]);

            // slicing error detection
            decoupeOk = decoupe(message, emeteur, type, valeur);
            
            if (!decoupeOk) {
                printf("Erreur de découpage!!\n");
                exit(0);
            } // if

            // end the program if we reach the end of the file
            if (strcmp (message, msgNull) == 0){
                return 0;
            } // if

            // verification
            printf("Acquisition La test PCR est : %s\n", emeteur);
            printf("Acquisition Le type du message est : %s\n", type);
            printf("Acquisition La valeur est : %s\n", valeur); 

            // test whether the test is local (id == 3000), alternative to serveur_inter
            for(i = 0; i < 4; i++){
                if (emeteur[i] == verif[i]){
                    count++;
                } // if
            } // for

            // send the message to serveur_validation (local test)
            if (count == 4){
                ecritLigne(fd4[1], message);
            } // if
            // write the null message in the pipe, alternative to serveur_inter
            else{
                ecritLigne(fd4[1], msgNull);
            } // else

            // reset the counter
            count = 0;

            // read from serveur_validation (through fd3)
            message = litLigne(fd3[0]);
            ecritLigne(0,message);

            // slicing error detection
            decoupeOk = decoupe(message, emeteur, type, valeur);
            
            if (!decoupeOk) {
                printf("Erreur de découpage!!\n");
                exit(0);
            } // if

            // verification
            printf("Acquisition-Retour La test PCR est : %s\n", emeteur);
            printf("Acquisition-Retour Le type du message est : %s\n", type);
            printf("Acquisition-Retour La valeur est : %s\n", valeur); 

            // send the message to terminal (through fd2)
            ecritLigne(fd2[1], message);

        } // while

    return 0;
} // main