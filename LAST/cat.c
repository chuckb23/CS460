#include "ucode.c"

int main(int argc,char * argv[]){
	int fd, numread;
	int i =0;
	char buf;
	char stdin[128];

	if(argc ==1){
		fd = 0;
		/*while(1){
			gets(stdin);
			printf("%s\n",stdin);

		}*/
		/*	while(readBytes=read(fd,&buf,1)){
			stdin[i++] = buf
			if(buf=='\n'){
				stdin[i] = '\r';
				stdin[i+1]=0;
				
				i=0;
			}*/
	}
	else{

		fd = open(argv[1],O_RDONLY);
	//	printf("We are going to  Cat %d",fd);
	//	getc();
	}
	if(fd>-1){
		while(read(fd,&buf,1)){
			if(buf == '\0'){
				putc(buf)
				exit(0);
			}
			else if(buf=='\r'){
				//putc('\n');
				putc('\r');		//}
		} 
		else{
			putc(buf);
		}

	}
}
	close(fd);
	exit(0);

}