#include "ucode.c"

int main(int argc,char * argv[]){
	int fd, numread;
	char buf;
	char tty[128];

	if(argc ==1){
		fd = 0;
	}
	else{

		fd = open(argv[1],O_RDONLY);
	//	printf("We are going to  Cat %d",fd);
	//	getc();
	}
	if(fd>=0){
		while(numread = read(fd,&buf,1)){
			if(buf!='\r'){
				putc(buf);
			}
		}
	}

	return 0;

}