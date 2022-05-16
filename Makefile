all: terminal serveur_acquisition serveur_validation 

message.o: message.c message.h
	gcc -Wall -c message.c

lectureEcriture.o: lectureEcriture.c lectureEcriture.h
	gcc -c -Wall lectureEcriture.c

terminal: terminal.c lectureEcriture.o  message.o
	gcc -Wall terminal.c lectureEcriture.o message.o -o  terminal

serveur_validation: serveur_validation.c lectureEcriture.o message.o
	gcc -Wall serveur_validation.c lectureEcriture.o message.o -o  serveur_validation

serveur_acquisition: serveur_acquisition.c lectureEcriture.o message.o
	gcc -Wall serveur_acquisition.c lectureEcriture.o message.o -o  serveur_acquisition

clean:	
	rm -f *.o