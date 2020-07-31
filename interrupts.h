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
extern char pidset[100][3][100];
extern int pidcnt;
extern int gl_fg;
extern char *argset[100];
extern int argcnt;
void print_ack()
{
//   printf("function called\n");
  int a;
  int pid=waitpid(-1,&a,WNOHANG);
//   printf("exited pid:- %d\n",pid);
  char *pname=(char *)malloc(sizeof(char)*1000);;
  char *n=(char *)malloc(sizeof(char)*1000);;
  for(int i=0;i<pidcnt;i++)
  {
    if(integer(pidset[i][0])==pid)
    {
        pname=pidset[i][1];
        strcpy(n,pidset[i][2]);
        // printf("n=%s\n",n);
        strcpy(pidset[i][2],"-1");
        // printf("pid changed to %s\n",pidset[i][0]);
        break;
    }
  }
  if(!strcmp(n,"1"))
  {
    if(WIFEXITED(a))
    {
	    fflush(stderr);
	    fflush(stdout);
	    fprintf(stderr,"\n%s with pid %d exited normally\n",pname,pid);
	    fflush(stdout);
    }
    else
    {
	    fflush(stderr);
	    fflush(stdout);
   	    fprintf(stderr,"\n%s with pid %d exited abnormally\n",pname,pid);
	    fflush(stdout);
    }
  }
}

void parent_int(int sig)
{
        //printf("parent interrupt\n");
        if(gl_fg!=-1)
        {
                kill(gl_fg,SIGKILL);
                for(int i=0;i<pidcnt;i++)
                {
                  if(gl_fg==integer(pidset[pidcnt][0]))
                  {
                        // strcpy(pidset[i][2],"-1");
                        // fprintf(stderr,"\n%s with pid %s exited abnormally\n",pidset[i][1],pidset[i][0]);
                        break;
                  }    
                }
        }
        return;
}
void parent_stp(int sig)
{   
        // printf("%d\n",gl_fg);
        if(gl_fg!=-1)
        {
                // printf("pgid setting %d %d\n",getpid(),getpgid(getpid()));
                int lk=setpgid(getpid(),getpid());
                // printf("pgid set %d %d %d\n",getpid(),getpgid(getpid()),lk);                         
                kill(gl_fg,SIGTSTP);
                int flag=0;
                int i=0;
                for(i;i<pidcnt;i++)
                {
                        if(integer(pidset[i][0])==gl_fg)
                        {
                                flag=1;
                                break;
                        }
                }
                if(flag==1)
                {
                        // printf("Updated\n");
                        strcpy(pidset[i][2],"0");
                }
                else
                {
                        // printf("Created");
                        sprintf(pidset[pidcnt][0],"%d",gl_fg);
                        strcpy(pidset[pidcnt][1],argset[0]);
                        sprintf(pidset[pidcnt][2],"%d",0);
                        pidcnt++;
                        // printf("added\n");                        
                }
        	// sprintf(pidset[pidcnt][0],"%d",gl_fg);
		// strcpy(pidset[pidcnt][1],argset[0]);
		// sprintf(pidset[pidcnt][2],"%d",0);
		// pidcnt++;
                // printf("added\n");
                // for(int i=0;i<pidcnt;i++)
                // {
                //   if(gl_fg==integer(pidset[pidcnt][0]))
                //   {
                //         strcpy(pidset[i][2],"0");
                //         printf("stopped\n");
                //         break;
                //   }    
                // }
        }
        return;
}
void parent_con(int sig)
{
        if(gl_fg!=-1)
        {
                for(int i=0;i<pidcnt;i++)
                {
                  if(gl_fg==integer(pidset[pidcnt][0]))
                  {
                        strcpy(pidset[i][2],"1");
                        // printf("changed");
                        // fprintf(stderr,"\n%s with pid %s exited abnormally\n",pidset[i][1],pidset[i][0]);
                        break;
                  }    
                }
                kill(gl_fg,SIGCONT);
        	// sprintf(pidset[pidcnt][0],"%d",gl_fg);
		// strcpy(pidset[pidcnt][1],argset[0]);
		// sprintf(pidset[pidcnt][2],"%d",0);
		// pidcnt++;
                // printf("added\n");
                // for(int i=0;i<pidcnt;i++)
                // {
                //   if(gl_fg==integer(pidset[pidcnt][0]))
                //   {
                //         strcpy(pidset[i][2],"0");
                //         printf("stopped\n");
                //         break;
                //   }    
                // }
        }
}
void child_int(int sig)
{
        printf("---------\nchild interrupt\n--------\n");
        return;
}
// void exec(char **argset)
// {
//         char **temp=(char **)malloc(sizeof(char*)*1000);
//         for(int i=0;i<1000;i++)
//                 temp[i]=(char *)malloc(1000);
//         temp=argset;
//         pid_t pid;
//         pid=fork();
//         int flag=1;
//         if(!strcmp(argset[argcnt-1],"&"))
//         {
//                 flag=0;
//                 argset[argcnt-1]=NULL;
//         }
//         if(pid==-1)
//                 fprintf(stderr,"Couldn't create new process (failed forking)\n");
//         else if(pid==0)
//         {
//           argset[argcnt]=NULL;
//           int s1=execvp(*argset,argset);
//           char *temp=(char *)malloc(sizeof(char)*1000);
//           strcpy(temp,"/bin/");
//           strcat(temp,argset[0]);
//           int s2=execvp(temp,argset);
//           strcpy(temp,"/sbin/");
//           strcat(temp,argset[0]);
//           int s3=execvp(temp,argset);
//           strcpy(temp,"/usr/bin/");
//           strcat(temp,argset[0]);
//           int s4=execvp(temp,argset);
//           strcpy(temp,"/usr/sbin/");
//           strcat(temp,argset[0]);
//           int s5=execvp(temp,argset);
//           if(s1<0 && s2<0 && s3<0 && s4<0 && s5<0)
//                   fprintf(stderr,"%s : Command doesn't exist\n",argset[0]);
//           exit(0);
//         }
//         else if(flag)
//                 wait(NULL);
// 	else if(flag==0)
// 	{
// 		//printf("firefox pid:%d\n",pid);
// 		sprintf(pidset[pidcnt][0],"%d",pid);
// 		strcpy(pidset[pidcnt][1],argset[0]);
// 		sprintf(pidset[pidcnt][2],"%d",!flag);
// 		pidcnt++;
// 		signal(SIGCHLD,print_ack);
// 	}
// }