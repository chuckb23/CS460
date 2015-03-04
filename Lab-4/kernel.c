

PROC *kfork(char *filename){
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

