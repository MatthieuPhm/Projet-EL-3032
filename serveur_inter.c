#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "lectureEcriture.h"
#include "message.h"
int main(){

    // creating serveur_acquisition-serveur_inter pipes
    int fdA[2];
    int fdA2[2];
    pipe(fdA);
    pipe(fdA2);

    // creating serveur_inter-serveur_acquisition_etranger pipes
    int fdE[2];
    int fdE2[2];
    pipe(fdE);
    pipe(fdE2);

    pid_t cA, cAE;

    cA = fork();
    
    if(cA == -1){
        printf("Erreur dans le premier fork \n");
    } // error

    else if (cA == 0){
        char firstPipe[255];
        char secondPipe[255];

        sprintf(firstPipe, "%d", fdA[1]);
        sprintf(secondPipe, "%d", fdA2[0]);

    	execlp("./serveur_acquisition","./serveur_acquisition",firstPipe,secondPipe,NULL);
    } // cA

        cAE = fork();

        if (cAE == -1){
            printf("Erreur dans le second fork \n");
        } // error

        else if (cAE == 0){
            char thirdPipe[255];
            char fourthPipe[255];

            sprintf(thirdPipe, "%d", fdE[1]);
            sprintf(fourthPipe, "%d", fdE2[0]);

            execlp("./serveur_validation","./serveur_validation",thirdPipe,fourthPipe,NULL);
        } // cAE
        
} // main