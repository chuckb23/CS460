
/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/
#define PA 9
#define PB 10
#define PC 11
#define PD 12
#define AX  4

/****************** syscall handler in C ***************************/
int kcinth()
{
   u16    segment, offset;
   int    a,b,c,d, r;
   segment = running->uss; 
   offset = running->usp;
	//printf("offset: %d\n",offset);
   /** get syscall parameters from ustack **/
   a = get_word(segment, offset + 2*PA);
   b = get_word(segment, offset + 2*PB);
   c = get_word(segment, offset + 2*PC);
   d = get_word(segment, offset + 2*PD);

   switch(a){
       case 0 : r = running->pid;     break;
       case 1 : r = kps();            break;
       case 2 : r = kchname(b);       break;
       case 3 : r = kkfork();         break;
       case 4 : r = ktswitch();       break;
       case 5 : r = kkwait(b);        break;
       case 6 : r = kkexit(b);        break;
			 case 7 : r = getMyname(b);			break;
			 case 8 : r = khop(b);					break;
   case 90: r = getc();               break;
   case 91: r = putc(b);              break;

   case 99: kkexit(b);                break;
       default: printf("invalid syscall # : %d\n", a); 
   }
   put_word(r, segment, offset + 2*AX);
}


//int color;
extern PROC proc[];

char *hh[ ] = {"FREE   ", "READY  ", "RUNNING", "STOPPED", "SLEEP  ", 
               "ZOMBIE ",  0}; 
int do_ps()
{
   int i,j; 
   char *p, *q, buf[16];
   buf[15] = 0;

   printf("============================================\n");
   printf("  name         status      pid       ppid  \n");
   printf("--------------------------------------------\n");

   for (i=0; i<NPROC; i++){
       strcpy(buf,"               ");
       p = proc[i].name;
       j = 0;
       while (*p){
             buf[j] = *p; j++; p++;
       }      
       prints(buf);    prints(" ");
       
       if (proc[i].status != FREE){
           if (running==&proc[i])
              prints("running");
           else
              prints(hh[proc[i].status]);
           prints("     ");
           printd(proc[i].pid);  prints("        ");
           printd(proc[i].ppid);
       }
       else{
              prints("FREE");
       }
       printf("\n");
   }
   printf("---------------------------------------------\n");

   return(0);
}

#define NAMELEN 32
int kchname(char * y)
{
  char buf[64];
  char *cp = buf;
  int count = 0; 

  while (count < NAMELEN){
     *cp = get_byte(running->uss, y);
     if (*cp == 0) break;
     cp++; y++; count++;
  }
  buf[31] = 0;

  printf("changing name of proc %d to %s\n", running->pid, buf);
  strcpy(running->name, buf); 
  printf("done\n");
}
int getMyname(char myname[64])
{
	int i;
	//char buf[64];
	char *p;
	i =0;
	strcpy(myname,running->name);
	p = myname;
	while(*p!=0){
		i++;
		p++;
		}
	printf("Name is %s which is %d characters long\n", myname, i);
	return i;
}
int copy_image(u16 childsegment){
	u16 end,offset;
	int word;
	end = 0x1000;
	offset = 0;
	while(offset<end)
	{
	word = get_word(running->uss,offset);
	put_word(word, childsegment,offset);
	offset+=1;
		}
	return 1;
}
int khop(u16 newsegment){
	u16 segsize;
	segsize = 0x800; 
	copy_image(newsegment);
	 put_word(0x0200,   newsegment, -2*1);   /* flag */  
   put_word(newsegment,  newsegment, (-2*2-segsize));   /* uCS */  
   put_word(newsegment,  newsegment, (-2*7-segsize));  /* uES */  
   put_word(newsegment,  newsegment, (-2*8-segsize));  /* uDS */
		running->usp = -2*8-segsize;
	printf("Newseg %x",newsegment);
	//printf("\nOffset %u",offset);
	//running->uss = newsegment;
	printf("\nSegment %x",running->uss);
	//running->usp = -2*8;
	printf("\nSegment %x",running->usp);
	running->uss = newsegment;
	//goUmode();
	printf("\nSegment %x",running->uss);
	return 1;
}

int kps()
{
  return do_ps();
}

int kkfork()
{
  return do_kfork("/bin/u1");
}

int ktswitch()
{
   return tswitch();
}

