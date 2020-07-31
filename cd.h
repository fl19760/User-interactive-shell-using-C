#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
int argcnt;
char * cd(char *argset[],char *wdir,char *hdir,int argcnt)
{
          //printf("command is: %s\n",argset[0]);
        if(argcnt!=2)
                fprintf(stderr,"invalid no.of arguments\n");
        else
        {
	  if(argset[1][0]=='~')
	  {
	    argset[1]=telda_rem(argset[1],hdir);
	  }
          if(chdir(argset[1])==0)
          {
            getcwd(wdir,1000);
            wdir=telda_rep(wdir,hdir);
          }
          else
                  perror(argset[1]);
        }
	return wdir;
}
