#include "ucode.c"
char *tty;

main(int argc, char *argv[])   // invoked by exec("login /dev/ttyxx")
{
	char username[32], passwd[32],buf[1024];
	int fd,stdin,stdout,stderr,bytes;
	char *suname,*homedir,*program,*fullname,*spwd,*getUsers;
	char *userStrs[5];
	int gid,uid,userslen;
	int i = 0, j =0;
  tty =  argv[1];

 close(0); close(1); close(2); // login process may run on different terms

 //2. // open its own tty as stdin, stdout, stderr

 stdin = open(tty, O_RDONLY);
 stdout = open(tty, O_WRONLY);
 stderr = open(tty, O_RDWR);

  settty(tty);   // store tty string in PROC.tty[] for putc()

  // NOW we can use printf, which calls putc() to our tty
  printf("Chucks LOGIN : open %s as stdin, stdout, stderr\n", tty);

  signal(2,1);  // ignore Control-C interrupts so that 
                // Control-C KILLs other procs on this tty but not the main sh

 /* while(1){
    1. show login:           to stdout
    2. read user nmae        from stdin
    3. show passwd:
    4. read user passwd

    5. verify user name and passwd from /etc/passwd file

    6. if (user account valid){
          setuid to user uid.
          chdir to user HOME directory.
          exec to the program in users's account
       }
       printf("login failed, try again\n");
  }*/
   while(1){
   	printf("login: ");
   	gets(username);
   	printf("password: ");
   	gets(passwd);
   	fd = open("/etc/passwd/",O_RDONLY);
   	while(bytes=read(fd,buf,256));
   
   getUsers = strtok(buf, "\n");
   while(getUsers!=0){
   	userStrs[i++] = getUsers;
   	getUsers = strtok(0,"\n");
   }
   userslen = i;
   //i=0;
   for(i=0;i<userslen;i++){
   	getUsers = strtok(userStrs[i],":");
   	j=0;
   	while(getUsers){
   		switch(j){
   			case 0:
   				suname = getUsers;
   				//printf("username:(%s)\n",suname );
   				break;
   			case 1:
   				spwd = getUsers;
   				//printf("password:(%s)\n",spwd );
   				break;
   			case 2:
   				gid = atoi(getUsers);
   				break;
   			case 3:
   				uid = atoi(getUsers);
   				break;
   			case 4:
   				fullname = getUsers;
   				break;
   			case 5:
   				homedir = getUsers;
   				break;
   			case 6:
   				program = getUsers;
   				break;
   			}
   		getUsers = strtok(0,":");
   		j++;
   		}
   		if(strcmp(username,suname)==0 && strcmp(passwd,spwd)==0){
   			//print information about user
	   			//and that login was success
	   		printf("LOGIN SUCCEEDS\n\n");
			printf("----- %s's Profile -----\n", fullname);
			printf("username: %s\n", suname);
			printf("password: %s\n", spwd);
			printf("uid: %d\n", uid);
			printf("gid: %d\n", gid);
			printf("homedir: %s\n", homedir);
			printf("program: %s\n", program);
   			//change uid 
   			chuid(uid,gid);
   			//change dir
   			chdir(homedir);
   			//execute program
   			exec(program);
   			break;
   		}
   		//i++;
   	}
   	printf("------Login failed-----\n");
   	printf("username: %s and password: %s are not valid\n",username,passwd);

  }
   
}

