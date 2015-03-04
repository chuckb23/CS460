/************ t.c file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

#define DEAD      0                /* proc status     */
#define READY     1   
#define FREE 			2   

#define NULL 0

typedef struct proc{
    struct proc *next;   
           int  ksp;               /* saved sp; offset = 2 */
           int  pid;
					 int  ppid;
           int  status;            /* READY|DEAD, etc */
					 int  priority;
		struct proc *parent;
           int  kstack[SSIZE];     // kmode stack of task
}PROC;


// #include "io.c" /**** USE YOUR OWN io.c with YOUR printf() here *****/

PROC proc[NPROC], *running, *freeList, *readyQueue;

int  procSize = sizeof(PROC);

/****************************************************************
 Initialize the proc's as shown:

 running->proc[0]--> proc[1] --> proc[2] ... --> proc[NPROC-1] -->
                       ^                                         |
            |<---------------------------------------<------------

 Each proc's kstack contains:
      retPC, ax, bx, cx, dx, bp, si, di, flag;  all 2 bytes
*****************************************************************/

int body();  

int initialize()
{
  int i, j;
  PROC *p;

  for (i=0; i < NPROC; i++){
    p = &proc[i];
    p->next = &proc[i+1];
    p->pid = i;
		p->ppid = NULL;
    p->status = FREE;
		p->priority = 0;
		p->parent = 0;
    
   if (i){     // initialize kstack[ ] of proc[1] to proc[N-1]
      for (j=1; j < 10; j++){
          p->kstack[SSIZE - j] = 0;
						}				          // all saved registers = 0
      		p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
      		p->ksp = &(p->kstack[SSIZE-9]);        // ksp -> kstack top
			
    }
  }
  running = &proc[0];
	running->status = READY;
	running->priority = 0;
	running->next = NULL;
	
	freeList = &proc[1];
	readyQueue = NULL;
  
	proc[NPROC-1].next = &proc[0];
  printf("initialization complete\n"); 
}

void printQueue(PROC *queue){
	PROC *p = queue;
	while(p!=NULL){
		printf(" [ %d ] -> ",p->pid);
		p = p->next;
	}
	return;
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
	

	for (j=1; j < 10; j++){
    p->kstack[SSIZE - j] = 0;
			}
		p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
  	p->ksp = &(p->kstack[SSIZE-9]);
	
	printf("Process %d forked\n",p->pid);
	enqueue(&readyQueue,p);
	return p; 
}

int body()
{
   char c;
   printf("proc % resumes to body() function\n");
   while(1){
      printf("I am Proc %d in body(): Enter a key [s, q, f]:  ", running->pid);
      c=getc();
      printf("%c\n", c);
			switch(c){
				case 's':tswitch(); break;
				case 'f':kfork(); break;
				default: break;
			}
   }

}

main()
{
  	char c;
   printf("\nWelcome to the 460 Multitasking System\n");
   initialize();
   kfork();   // create a child process in readyQueue
	 printQueue(&readyQueue);
       while(1){  // loop forever
					printf("Forever and ever and ever and ever\n");
         if (readyQueue){ 
						printf("Ready");
            tswitch(); // switch process
						}
       }
	/*printf("Welcom to the 460 MultiTasking System\n");
	initialize();
	printf("P0 forks to P1\n");
	kfork();
	
	tswitch();
	printf("Switch back \n");*/
}

int scheduler()
{
    if (running->status == READY)
          enqueue(&readyQueue, running);

    running = dequeue(&readyQueue);
		printf("Running PID: %d \n", running->pid);
}

