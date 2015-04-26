#include "ucode.c"
int pid, status, child, childS0, childS1;
int stdin,stdout;

main(int argc, char *argv){
	open("/dev/tty0/",O_RDONLY);
	open("/dev/tty0/",O_WRONLY);
	printf("Chucks init: FOrk login taks on console\n");
	child = fork();
	if(child)
		parent();
	else
		login();
}

int login(){
	exec("login /dev/tty0");
}

int parent(){
	childS0 = fork();
	if(childS0){
		chilS1 = fork();
		if(childS1)
			return
		else{
			exec("login /dev/ttyS1");
		}
	}
	else{
		exec("login /dev/ttyS0");
	}
	while(1){
		printf("Chucks init waiting......");
		pid = wait(&status);
		if(pid == child){
			//fork another process
			child = fork();
		}
		else
			printf("INit: Buried an orphan child %d \n",pid);
	}
}