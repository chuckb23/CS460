#include "ucode.c"
int pid, status, child, childS0, childS1;
int stdin,stdout;

main(int argc, char *argv){
	open("/dev/tty0/",O_RDONLY);
	open("/dev/tty0/",O_WRONLY);
	printf("Chucks init: Fork login taks on console\n");
	child = fork();
	if(child){
		childS0 = fork();
		if(childS0){
			childS1 = fork();
			if(childS1)
				parent();
			else{
				exec("login /dev/ttyS1");
			}
		}
		else{
			exec("login /dev/ttyS0");
		}
		//parent();
	}
	else{
		login();
	}
}

int login(){
	exec("login /dev/tty0");
}

int parent(){
	
	while(1){
		printf("Chucks init waiting......");
		pid = wait(&status);
		if(pid == child){
			//fork another process
			child = fork();
			if(!child)
				exec("login /dev/tty0");
		}else if(pid == childS0){
			childS0 = fork();
			if(!childS0)
				exec("login /dev/ttyS0");
		}else if(pid == childS1){
			childS1 = fork();
			if(!childS1)
				exec("login /dev/ttyS1");
		}else{
			printf("Init: Buried an orphan child %d \n",pid);
		
		}
	}
}