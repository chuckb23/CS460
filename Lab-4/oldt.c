#define NPROC    9
#define SSIZE 1024

/******* PROC status ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5
#define NULL     0
typedef struct proc{
    struct proc *next;
    int    *ksp;
    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
  struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    int    kstack[SSIZE];      // per proc stack area
}PROC;

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList, *procList;
int procSize = sizeof(PROC);
int nproc = 0;
int color;

/*#include "queue.c"
#include "wait.c"
#include "kernel.c"*/
int body();

void printQueue(PROC *queue){
	PROC *p = queue;
	while(p!=NULL){
		printf(" [ %d ] -> ",p->pid);
		p = p->next;
	}
	return;
}

PROC *dequeue(PROC **queue){
	
	PROC *p = *queue;
	//printQueue(p);
	if(*queue != NULL)
		*queue = (*queue)->next;
	return p;
}

PROC *get_proc(){
	if(freeList == NULL){
		return NULL;	
	}
	else{
		return dequeue(&freeList);
		}
}

int init()
{
       PROC *p;
       int i;

       printf("init ....");

       for (i=0; i<NPROC; i++){   // initialize all procs
           p = &proc[i];
           p->pid = i;
           p->status = FREE;
           p->priority = 0;     
           p->next = &proc[i+1];
       }
       freeList = procList =  &proc[0];      // all procs are in freeList
       proc[NPROC-1].next = 0;
       readyQueue = sleepList = 0;

       /**** create P0 as running ******/
       p = get_proc(&freeList);
       p->status = RUNNING;
       running = p;
       nproc++;
       printf("done\n");
} 
void enqueue(PROC **queue,PROC *p){
	PROC *head = *queue, *tmp = NULL;
	//printQueue(head);
	if(*queue == NULL || p->priority > (*queue)->priority){
		*queue = p;
		p->next = head;
		return;
		}
	tmp = (*queue)->next;
	while(tmp!=NULL && p->priority <= tmp->priority){
		head = tmp;
		tmp = head->next;
	}
	head->next = p;
	p->next = tmp;
	return;
	
}





int scheduler()
{
  if (running->status == READY)
      enqueue(&readyQueue, running);
  running = dequeue(&readyQueue);
  color = 0x000A + (running->pid % 6);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    kfork();     // P0 kfork() P1
    while(1){
      printf("P0 running\n");
      if (nproc==2 && proc[1].status != READY)
	  		printf("no runable process, system halts\n");
      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();   // P0 switch to run P1
				
   }
}

int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
            //print freeList;
            // print readyQueue;
            // print sleepList;
    printf("-----------------------------------------\n");

    printf("proc %d[%d] running: parent=%d\n",
	   running->pid, running->priority, running->ppid);

    printf("enter a char [s|f|q| p|z|a| w ] : ");
    c = getc(); printf("%c\n", c);
  
    switch(c){
       case 's' : tswitch();   break;
       case 'f' : kfork();     break;
       case 'q' : do_exit();      break; 
       case 'p' : do_ps();        break; 
       case 'z' : do_sleep();     break; 
       case 'a' : do_wakeup();    break; 
       case 'w' : do_wait();      break;
     }
  }
}

int do_exit(){
	char c;
	int exitval;
	printf("Enter an exit value: ");
	c = getc();
	exitval = c - '0';
	kexit(exitval);
}

int do_sleep(){
	char c;
	int sleep;
	printf("Enter an sleep value: ");
	c = getc();
	sleep = c - '0';
	ksleep(sleep);
}
int do_wakeup(){
	char c;
	int wake;
	printf("Enter an wakeup value: ");
	c = getc();
	wake = c - '0';
	kwakeup(wake);
}
int do_wait(){
	char c;
	int wait;
	printf("Enter an wait value: ");
	c = getc();
	wait = c - '0';
	kwait(wait);
}

int kexit(int val){
	int i;
	running->status = ZOMBIE;
	running->exitCode = val;
	/*tmp = procList;
	while(tmp!=NULL){
		if(tmp->parent == running){
			tmp->parent = running->parent;
			}
		tmp = tmp->next;
		}*/
	
	for(i = 0;i<NPROC;i++){
		if(proc[i].parent == running){
			proc[i].parent = running->parent;
			}
		}
	kwakeup(running->parent);
	printf("Woke up parent %d \n",running->parent->pid);
	tswitch();
}

int ksleep(int val){
	running->event = val;
	running->status = SLEEP;
	enqueue(&sleepList,running);	
	tswitch();
}
int kwait(int *status){
	int children = 0, i;
	for(i =0;i<NPROC;i++){
		if(proc[i].ppid == running->pid){
			children++;
			}
		}
	if(children == 0){
		return -1;
		}
	else{
		while(1){
			for(i=0;i<NPROC;i++){
				if(proc[i].status == ZOMBIE && proc[i].ppid == running->pid){
					status = proc[i].exitCode;
					enqueue(&freeList,proc[i]);
					return proc[i].pid;
					}
				}
			sleep(running);
		}
	}
}
					
int kwakeup(int val){
	int i;
	/*PROC *tmp = procList;
	while(tmp!=NULL){
		if(tmp->status == SLEEP && tmp->event==val){
			dequeue(&sleepList);
			tmp->status = READY;
			enqueue(&readyQueue, tmp);
			}
		tmp=tmp->next;
	}*/
	for(i=0;i<NPROC;i++){
		if(proc[i].status == SLEEP && proc[i].event == val){
			dequeue(&sleepList);
			proc[i].status = READY;
			enqueue(&readyQueue, proc[i]);
			}
		}
	return 1;
}
int do_ps(){
	printf("-------------------------------------\n");
	printf("Free Processes = ");
	printQueue(freeList);
	printf("\nReady Processes = ");
	printQueue(readyQueue);
	printf("\nSleeping Processes = ");
	printQueue(sleepList);
}


/***********************************************************
  Write YOUR C code for
        ksleep(), kwakeup()
        kexit()
        kwait()

Then, write your C code for
      do_ps(), do_sleep(), do_wakeup(), do_wait()
************************************************************/
