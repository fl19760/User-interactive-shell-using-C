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
#include<readline/readline.h>
#include<readline/history.h>
#include"ls.h"
#include"parsing.h"
#include"cd.h"
#include"pid.h"
#include"interrupts.h"
#include"execute.h"
int ex_flag=1;
int comcnt=0,argcnt=0,pidcnt=0,pipecnt=0;
char pidset[100][3][100];
char ch;
char *host;
char  *user;
char *dir;
char *input;
char *comset[100];
char *argset[100];
char *pipeset[100];
char *wdir;
char *hdir;
char prompt[500];
int gl_fg=-1;
int main()
{
	signal(SIGINT,&parent_int);
	signal(SIGTSTP,&parent_stp);
//	signal(SIGCONT,&parent_con);
	host= (char *)malloc(sizeof(char)*100);
	user= (char *)malloc(sizeof(char)*100);
	wdir = (char *)malloc(sizeof(char)*1000);
	hdir = (char *)malloc(sizeof(char)*1000);
	dir = (char *)malloc(sizeof(char)*1000);
    input= (char *)malloc(sizeof(char)*1000);
	getcwd(wdir,1000);
	getcwd(hdir,1000);
	wdir=telda_rep(wdir,hdir);
	for(int i=0;i<100;i++)
	{
		comset[i]=(char *)malloc(sizeof(char)*100);
		pipeset[i]=(char *)malloc(sizeof(char)*100);
		argset[i]=(char *)malloc(sizeof(char)*100);
	}
	using_history();
 	while(1)
	{   
		// char c;
		// proc_state(getpid(),&c);
		// printf("%c\n",c);
	    int temp_out,temp_in;
		input=NULL;
		temp_out=dup(1);
		temp_in=dup(0);
		// input[0]=0;
		user=getenv("USER");
		gethostname(host,100);
		int l_flag=0,ll_flag=0;
		sprintf(prompt,"<%s@%s:%s> ",user,host,wdir);
		// scanf("%[^\n]s",input);
		// scanf("%c",&ch);
		input=readline(prompt);
		// fprintf(stderr, "GOT INPUT %d bytes, as %s\n", strlen(input), input);
		add_history(input);
		if(strlen(input) == 0 ) continue;
		input=rem_spaces(input);
	    // printf("input:%s\n",input);
		
		comcnt=comsep(input,comset);
		for(int i=0;i<comcnt;i++)
		{
			comset[i]=rem_spaces(comset[i]);
			// printf("comset:%s\n",comset[i]);
		}
		for(int i=0;i<comcnt;i++)
		{
			ex_flag=1;
			// printf("i=%d  %d\n",i,comcnt);
			char *temp=(char *)malloc(sizeof(char)*100);
			int file=check_out_redirection(comset[i],temp);
			// if(comset[i][strlen(comset[i])-1]==' ')
			// 	comset[i][strlen(comset[i])-1]=0;
			// printf("in main: %d\n",ex_flag);
			if(ex_flag==0)
				continue;
			// printf("did not continue\n");
			pipecnt=pipesep(comset[i],pipeset);
			for(int i=0;i<pipecnt;i++)
			{
				pipeset[i]=rem_spaces(pipeset[i]);
				// printf("comset:%s\n",comset[i]);
			}			
			write(0,NULL,0);
			temp=strstr(pipeset[0],"<");
			if(temp)
			{
				*temp=0;
				temp+=1;
				while(*temp==' ')
					temp++;
				pipeset[0]=rem_spaces(pipeset[0]);
				// printf("temp :%s pipeset[0] :%s--\n",temp,pipeset[0]);
				int k=open(temp,O_RDONLY);
				dup2(k,0);
			}
			// int outfile=check_in_redirection(pipeset[0],temp);
			// pipeset[0]=rem_spaces(pipeset[0]);
			// printf("%s--\n",pipeset[0]);
			// if (outfile>0)
			// {
			// 	dup2(outfile,0);
			// 	printf("duped\n");
			// }
			// if(ex_flag==0)
			// 	continue;
			// printf("%s--\n",pipeset[0]);
			for(int j=0;j<pipecnt-1;j++)
			{					
				int pip[2];
				pipe(pip);				
				execute(pipeset[j],pip,hdir,argset);
				close(pip[1]);
				dup2(pip[0],0);
				// printf("executed command\n");													
			}
		    // printf("cameout\n");
			int n[2];
			n[0]=0;
			n[1]=file;
			execute(pipeset[pipecnt-1],n,hdir,argset);
			dup2(temp_in,0);
			dup2(temp_out,1);					
		}
	}
	printf("cameout\n");
	free(host);//= (char *)malloc(sizeof(char)*100);
	free(user);//= (char *)malloc(sizeof(char)*100);
	free(wdir);// = (char *)malloc(sizeof(char)*1000);
	free(hdir);// = (char *)malloc(sizeof(char)*1000);
	free(dir);// = (char *)malloc(sizeof(char)*1000);
    free(input);//= (char *)malloc(sizeof(char)*1000);
	for(int i=0;i<100;i++)
	{
		free(comset[i]);//=(char *)malloc(sizeof(char)*100);
		free(pipeset[i]);//=(char *)malloc(sizeof(char)*100);
		free(argset[i]);//=(char *)malloc(sizeof(char)*100);
	}
}
