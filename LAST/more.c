#include "ucode.c"
int fd;
int main(int argc, char * argv[]){
	char buf;
	if(argc>1){
		fd = open(argv[1],O_RDONLY);
	}
	printPage(1);
	while(1){
		char c = getc();
		read(fd,&buf,1);
		switch(c){
			case '\r':
				printPage(0);
				break;
			case 'q':
				close(fd);
				exit(0);
			case ' ':
				printPage(1);
				//diplay a full page
		}
	}

}

int printPage(int ptype){
	int i,h;
	char buf;
	for(i=0;i<(23*ptype+1);i++){
		for(h=0;h<80;h++){
			if(!read(fd,&buf,1)){
				exit(0);
			}
			putc(buf);
			if(buf=='\n'||buf=='\r'){
				break;
			}
		}
		//putc('\n');
	}
}