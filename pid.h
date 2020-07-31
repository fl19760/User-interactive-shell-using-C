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
long long int integer(char *arr)
{
  long long int a=0,len,pow=1;
  for(int i=1;i<strlen(arr);i++)
	  pow*=10;
  len=strlen(arr);
  for(int i=0;i<len;i++)
  {
    a+=(arr[i]-48)*pow;
    pow/=10;
  }
  return a;
}
int proc_state(int pid,char *c)
{
		// printf("called\n");
		char *temp1=(char *)malloc(sizeof(char)*100);
		char *temp2=(char *)malloc(sizeof(char)*100);
		char *temp=(char *)malloc(sizeof(char)*100000);
		strcpy(temp,"/proc/");
		sprintf(temp1,"%d",pid);
		strcat(temp1,"/status");
		strcat(temp,temp1);
		int f=open(temp,O_RDONLY);
		// printf("f=%d\n",f);
		if(f==-1)		
		{
			free(temp);free(temp1);free(temp2);
			return -1;
		}
		strcpy(temp1,"State");
		strcpy(temp2,"VmSize");
		read(f,temp,100000);
		// printf("came here1\n");
		char *cp_temp1 = temp1, *cp_temp2 = temp2;
		temp1=strstr(temp,temp1);
		temp2=strstr(temp,temp2);
		// printf("pid -- %d\n",pid);
		int i=0;
		while(1)
		{
			if(temp1[i]=='\n')
			{temp1[i]=0;break;}
			i++;
		}
		// printf("came here2\n");
		// printf("Process Status -- %.12s\n",temp1+7);
		*c=*(temp1+7);
		if(temp!=NULL) free(temp);
		if(cp_temp1!=NULL)free(cp_temp1);
		if(cp_temp2!=NULL)free(cp_temp2);
		close(f);
}
 void pinfo(char **argset,int argcnt,int pid)
 {
		char *temp1=(char *)malloc(sizeof(char)*100);
		char *temp2=(char *)malloc(sizeof(char)*100);
		char *temp=(char *)malloc(sizeof(char)*100);
		strcpy(temp,"/proc/");
		sprintf(temp1,"%d",pid);
		strcat(temp1,"/status");
		strcat(temp,temp1);
		int f=open(temp,O_RDONLY);
		if(f==-1)
		{
			fprintf(stderr,"no process with given pid!!\n");
			return;
		}
		strcpy(temp1,"State");
		strcpy(temp2,"VmSize");
		read(f,temp,100000);
		char *cp_temp1 = temp1, *cp_temp2 = temp2;
		temp1=strstr(temp,temp1);
		temp2=strstr(temp,temp2);
		printf("pid -- %d\n",pid);
		int i=0;
		while(1)
		{
			if(temp1[i]=='\n')
			{temp1[i]=0;break;}
			i++;
		}
		printf("Process Status -- %.12s\n",temp1+7);
		i=0;
		while(1)
		{
			if(temp2[i]=='\n')
			{temp2[i]=0;break;}
			i++;
		}
		printf("memory -- %s\n",temp2+10);
		strcpy(temp,"/proc/");
		sprintf(temp1,"%d",pid);
		strcat(temp,temp1);
		strcat(temp,"/cmdline");
		close(f);
		f=open(temp,O_RDONLY);
		read(f,temp,100000);
		printf("Executable path -- %s\n",temp);
		// exit(0);
		if(temp!=NULL) free(temp);
		if(cp_temp1!=NULL)free(cp_temp1);
		if(cp_temp2!=NULL)free(cp_temp2);
		close(f);
		return;
 }
