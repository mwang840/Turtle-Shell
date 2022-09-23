#include "sh.h"
#include <stdio.h>
#include <stdlib.h>

void sig_hander(int signal);

int main(int argc, char** argv, char** envp){
    printf("My name is Walter Hartwell  White");
    return sh(argc,argv,envp);
}

void sig_handler(int signal) {
}
