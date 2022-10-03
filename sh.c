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
    	if (strcmp(command,"prompt") == 0)
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

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   char *findCommand = (char*)malloc(sizeof(char)*1024);
   struct pathelement* p=pathlist;
   while(p != NULL){
    sprintf(findCommand,"%s/%s",p->element,command);
    if (access(findCommand,X_OK)==0){
      printf("%s\n",findCommand);
      return findCommand;
    }
    p = p->next;
   }
   return NULL;

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */
