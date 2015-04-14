#include "type.h"

PROC proc[NPROC], *freeList, *running, *readyQueue, *sleepList;
int procSize = sizeof(PROC);

char *pname[]={"Sun","Mercury", "Venus", "Earth",  "Mars", "Jupiter", 
               "Saturn",  "Uranus","Neptune"};
int nproc;

#include "wait.c"
#include "kernel.c"
#include "int.c"
#include "pv.c"
#include "serial.c"

int body();

int init()
{
  int i,j,fd; PROC *p;
   for (i=0; i<NPROC; i++){
     p = &proc[i];
       p->pid = i;
       p->status = FREE;
       p->next = &proc[i+1];
       strcpy(p->name, pname[i]);
       printf("%d %s ", p->pid, proc[i % 4].name);
   }
   printf("\n");

   proc[NPROC-1].next = 0;
   freeList = &proc[0];   
   readyQueue = 0;
   sleepList = 0;

   p = get_proc(&freeList);
   p->priority = 0;
   p->status = READY;
   p->parent = p;
   p->inkmode = 1;
   running = p;
   nproc = 1;
}

int int80h(), s0inth(), s1inth();

int set_vec(u16 vector, u16 addr)
{ 
    put_word(addr,  0, vector << 2);
    put_word(0x1000,0, (vector << 2) + 2);
}

main()
{
      int i, pid, status;  char c, d;
      printf("Welcome to the 460 Multitasking System\n");

      printf("initializing ......\n");
        init();
      printf("initialization complete\n");

      set_vec(80, int80h);

      kfork("/bin/u1"); 
      
      set_vec(12, s0inth);
      sinit();
      printQueue(readyQueue);

      while(1){
        if (readyQueue)
           tswitch();
      }
      printf("all dead, happy ending\n");
}
int body()
{
  char c, CR, buf[64];
  while(1){
    printf("=======================================\n");
    printQueue(readyQueue);
    printf("proc %d %s in Kmode\n", running->pid, running->name);
    printf("input a command (s|f|u|q|i|o) : ");
    c=getc(); putc(c); CR=getc(); putc(CR);
    switch(c){
      case 's' : tswitch(); break;
      case 'u' : printf("\nProc %d ready to go U mode\n", running->pid);
      goUmode(); break;
      case 'f': fork(); break;
      case 'q' : kexit(); break;
      case 'i' : iline(); break;
      case 'o' : oline(); break;
      } 
    }
}

int scheduler()
{ 
    if (running->status == READY)
       enqueue(&readyQueue,running);
    running = dequeue(&readyQueue);
}

int oline()
{
// assume port = 0;
  sputline(0, "serial line from Kmode\n\r");
}
char kline[64];
int iline()
{
  sgetline(0, kline);
  printf("\nkline=%s\n", kline);
}