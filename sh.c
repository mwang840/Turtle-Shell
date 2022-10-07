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
    	command = strtok(commandline," ");

    	//printf("Command: %s\n",command);
    	command = strtok(NULL, " ");
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
	//  which(args[0],args[1]);
	    args[0] = NULL;
	    args[1] = NULL;
	}
	else if (strcmp(command,"where") == 0)
	{
	    //where(args[0],args[1]);
	    args[0] = NULL;
	    args[1] = NULL;
	}
	else if (strcmp(command,"cd") == 0)
	{
	}
	else if (strcmp(command,"pwd") == 0)
	{
	    printf("%s\n",pwd);
	}
	else if (strcmp(command,"list") == 0)
	{
	}
	else if (strcmp(command,"pid") == 0)
	{
	}
	else if (strcmp(command,"kill") == 0)
	{
	}
	else if (strcmp(command,"prompt") == 0)
	{
	    if (args[0]) {
		prompt = args[0];
	    }
	    else {
    	        printf("Input prompt prefix: ");
    	        fgets(prompt,PROMPTMAX,stdin);
	    }
    	        prompt[strcspn(prompt,"\n")] = 0;
		args[0] = NULL;
    	}
	else if (strcmp(command,"printenv") == 0)
	{
	}
	else if (strcmp(command,"setenv") == 0)
	{
	}
     /*  else  program to exec */
    {
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    }
  }
  return 0;
} /* sh() */

//Wrote the which functionality with the help of the Professor
char *which(char *command, struct pathelement *pathlist )
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
{
  char *findLoaction = (char*)malloc((sizeof(char) * 100) + 1);
  struct pathelement *p = pathlist;
  while(p != NULL){
    snprintf(findLoaction, 100, "%s/%s", p->element, command);
    if(access(findLoaction, X_OK) == 0){
      int pathLen = strlen(findLoaction);
      char *iFoundTheLocation = calloc(pathLen + 1, sizeof(char));
      strncpy(iFoundTheLocation, findLoaction, pathLen);
    }
    p = p->next;
  }
  return NULL;
  /* similarly loop through finding all locations of command */
  /*char cmd[128];
  char list[128][128];
  int ind = 0;
  struct pathelement* p=pathlist;
  while (p) {
   sprintf(cmd, "%s/gcc", p->element);
   if (access(cmd, F_OK) == 0) {
     list[ind] = cmd;
     ind++;
   }
   p = p->next;
  }*/

} /* where() */


void list ( char *dir )
{
  DIR *direct = opendir(dir);
  struct dirent *theFile = readdir(direct);
  if(direct != NULL){
    while((theFile) != NULL){
      printf("%s\n", theFile->d_name);
    }
  }
  closedir(direct);
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */
