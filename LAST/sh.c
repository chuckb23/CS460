#include "ucode.c"

char command[256];
int cmdCount,fd[2];
char *cmdList[16];
int status;

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
		//if(strcmp(command))

		//one command
		cmdCount = getPipes();
		if(cmdCount){
			//printf("Executing multiple %s\n",command);
			//getc();
			pid = fork();
			//printf("D1\n");
			//getc();
			if(pid==0){
				//printf("D2 command is %s\n",cmdList[cmdCount-1]);
				//getc();
				//setPipes();
				fileIO(command);
				//chuckexec(command);
				//printf("D3 command is %s\n",cmdList[cmdCount-1]);
			    //getc();
				//exec(cmdList[cmdCount-1]);
				//pipe each cmd
			}else{
				pid = wait(&status);
				//wait(&status);
			}
		} else{
			printf("Error: %s is not a valid command\n",command);
		}
	}
}

int getPipes(){
	char tmp[256];
	char *pipes;
	int pipeNum=0;
	strcpy(tmp,command);
	pipes = strtok(tmp," ");
	while(pipes){
		cmdList[pipeNum++] = pipes;
		//printf("CMD: %s\n",cmdList[0]);
		pipes = strtok(0," ");
	}

	//gets();
	cmdList[pipeNum]='\0';
	return pipeNum;
}

int setPipes(){
	int i =cmdCount-2;
	int pid, save,ttyFD;
	char tty[64];
	printf("S1\n");
	getc();
	while(i>0){
		pipe(fd);
		printf("S2 cmds = %d\n",cmdCount);
		getc();
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
	int fdf,pid,cpid;
	char *fileName;
	char *pipeCmd = command;
	int fileOp=0,i=0;

	//pid = fork();
	
	
		do{
			//check if output to file
			if(*cmdList[i] == '>'){
				printf("Outputing contents to file\n");
				//check if appending
				if(*(cmdList[i]+1)=='>'){
					printf("Appending to file\n");
					fileOp = 3;
					//get filename
					fileName=cmdList[++i];
					//open file
					fdf = open(fileName,O_WRONLY|O_APPEND|O_CREAT);
					if(fdf>-1){
						//*(cmdList[i]-1)='\0';
						dup2(fdf,1);
						close(fdf);
					}

				}else{
					if(fileOp==0){
						fileOp == 1;
						fileName = cmdList[++i];
						fdf = open(fileName,O_WRONLY|O_CREAT);
						if(fdf>-1){
							//*(cmdList-1)='\0';
							dup2(fdf,1);
							close(fdf);
						}
						else{
							//bad file descriptor
						}
					}
				}//Input
			}else if(*cmdList[i] == '<'){
				fileOp = 2;
				fileName = cmdList[++i];
				close(0);
				fdf = open(fileName,O_RDONLY);
				/*if(fdf>-1){
					//*(cmdList-1)='\0';
					dup2(fdf,0);
					close(fdf);
				}
				else{
					//bad 
					break;
				}*/
			} else if(*cmdList[i] == '|'){
				//printf("Command %s\n",cmdList[i]);
				pipe(fd);
				cpid=fork();
				//printf("Forked child process %d\n",cpid);
				//write(2,cpid,2);
				if(cpid==0){
					pipeCmd = command;
					close(fd[1]);
					close(0);
					dup2(fd[0],0);
				}
				else{
					cmdList[i+1]=0;
					close(fd[0]);
					close(1);
					dup2(fd[1],1);
					//exec(command);
					//wait(&status);
				}
			} else{
				strcpy(pipeCmd,cmdList[i]);
				pipeCmd += strlen(cmdList[i]);
				*(pipeCmd++) = ' ';
				//printf("Command %s\n",cmdList[i]);
				//printf("Forked child process %d\n",cpid);
				//getc();
			}
			//cmdList++;
			i++;
		}while(cmdList[i] !=0);
		*(pipeCmd-1) = 0;
		//printf("Exec the command %s\n",command);
		//getc();
		exec(command);
		exit(&command);
		return fileOp;
	
}