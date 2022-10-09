#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>
#include "sh.h"

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/
     
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  while ( go )
  {
    /* print your prompt */
	if (strcmp(prompt," ") == 0) printf(" [%s]> ",pwd);
    	else printf("%s [%s]> ",prompt,pwd);

    /* get command line and process */
	fgets(commandline,MAX_CANON,stdin);
    	commandline[strcspn(commandline,"\n")] = 0;
	if (strcmp(commandline,"") == 0) continue;
    	command = strtok(commandline," ");
    	i = 0;
    	while (command != NULL) {
      	    args[i] = command;
            //printf("Arg %d: %s\n",i+1,command);
      	    command = strtok(NULL, " ");
      	    i++;
   	}
    /* check for each built in command and implement */
	command = strtok(commandline," ");
	if (strcmp(command,"exit") == 0)
	{
	    free(prompt);
	    free(commandline);
	    free(args);
	    free(owd);
	    go = 0;
	}
	else if (strcmp(command,"which") == 0)
	{
      char *whichIsThis = which(args[1], pathlist);
      args[1] = NULL;
	}
	else if (strcmp(command,"where") == 0)
	{
	    char *whereamI = where(args[1],pathlist);
	    args[1] = NULL;
	}
	else if (strcmp(command,"cd") == 0)
	{
	//	printf("In cd");
	    char s[100];
	    if (args[1]) {
	      if (strcmp(args[1],"-") == 0) {
	//	    printf("Previous case");
		chdir(owd);
	      }
	      else {
	//	    printf("Actual argument case");
		chdir(args[1]);
	      }
	    }
	    else {
	//	    printf("No argument case");
		    chdir(homedir);
	    }
//	    printf("changing directories");
	    owd = pwd;
	    pwd = getcwd(s,100);
	    args[1] = NULL;
	}
	else if (strcmp(command,"pwd") == 0)
	{
	    printf("%s\n",pwd);
	}
	else if (strcmp(command,"pid") == 0)
	{
	    printf("%d\n",getpid());
	}
	else if (strcmp(command,"kill") == 0)
	{
	    int arg1;
	    int arg2;
	    if (args[1]) arg1 = atoi(args[1]);
	    if (args[2]) arg2 = atoi(args[2]);
	    if (arg1 == getpid() || arg2 == getpid()) kill(getpid(),SIGTERM);
	    else if (arg1 < 0) kill(arg2,arg1);
	    args[1] = NULL;
	    args[2] = NULL;
	}
	else if (strcmp(command,"prompt") == 0)
	{
	    if (args[1]) {
		strcpy(prompt,args[1]);
	    }
	    else {
    	        printf("Input prompt prefix: ");
    	        fgets(prompt,PROMPTMAX,stdin);
	    }
    	        prompt[strcspn(prompt,"\n")] = 0;
		args[1] = NULL;
    	}
	else if (strcmp(command,"printenv") == 0)
	{
	    if (args[1]) printf("%s\n",getenv(args[1]));
	    while(*envp != 0) {
		char* temp = *envp;
		printf("%s\n",temp);
		envp++;
	    }
	    args[1] = NULL;
	    args[2] = NULL;
	}
  else if(strcmp(command, "list") == 0){
    (list(args[1]));
    args[1] = NULL;
  }
	else if (strcmp(command,"setenv") == 0)
	{
	    if (!args[1]) {
		while(*envp != 0) {
                char* temp = *envp;
                printf("%s\n",temp);
                envp++;
            	}
	    }
	    else if (!args[2]) {
		setenv(args[1],"",1);
	    }
	    else if (!args[3]) {
		    printf("Too many arguments");
	    }
	    args[1] = NULL;
	    args[2] = NULL;
	    args[3] = NULL;

	}
     /*  else  program to exec */
	else if (strcmp(which(args[0], pathlist),"Error, command cannot be found, please try again!") != 0){
	    int forkid = fork();
	    if (forkid == 0) {
		    execve(which(args[0],pathlist),args,envp);
	    }
	    else {
		    waitpid(forkid,NULL,0);
	    }
	}
       /* find it */
       /* do fork(), execve() and waitpid() */
	else if (args[0]){
            fprintf(stderr, "%s: Command not found.\n", args[0]);
	}
  }
  return 0;
} /* sh() */

//Wrote the which functionality with the help of the Professor
char *which(char *command, struct pathelement *pathlist )
	/*
	 * Which allocates memory and frees it in the function.
	 * It takes in a command (string) and a path struct.
	 */
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   char *findCommand = (char*)malloc((sizeof(char) * 100) + 1);
   //Sets a flag variable
   bool foundCommand = false;
   struct pathelement* p = pathlist;
   
   while(p != NULL){
    sprintf(findCommand,"%s/%s",p->element,command);
    if (access(findCommand,X_OK)==0){
      foundCommand = true;
      printf("%s\n",findCommand);
      return findCommand;
    }
    else{
      p = p->next;
    }
   }
   free(findCommand);
   if(foundCommand == false){
    return "Error, command cannot be found, please try it again!";
   }

} /* which() */

char *where(char *command, struct pathelement *pathlist )
	/*
	 * Where allocates memory.
	 * It takes in a command (string) and a path struct
	 */
{
  bool foundLocation;
  char *findLoaction = (char*)malloc((sizeof(char) * 100) + 1);
  struct pathelement *p = pathlist;
  while(p != NULL){
    snprintf(findLoaction, 100, "%s/%s", p->element, command);
    if(access(findLoaction, X_OK) == 0){
      int pathLen = strlen(findLoaction);
      foundLocation = true;
      char *iFoundTheLocation = calloc(pathLen + 1, sizeof(char));
      strncpy(iFoundTheLocation, findLoaction, pathLen);
      return iFoundTheLocation;
    }
    else{
      p = p->next;
    }
  }
  foundLocation = false;
  free(findLoaction);
 if(foundLocation == false){
  return "Error command cannot be found, please try again";
 }

} /* where() */


void list ( char *dir )
	/*
	 * Takes in a directory (string)
	 */
{
  DIR *direct = opendir(dir);
  struct dirent *theFile = readdir(direct);
  if(direct != NULL){
    while((theFile) != NULL){
      printf("%s\n", theFile->d_name);
    }
  }
  closedir(direct);
} /* list() */

//pid()?
