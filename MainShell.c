#include "sh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sig_hander(int signal);

int main(int argc, char** argv, char** envp){
    //printf("My name is Walter Hartwell  White");
    //signal(SIGINT,sig_hander);
    return sh(argc,argv,envp);
}

void sig_handler(int signal) {
}
