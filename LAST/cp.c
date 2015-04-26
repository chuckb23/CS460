#include "ucode.c"

int main(int argc, char * argv[]){
	int readFD,writeFD,bytesRead,totalTD,bytesWrite;
	char buf[1024];
	if(argc<3){
		printf("CP command not recognizd no destination\n");
	}
	else{
		readFD = open(argv[1],O_RDONLY);
		if(readFD>-1){
				writeFD = open(argv[2],O_WRONLY|O_CREAT);
				if(writeFD>-1){
					while(bytesRead=read(readFD,buf,1024)){
						totalTD += bytesRead;
						bytesWrite = write(writeFD,buf,bytesRead);
					}
					close(readFD);
					close(writeFD);
					printf("Wrote %d bytes from %s to %s\n",totalTD,argv[1],argv[2]);

				}else{
					printf("Bad write file\n");
				}
		}else{
			printf("Bad file\n");
		}

	}
}