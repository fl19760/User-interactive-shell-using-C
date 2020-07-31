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
extern char *wdir;
extern char pidset[100][3][100];
extern int pidcnt;
extern int gl_fg;
void execute(char *command,int pip[2],char *hdir,char **argset)
{
	// printf("came here to start execution1 %s\n",command);
	
	int argcnt=argsep(command,argset);
	pid_t ppid=getpid();
	// printf("came here to start execution2 %s\n",command);
	if(!strcmp(argset[0],"quit"))
	{
		exit(0);
	}
	else if(!strcmp(argset[0],"cd"))
	{
		wdir=cd(argset,wdir,hdir,argcnt);
		return ;
	}
	else if(!strcmp(argset[0],"setenv"))
	{
		if(argcnt==1 || argcnt>3)
		{
			fprintf(stderr,"Invalid no of arguments for setenv!!\n");
			return;
		}
		char *temp=(char *)malloc(sizeof(char)*100);
		strcpy(temp,argset[1]);
		strcat(temp,"=");
		if(argcnt==3)
			strcat(temp,argset[2]);
		putenv(temp);
		return;
	}
	else if(!strcmp(argset[0],"getenv"))
	{
		printf("%s\n",getenv(argset[1]) ? getenv(argset[1]) : "");
		return;
	}
	else if(!strcmp(argset[0],"unsetenv"))
	{
		if(argcnt!=2)
		{
			fprintf(stderr,"Invalid no.of arguments for unsetenv!!\n");
			return;
		}
		unsetenv(argset[1]);
		return;
	}
	else if(!strcmp(argset[0],"fg"))
	{
		if(argcnt!=2)
		{
			fprintf(stderr,"inavlid no. of arguments!!\n");
			return;
		}
		pid_t pid;
		// pidnumber=integer(argset[1]);
		int j=0;
		int i;
		for(i=0;i<pidcnt;i++)
		{
			if(strcmp(pidset[i][2],"-1"))
			{
				j++;
			}
			if(j==integer(argset[1]))
				break;
		}
		pid=integer(pidset[i][0]);
		kill(pid,SIGCONT);
		char stdin_path[100];
		sprintf(stdin_path,"/proc/%d/fd/0",pid);
		int tmp_in=dup(0);
		int new_stdin=open(stdin_path,O_APPEND);
		dup2(new_stdin,0);
		int sus_flag=0;
		gl_fg=pid;
		// printf("%d\n",pid);
		while(1)
		{
			char c;
			int yes=proc_state(pid,&c);
			if(yes==-1 || c=='Z')
				break;
			else if(c=='T')
			{	
				sus_flag=1;
				break;
			}
		}
		dup2(tmp_in,0);
		close(tmp_in);
		close(new_stdin);
		gl_fg=-1;
		// printf("susflag=%d\n",sus_flag);
		if(sus_flag)
			strcpy(pidset[i][2],"0");
		else
			strcpy(pidset[i][2],"-1");
		return;
		
	}
	else if(!strcmp(argset[0],"bg"))
	{
		if(argcnt !=2)
		{
			fprintf(stderr,"inavlid no. of arguments!!\n");
			return;
		}
		pid_t pid;
		int j=0;
		int i;
		for(i=0;i<pidcnt;i++)
		{
			if(strcmp(pidset[i][2],"-1"))
			{
				j++;
			}
			if(j==integer(argset[1]))
				break;
		}
		// tcsetpgrp(0,getpgid(pid));
		pid=integer(pidset[i][0]);
		int lk=setpgid(pid,pid);
		// tcsetpgrp(0,getpgid(ppid));
		// printf("pid: %d\n",pid);
		kill(pid,18);
		strcpy(pidset[i][2],"1");
		// printf("pgid setting %d %d\n",pid,getpgid(pid));
		// printf("pgid set %d %d %d\n",pid,getpgid(pid),lk);
		return;
	}
	else if(!strcmp(argset[0],"overkill"))
	{
		for(int i=0;i<pidcnt;i++)
		{
			if(strcmp(pidset[i][2],"-1"))
				kill(integer(pidset[i][0]),SIGKILL);
		}
		return;
	}
	else if(!strcmp(argset[0],"kjob"))
	{
		if(argcnt!=3)
		{
			fprintf(stderr,"inavlid no. of arguments!!\n");
			return;
		}
		int j=0;
		int i;
		for(i=0;i<pidcnt;i++)
		{
			if(strcmp(pidset[i][2],"-1"))
			{
				j++;
			}
			if(j==integer(argset[1]))
				break;
		}
		// printf("pid=%lld\n",integer(pidset[i][0]));
		kill(integer(pidset[i][0]),integer(argset[2]));
		sleep(1);
		char str[1];
		char a;
		int yes=proc_state(integer(pidset[i][0]),&a);
		// printf("char: %c\n",a);
		str[0]=a;
		// printf("%s\n",str);
		if(yes==-1 || a=='Z')
			strcpy(pidset[i][2],"-1");
		else if(a=='S' || a=='R')
			strcpy(pidset[i][2],"1");
		else if(a=='T')
			strcpy(pidset[i][2],"0");
		// printf("%s %s\n",pidset[i][0],pidset[i][2]);
		return;
	}
	// printf("came here to be executed1\n");
	// char **temp=(char **)malloc(sizeof(char*)*100);
    // for(int i=0;i<100;i++)
    //     temp[i]=(char *)malloc(100);
	// printf("came here to be executed2\n");
    // temp=argset;
	// signal(SIGINT,child_int);
    int flag=1;
	if(!strcmp(argset[argcnt-1],"&"))
    {
        flag=0;
        argset[argcnt-1]=NULL;
		argcnt--;
    }
	pid_t pid;
    pid=fork();
	gl_fg=pid;
	// printf("came here to be execute3\n");
    if(pid==-1)
        perror("fork failed");
		// fprintf(stderr,"Couldn't create new process (failed forking)\n");
    else if(pid==0)
    {
	//	signal(SIGINT,&child_int);
		// signal(SIGTSTP,child_int);		
		dup2(pip[1],1);
		if(!strcmp(argset[0],"pwd"))
		{
			getcwd(wdir,1000);
			printf("%s\n",wdir);
			wdir=telda_rep(wdir,hdir);
			exit(0);
		}
		else if(!strcmp(argset[0],"echo"))
		{
			for(int i=1;i<argcnt;i++)
				printf("%s ",argset[i]);
			printf("\n");	
			exit(0);
		}
		else if(!(strcmp(argset[0],"ls")))
		{
			// dup2(haha,1); 
			// printf("came here for ls\n");
			// dup2(pip[1],1);
			// printf("in ls:%s--- %s---- %d\n",argset[0],argset[1],argcnt);
			if(argcnt==1)
				ls(argset,hdir,argcnt);
			else if(argcnt ==2)
			{
				if(argset[1][0]!='-')
					ls(argset,hdir,argcnt);
				else if(!strcmp(argset[1],"-a"))
					ls_a(argset,hdir,argcnt);
				else if(!strcmp(argset[1],"-l"))
					ls_l(argset,hdir,argcnt);
				else if(!strcmp(argset[1],"-la") || !strcmp(argset[1],"-al"))
					ls_la(argset,hdir,argcnt);
				else
					fprintf(stderr,"Invalid flag(s)!!\n");
			}
			else if(argcnt==3)
			{
				if(argset[2][0]!='-' && !strcmp(argset[1],"-a"))
					ls_a(argset,hdir,argcnt);
				else if(argset[2][0]!='-' && !strcmp(argset[1],"-l"))
					ls_l(argset,hdir,argcnt);
				else if((!strcmp(argset[2],"-l") && !strcmp(argset[1],"-a")) || (!strcmp(argset[1],"-l") && !strcmp(argset[2],"-a")))
					ls_l_a(argset,hdir,argcnt);
				else
					fprintf(stderr,"Invalid flag(s)!!\n");
			}
			else if(argcnt==4)
			{
				if((!strcmp(argset[2],"-l") && !strcmp(argset[1],"-a")) || (!strcmp(argset[1],"-l") && !strcmp(argset[2],"-a")))
					ls_l_a(argset,hdir,argcnt);
				else
					printf("Invalid flag(s)!!\n");
			}
			else
				fprintf(stderr,"Invalid flag(s)!!\n");
			// printf("came to ls\n");s
			exit(0);
		}
		else if(!strcmp(argset[0],"pinfo"))
		{	
			pid_t pid;
			if(argcnt==1)
				pid=ppid;
			else
				pid=integer(argset[1]);
			pinfo(argset,argcnt,pid);
			exit(0);
		}
		else if(!strcmp(argset[0],"jobs"))
		{
			int it=1;
			char a;
			// printf("getpgid:- %d\n",getpgid(integer(pidset[0][0])));
			for(int i=0;i<pidcnt;i++)
			{
				if(strcmp(pidset[i][2],"-1") && !proc_state(integer(pidset[i][0]),&a) && a!='Z')
				{
					// printf("%s %s\n",pidset[i][0],pidset[i][2]);
					if(!strcmp(pidset[i][2],"0"))
					{ 
						printf("[%d] Stopped %s [%s]\n",it++,pidset[i][1],pidset[i][0]);
					}
					else if(!strcmp(pidset[i][2],"1") && !proc_state(integer(pidset[i][0]),&a))
					{
						printf("[%d] Running %s [%s]\n",it++,pidset[i][1],pidset[i][0]);
					}
				}
			}
			exit(0);
		}
		else if(!(strcmp(argset[0], "")))
    			exit(0);	
		else
		{
			// char *inp=(char *)malloc(sizeof(char)*100);
			// read(0,inp,10000);
			// printf("inp :%s for %s\n",inp,argset[0]);
			
			argset[argcnt]=NULL;
        	int s1=execvp(*argset,argset);
        	char *temp=(char *)malloc(sizeof(char)*1000);
        	strcpy(temp,"/bin/");
        	strcat(temp,argset[0]);
        	int s2=execvp(temp,argset);
        	strcpy(temp,"/sbin/");
        	strcat(temp,argset[0]);
        	int s3=execvp(temp,argset);
        	strcpy(temp,"/usr/bin/");
        	strcat(temp,argset[0]);
        	int s4=execvp(temp,argset);
        	strcpy(temp,"/usr/sbin/");
        	strcat(temp,argset[0]);
        	int s5=execvp(temp,argset);
        	if(s1<0 && s2<0 && s3<0 && s4<0 && s5<0)
            	    fprintf(stderr,"%s : Command doesn't exist\n",argset[0]);
			// printf("exiting\n");
        	exit(0);
		}
		close(pip[1]);
		exit(0);		
    }
    else if(flag)
    {
		waitpid(pid,NULL,WUNTRACED);
		gl_fg=-1;
	}
	else if(flag==0)
	{
		setpgid(pid,pid);
		sprintf(pidset[pidcnt][0],"%d",pid);
		strcpy(pidset[pidcnt][1],argset[0]);
		sprintf(pidset[pidcnt][2],"%d",!flag);
		pidcnt++;
		signal(SIGCHLD,print_ack);
		gl_fg=-1;
	}
}	