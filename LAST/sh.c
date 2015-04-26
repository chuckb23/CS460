#include "ucode.c"

char command[256];
int cmdCount,fd[2];
char *cmdList[16];


int main(int argc, char *argv[]){
	//int numCmds;
	while(1){
		printf("ccbsh # : ");
		gets(command);
		if(strcmp(command,"logout")==0){
			printf("Logout...Goodbye\n");
			exit(&command);
			//syscall to die
		}

		//one command
		cmdCount = getPipes();
		if(cmdCount==1){
			printf("Executing %s\n",command);
			getc();
			pid = fork();
			if(pid==0){
				fileIO(cmdList[0]);
				exec(cmdList[0]);
			}
			else{
				pid = wait(getpid());
			}
		} else if(cmdCount>1){
			pid = fork();
			if(pid==0){
				setPipes();
				exec(cmdList[cmdCount-1]);
				//pipe each cmd
			}else{
				pid = wait(getpid());
			}
		} else{
			printf("Error: %s is not a valid command\n",command);
		}
	}
}

int getPipes(){
	char * pipes;
	int pipeNum=0;

	pipes = strtok(command,"|");
	while(pipes){
		cmdList[pipeNum++] = pipes;
		printf("CMD: %s\n",cmdList[pipeNum]);
		pipes = strtok(0,"|");
	}

	gets();
	pipes[pipeNum]=0;
	return pipeNum;
}

int setPipes(){
	int i =cmdCount-2;
	int pid, save,ttyFD;
	char tty[64];
	while(i<cmdCount){
		pipe(fd);
		pid = fork();
			if(pid==0){
				close(fd[0]);
				dup2(fd[1],1);
				close(fd[1]);
				save = dup(1);
				close(1);
				gettty(tty);
				ttyFD = open(tty,O_WRONLY);
				if(ttyFD==1){
					close(1);
					dup(save);
					close(save);
				}
				exec(cmdList[i]);

			}
			else{
					close(fd[1]);
					dup2(fd[0],0);
					close(fd[0]);
					i=i-2;

			}
		}
	}


int fileIO(char * cmd){
	char * tmp = cmd;
	char *fileName;
	int fileOp=0,fd;
	do{
		//check if output to file
		if(*tmp == '>'){
			printf("Outputing contents to file\n");
			//check if appending
			if(*(tmp+1)=='>'){
				printf("Appending to file\n");
				fileOp = 3;
				//get filename
				fileName=tmp+3;
				//open file
				fd = open(fileName,O_WRONLY|O_APPEND|O_CREAT);
				if(fd>-1){
					*(tmp-1)='\0';
					dup2(fd,1);
					close(fd);
				}

			}else{
				if(fileOp==0){
					fileOp == 1;
					fileName = tmp+2;
					fd = open(fileName,O_WRONLY|O_CREAT);
					if(fd>-1){
						*(tmp-1)='\0';
						dup2(fd,1);
						close(fd);
					}
					else{
						//bad file descriptor
					}
				}
			}//Input
		}else if(*tmp == '<'){
			fileOp = 2;
			fileName = tmp+2;
			fd = open(fileName,O_RDONLY);
			if(fd>-1){
				*(tmp-1)='\0';
				dup2(fd,0);
			}
			else{
				//bad 
				break;
			}
		}
		tmp++;
	}while(*tmp !='\0');
	return fileOp;
}