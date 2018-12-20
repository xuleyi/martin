#include <sys/ptrace.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define BUFSIZE 30
#define BOOL int
#define TRUE 1
#define FALSE 0

struct CMD{
        int ID;
        int Freq;
        char Name[30];
    } ; 
void sort_freq(struct CMD arr[], int len);
int main()
{
  
    int  status = 0, pid, id,flag1 = 0, line =0;
    struct user_regs_struct uregs;
    struct timespec ts;
    char cmd[32];
    char sysname[BUFSIZE];
    char systime[BUFSIZE];
    FILE *fp,*fo;
    fo = fopen("file.txt","w");
    if (fo == NULL){
	    printf("Error opening file.txt\n");
	    exit(1);
    }
    fprintf(fo,"%10s %10s %8s\n", "TIME","ID","NAME");
    if ((pid=fork())==0) {
        printf("pid = %d, ppid = %d\n", getpid(), getppid());
        ptrace(PTRACE_TRACEME, 0, 0, 0);//no signal sent, childpr continues
        kill(getpid(), SIGINT);//delivers a signal->stop, ready to be traced
        execl("./file", "file", NULL);//-> stop again	
    } else {
        wait(&status);
	printf("ready to trace\n");
	printf("%10s %10s %8s\n", "TIME","ID","NAME");
	ptrace(PTRACE_SYSCALL, pid, 0, 0);//restart childpr, stop at syscall    
	while(1)
	{
		wait(&status);
		if(WIFEXITED(status)){
			printf("exit value: %d\n", WEXITSTATUS(status));
   			fclose(fo);
   			printf("line:%d\n",line);
/*process the result*/
    int N;
    N=line;
    char text[128];
    char *p;
    const char *d=" ";
    int i=0,j=0;
    BOOL flag=TRUE;
    struct CMD mycmd[N];
    int l=N;
    FILE* file = fopen("file.txt", "r"); 
    fgets(text,sizeof(text),file);
    while (fgets(text, sizeof(text), file)) {	
	if(i==0) {
		p = strtok(text,d);
		p=strtok(NULL,d);
       		mycmd[i].ID=atoi(p);
		mycmd[i].Freq=1;
		p=strtok(NULL,d);
		strcpy(mycmd[i].Name,p);		
		}
	else {
		p = strtok(text,d);
		p=strtok(NULL,d);
       		mycmd[i].ID=atoi(p);
		for(j=0;j<i;j++){
			if (mycmd[i].ID==mycmd[j].ID){
		    		mycmd[j].Freq=mycmd[j].Freq+1;
		    		i--;
				l--;
				flag = FALSE;
				break;
			}
		}
		if (flag){
			mycmd[i].Freq=1;
	    	    	p=strtok(NULL,d);
	    	    	strcpy(mycmd[i].Name,p);	
		}
	}
		i++;
		flag = TRUE;
}

sort_freq(mycmd,l);

       for(  i = 0; i < l; i++ )
        {
		printf("ID[%d]: %d\n",i,mycmd[i].ID);
		printf("Freq[%d]: %d\n",i,mycmd[i].Freq);
		printf("NAME[%d]: %s\n",i,mycmd[i].Name);
	   }	
//printf("size of l:%d\n",l);
//printf("length of arrary : %ld\n",sizeof(mycmd)/sizeof(mycmd[0]));

/*process the result*/
			return 0;
		}
	if(flag1 == 0){
		ptrace(PTRACE_GETREGS, pid, 0, &uregs);//extract the syscall #
		id = uregs.orig_rax;
		snprintf(cmd,sizeof(cmd),"ausyscall %d",id);
		if ((fp = popen(cmd, "r")) == NULL) {
			printf("Error opening pipe!\n");
			return -1;
			}
		while(fgets(sysname, BUFSIZE, fp) != NULL) {
			timespec_get(&ts, TIME_UTC);
			strftime(systime, sizeof systime, "%T",
				       	localtime(&ts.tv_sec));
			printf("%s.%09ld %-5d %s",systime,ts.tv_nsec,
				       id, sysname);
			fprintf(fo,"%s.%09ld %-5d %s",systime,ts.tv_nsec,
				       id, sysname);
			line++;
		}
		if(pclose(fp)){
			printf("Command not found or error status\n");
			return -1;
			}						
		flag1 = 1;
	}else{
		flag1 = 0;
		}
	ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	}
    }
}

void sort_freq(struct CMD arr[], int len) {
	int i, j;
	struct CMD temp;
	for (i = 0; i < len - 1; i++)
		for (j = 0; j < len - 1 - i; j++)
			if (arr[j].Freq < arr[j + 1].Freq) {
			        temp = arr[j];
		                arr[j] = arr[j + 1];
		                arr[j + 1] = temp;
		               }
}

