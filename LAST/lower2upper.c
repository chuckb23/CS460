#include "ucode.c"


int main(int argc, char * argv[]){
	int rfd, wfd;
	char lower;
	char buf, upper;
	if(argc <3){
		rfd =0;
		wfd = 1;
	}
	
	else if(argc==3){
		rfd = open(argv[1],O_RDONLY);
		if(rfd>-1){
			wfd = open(argv[2],O_WRONLY|O_CREAT);
			if(wfd<0){
				printf("bad write file\n");
			}
		}else{
			printf("Bad read file\n");
		}
	} else{
		//too many arguments
	}
	while(read(rfd,&lower,1)){
		//upper = (char)lower;
		//printf("originally : %c\n",lower);
		if(lower>96 && lower<123){
			upper = lower- 32;
		} else{
			upper = lower;
		}//putc(upper);
		//putc(upper);
		//printf("now : %c\n",upper);
		if(upper!='\0'){
			write(wfd,&upper,1);
			//write(wfd,'1',1);
		}
	}

}