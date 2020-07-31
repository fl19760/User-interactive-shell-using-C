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
extern int ex_flag;
char * rem_spaces(char *string)
{
  char *temp=(char *)malloc(sizeof(char)*10000);
  int k=-1,i=0;
  while(string[i]==' ')
              i++;
  for(i;i<strlen(string);i++)
  {
    if( !(string[i]==' ' && (string[i+1]==' '||i==strlen(string)-1)) )
    {
      k++;
      temp[k]=string[i];
    }
  }
  temp[k+1]=0;
  return temp;    
}
char *telda_rep(char *wdir,char *hdir)
{
  int i;
  for(i=0;i<strlen(hdir);i++)
  {
    if(hdir[i]!=wdir[i])
	    break;
  }
  if(i==strlen(hdir))
  {
    wdir[i-1]='~';
    wdir=wdir+(i-1);
  }
  return wdir;
}
int check_out_redirection(char *comset, char *temp)
{			
      int file=-1;
			temp=strstr(comset,">>");
			if(temp)
			{
        char *a=strstr(temp+3," ");
				if(strstr(temp+2,">") || strstr(temp+2,">>")  || strstr(temp+2,"<") || strstr(temp+2,"|"))
				{
          fprintf(stderr,"invalid format\n");
          ex_flag=0;
        }
        else
				{
					*temp=0;
					int z=2;
					while(temp[z]==' ')
						z++;
					temp=temp+z;
          if(a)
          {
            fprintf(stderr,"invalid format %d\n",ex_flag);
            ex_flag=0;
          }
          else
            file=open(temp,O_RDWR |O_APPEND | O_CREAT,0611);
				}
			}
			else if(temp=strstr(comset,">"))
			{
				char *a=strstr(temp+2," ");
        if(strstr(temp+2,">") || strstr(temp+2,">>")  || strstr(temp+2,"<") || strstr(temp+2,"|"))
				{
          fprintf(stderr,"invalid format\n");
          ex_flag=0;
        }
				else
				{
					*temp=0;
					int z=1;
					while(temp[z]==' ')
						z++;
					temp=temp+z;
          if(a)
          {
            ex_flag=0;
            fprintf(stderr,"invalid format\n");
          }
          else
            file=open(temp,O_TRUNC | O_RDWR | O_CREAT,0644);
				}
			}
      return file;
}
int comsep(char *input,char **comset)
{
  int comcnt=0;
  for(int i=0;i<100;i++)
	  comset[i]=(char *)malloc(sizeof(char)*100);
  while(input!=NULL)
  {
    comset[comcnt]=strsep(&input,";");
    // strcat(comset[comcnt],"");
    comcnt++;
  } 
  return comcnt;
}

int argsep(char *input,char **argset)
{
  int argcnt=0;
  for(int i=0;i<100;i++)
	  argset[i]=(char *)malloc(sizeof(char)*100);
  while(input!=NULL)
  {
      argset[argcnt]=strsep(&input," ");
      // strcat(argset[argcnt],"");
      argcnt++;
  }
  return argcnt;
}

int pipesep(char *input, char **pipeset)
{
  int pipecnt=0;
  for(int i=0;i<100;i++)
	  pipeset[i]=(char *)malloc(sizeof(char)*100);
  while(input!=NULL)
  {
      pipeset[pipecnt]=strsep(&input,"|");
      // strcat(pipeset[pipecnt],"");
      pipecnt++;
  }
  return pipecnt;
}

char * telda_rem(char *argset,char *hdir)
{
  char *temp=(char *)malloc(sizeof(char)*1000);
  strcpy(temp,hdir);
  strcat(temp,argset+1);
  strcpy(argset,temp);
  return argset;
}
