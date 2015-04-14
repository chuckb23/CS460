// YOUR wait.c file containing
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
