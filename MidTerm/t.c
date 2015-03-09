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
void do_wakeup();
void do_wait();
void do_exit();
void do_sleep();
void printQueue(PROC *queue){
	PROC *p = queue;
	while(p!=NULL){
		printf(" %d [%d] -> ",p->pid,p->priority);
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
       int i,j;

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
				//p->kstack[SSIZE-1]=(int)body;
       running = p;
       nproc++;
				
       printf("done\n");
				//tswitch();
} 
void enqueue(PROC **queue,PROC *p){
	PROC *head = *queue, *tmp = NULL;
	//printQueue(head);
	//printf("Woke Up Process 0000.pid\n");
	if(*queue == NULL || p->priority > (*queue)->priority){
		*queue = p;
		p->next = head;
		printf("NULL \n");
		return;
		}
	tmp = (*queue)->next;
	while(tmp!=NULL && p->priority <= tmp->priority){
		head = tmp;
		tmp = head->next;
	}
	//printf("Woke Up Process \n");
	head->next = p;
	p->next = tmp;
	return;
	
}
PROC *kfork(){
	PROC *p = get_proc();
	int j;
	if(p == NULL){
		printf("Free List is Empty Fails\n");
		return NULL;
		}
	
	p->status = READY;
	p->priority = 1;
	p->ppid = running->pid;
	p->parent = running;
	

	for (j=1; j <7; j++){
    p->kstack[SSIZE - j] = 0;
			}
		p->kstack[SSIZE-1]=(int)body; 
		p->kstack[SSIZE-6] = (unsigned short*) (0x1000);
		p->kstack[SSIZE-5] = (unsigned short*) (0x1000);
		p->kstack[SSIZE-4] = (unsigned short*) (0x1000);
		//p->ksp[SSIZE-3] = (int)body;
		//p->ksp[SSIZE-2] = 0x0200;	         // called tswitch() from body
  	p->ksp = &(p->kstack[SSIZE-6]);
		
	printf("Process %d forked\n",p->pid);
	enqueue(&readyQueue,p);
	return p; 
}

int printStack()
{
  int i; int *sp, *bp, *temp;

  if (running->pid == 2)
    return;

  printf("proc %d prints stack\n", running->pid);
  printf("ksp=%x  stackBottom=%x\n", running->ksp, &running->kstack[SSIZE-1]);
	//temp = running->ksp;
  sp = running->ksp;
  bp = getbp();

  printf("stack frame link list\n");
  while(bp){
    printf("%x->", bp);
    bp = *bp;
  }
  printf("0\n");
	getc();
  printf("stack contents from ksp to BOTTOM of stack\n");
  while (sp < &running->kstack[SSIZE]){
    printf("%x ", *sp);
    sp++; 
  }
	//running->ksp = temp;
  printf("\n");
  printf("Hello there stopping now:");
	getc();
	return 1;
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
		//running->ksp[SSIZE-1] = (int)body;
    kfork();     // P0 kfork() P1
		
		//printStack();
    while(1){
      printf("P%d running\n", running->pid);
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
	PROC *p;
	p = &proc[0];
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
		//do_ps();
    printf("-----------------------------------------\n");
    printf("Free Processes = ");
		printQueue(freeList);
		printf("NULL\n");
		printf("\nReady Processes = ");
		printQueue(readyQueue);
		printf("0 [0] -> NULL\n");
	
		printf("\nSleeping Processes = ");
		printQueue(sleepList);
		printf("NULL\n");
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

void do_exit(){
	char c;
	int exitval;
	printf("Enter an exit value: ");
	c = getc();
	exitval = c - '0';
	kexit(exitval);
}

void do_sleep(){
	char c;
	int sleep;
	printf("Enter an sleep value: ");
	c = getc();
	sleep = c - '0';
	ksleep(sleep);
}
void do_wakeup(){
	char c;
	int wake;
	printf("Enter an wakeup value: ");
	c = getc();
	wake = c - '0';
	kwakeup(wake);
}
void do_wait(){
	char c;
	int pid,status;
	pid = kwait(&status);
	printf("PID: %d  ExitValue: %d \n",pid,status);
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
			proc[i].ppid = running->parent->pid;
			//printf(" %d \n",proc[i].pid);
			}
		}
	kwakeup(running->parent->event);
	printf("Woke up parent pid = %d \n",running->parent->pid);
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
					*status = proc[i].exitCode;
					proc[i].status = FREE;
					proc[i].ppid = 0;
					enqueue(&freeList,&proc[i]);
					printf("Found Zombie Child ");
					return proc[i].pid;
					}
				}
			ksleep(running);
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
			//printf("Woke Up Process %d\n",proc[i].pid);
			proc[i].status = READY;
			//printf("Woke Up Process1 %d\n",proc[i].pid);
			enqueue(&readyQueue, &proc[i]);
			//printf("Woke Up Process2 %d\n",proc[i].pid);
			
			}
		}
	return 1;
}
int do_ps(){
	int i;
	printf("-------------------------------------\n");
	printf("-------------------------------------\n");
	printf("- name -    status    pid        ppid\n");
	printf("-------------------------------------\n");
	for(i = 0;i<NPROC;i++){
		if(i==running->pid){
			printf("       running     %d          %d\n",running->pid, running->ppid);
}
		else{
			printf("          %d       %d          %d\n",proc[i].status, proc[i].pid, proc[i].ppid);
}
	//	printf("Parent ID: %d\n",proc[i].parent->pid);
	}
	
}


/***********************************************************
  Write YOUR C code for
        ksleep(), kwakeup()
        kexit()
        kwait()

Then, write your C code for
      do_ps(), do_sleep(), do_wakeup(), do_wait()
************************************************************/
