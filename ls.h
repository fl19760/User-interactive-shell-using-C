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
char * teldal_rem(char *argset,char *hdir)
{
  char *temp=(char *)malloc(sizeof(char)*1000);
  strcpy(temp,hdir);
  strcat(temp,argset+1);
  strcpy(argset,temp);
  return argset;
}

void print_details(char * argset,char * fname,struct stat fileStat,int argcnt)
{
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    if(!(fileStat.st_mode & S_IFREG))
    {
      DIR *d;
      struct dirent *dir;
      struct stat file;
      char *str=(char *)malloc(sizeof(char)*1000);
      d=opendir(argset);
      long long int cnt=0;
      while(dir=readdir(d))
      {
	char *temp=(char *)malloc(sizeof(char)*1000);
	strcpy(temp,argset);
	strcat(temp,"/");
	strcat(temp,dir->d_name);
	stat(temp,&file);
        if(!(file.st_mode & S_IFREG))
	{
		cnt++;
	}
      }
      printf("  %lld  ",cnt);
    }
    else
	    printf("  1  ");
    struct passwd *id;
    id=getpwuid(fileStat.st_uid);
    printf("%s  ",id->pw_name);
    struct group *grp;
    grp=getgrgid(fileStat.st_gid);
    printf("%s  ",grp->gr_name);
    printf("%ld  ",fileStat.st_size);
    char *motime=(char *)malloc(sizeof(char)*10000);
    motime=ctime(&fileStat.st_mtime);
    motime[strlen(motime)-1]=0;
    printf("%s  ",motime);
    printf("%s\n",fname);
}
void ls_l(char *argset[],char *hdir,int argcnt)
{
  if(argcnt==2)
  {
    strcpy(argset[2],".");
    argcnt++;
  }
  if(argset[2][0]=='~')
      argset[2]=teldal_rem(argset[2],hdir);
  DIR *d;
  struct dirent *dir;
  struct stat file;
  stat(argset[2],&file);
  if(file.st_mode & S_IFREG)
  {
    print_details(argset[2],argset[2],file,argcnt);
    return;
  }
  d=opendir(argset[2]);
  if(d==NULL)
          perror(argset[2]);
  else
  {
    while(dir=readdir(d))
    {
      if(dir->d_name[0]!='.')
      {
	  char *temp = (char *)malloc(sizeof(char)*1000);
	  strcpy(temp,argset[2]);
	  strcat(temp,"/");
	  strcat(temp,dir->d_name);
	  stat(temp,&file);
	  print_details(temp,dir->d_name,file,argcnt);
      }
    }
  }
}

void ls_a(char *argset[],char *hdir,int argcnt)
{
  if(argcnt==2)
  {
    strcpy(argset[2],".");
    argcnt++;
  }
  if(argset[2][0]=='~')
	  argset[2]=teldal_rem(argset[2],hdir);
  DIR *d;
  struct dirent *dir;
  struct stat file;
  stat(argset[2],&file);
  if(file.st_mode & S_IFREG)
  {
    printf("%s\n",argset[2]);
    return;
  }
  d=opendir(argset[2]);
  if(d==NULL)
          perror(argset[2]);
  else
  {
    while(dir=readdir(d))
    {
        printf("%s  ",dir->d_name);
    }
    printf("\n");
  }  
}

void ls_l_a(char *argset[],char *hdir,int argcnt)
{
  if(argcnt==3)
  {
    strcpy(argset[3],".");
    argcnt++;
  }
  if(argset[3][0]=='~')
      argset[3]=teldal_rem(argset[3],hdir);
  DIR *d;
  struct dirent *dir;
  struct stat file;
  stat(argset[3],&file);
  if(file.st_mode & S_IFREG)
  {
    print_details(argset[3],argset[3],file,argcnt);
    return;
  }
  d=opendir(argset[3]);
  if(d==NULL)
          perror(argset[3]);
  else
  {
    while(dir=readdir(d))
    {
          char *temp = (char *)malloc(sizeof(char)*1000);
          strcpy(temp,argset[3]);
          strcat(temp,"/");
          strcat(temp,dir->d_name);
          stat(temp,&file);
          print_details(temp,dir->d_name,file,argcnt);
    }
  }
}

void ls_la(char *argset[],char *hdir,int argcnt)
{
  if(argcnt==2)
  {
    strcpy(argset[2],".");
    argcnt++;
  }
  if(argset[2][0]=='~')
      argset[2]=teldal_rem(argset[2],hdir);
  DIR *d;
  struct dirent *dir;
  struct stat file;
  stat(argset[2],&file);
  if(file.st_mode & S_IFREG)
  {
    print_details(argset[2],argset[2],file,argcnt);
    return;
  }
  d=opendir(argset[2]);
  if(d==NULL)
          perror(argset[2]);
  else
  {
    while(dir=readdir(d))
    {
          char *temp = (char *)malloc(sizeof(char)*1000);
          strcpy(temp,argset[2]);
          strcat(temp,"/");
          strcat(temp,dir->d_name);
          stat(temp,&file);
          print_details(temp,dir->d_name,file,argcnt);
    }
  }
}

void ls(char *argset[],char *hdir,int argcnt)
{
  if(argcnt==1)
  {
    strcpy(argset[1],".");
    argcnt++;
  }
  if(argset[1][0]=='~')
          argset[1]= teldal_rem(argset[1],hdir);
  DIR *d;
  struct dirent *dir;
  struct stat file;
  stat(argset[1],&file);
  if(file.st_mode & S_IFREG)
  {
    printf("%s\n",argset[1]);
    return;
  }
  d=opendir(argset[1]);
  if(d==NULL)
	  perror(argset[1]);
  else
  {
    while(dir=readdir(d))
    {
      if(dir->d_name[0]!='.')
        printf("%s  ",dir->d_name);
    }
    printf("\n");
  }
}
