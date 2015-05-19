#include "ucode.c"

int main(int argc, char * argv[]){
	char buf[1024];
	char * toMatch;
	char getCh;
	int fd, readBytes,i=0;

	if(argc<3){
		fd=0;
	}
	else{
		//printf("File to grep is %s\n",argv[2]);
		fd = open(argv[2], O_RDONLY);
	}
	if(fd>-1){
		toMatch = argv[1];

		while(readBytes=read(fd,&getCh,1)){
			buf[i++] = getCh;
			if(getCh=='\n'){
				buf[i] = '\r';
				buf[i+1]=0;
				grepLine(buf,toMatch);
				i=0;
			}

		}

	} else{
		printf("Bad file %d\n",fd);
	}
return readBytes;
}

int grepLine(char *buf, char *pattern){
	int i=0,j=0;
	int gbeg = 0;
	int patternLen = strlen(pattern);
	int bufLen = strlen(buf);

	while(i<bufLen){
		if(buf[i]!=pattern[j]){
			i++;
		}
		else{
			//matching
		//	printf("Match\n");
			gbeg = i;
			i++;
			j++;
			while(j<patternLen){
				if(buf[i] != pattern[j]){
					i = gbeg +1;
					j = 0; 
					break;
				}
				i++;j++;
			}
			if(j==patternLen){
				printf("%s",buf);
				break;
			}
		}
	}
}