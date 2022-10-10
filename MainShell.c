#include "sh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

//This project was done by Maxwell Wang and Blair Felker

void sig_handler(int signal);

int main(int argc, char** argv, char** envp){
    //printf("My name is Walter Hartwell  White");
    signal(SIGINT,sig_handler);
    signal(SIGTSTP,sig_handler);
    return sh(argc,argv,envp);
}

void sig_handler(int signal) {
	if (signal == SIGINT);
	if (signal == SIGTSTP);
}
