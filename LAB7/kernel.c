PROC * get_proc(PROC **list){
return (dequeue(list));
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
  //  printf("Parent ID: %d\n",proc[i].parent->pid);
  }
}
int body();

PROC *kfork(char *filename)
{
  PROC *p;
  int  i, child;
  u16  segment,segsize;
  printf("Beginning a kfork\n");
  /*** get a PROC for child process: ***/
  if ( (p = get_proc(&freeList)) == 0){
       printf("no more proc\n");
       return(-1);
  }
  /* initialize the new proc and its stack */
  p->status = READY;
  p->ppid = running->pid;
  p->parent = running;
  p->priority  = 1;                 // all of the same priority 1

  // clear all SAVed registers on kstack
  for (i=1; i<10; i++)
      p->kstack[SSIZE-i] = 0;
 
  // fill in resume address
  p->kstack[SSIZE-1] = (int)body;
  // save stack TOP address in PROC
  p->ksp = &(p->kstack[SSIZE - 9]);

  enqueue(&readyQueue,p);
	//ss = (unsigned short) 0x1000;
  nproc++;
  if (filename){

     segment = 0x1000*(p->pid+1);  // new PROC's segment
     printf("File %d loaded starts in %x\n",p->pid,segment);
     load(filename, segment);      // load file to LOW END of segment
			segsize = 0x1000;
      printf("File loaded completing\n");
     /********** ustack contents at HIGH END of ustack[ ] ************
        PROC.usp
       -----|------------------------------------------------
          |uDS|uES|udi|usi|ubp|udx|ucx|ubx|uax|uPC|uCS|flag|
       -----------------------------------------------------
           -12 -11 -10 -9  -8  -7  -6  -5  -4  -3  -2   -1
     *****************************************************************/

     for (i=1; i<=10; i++){         // write 0's to ALL of them

         put_word(0, segment, (segsize-2*i));
     }
     
     put_word(0x0200,   segment, (segsize-2*1));   /* flag */  
     put_word(segment,  segment, (segsize-2*2));   /* uCS */  
     put_word(segment,  segment, (segsize-2*11));  /* uES */  
     put_word(segment,  segment, (segsize-2*12));  /* uDS */  
     // YOU WRITE CODE TO FILL IN uDS, uES, uCS

     /* initial USP relative to USS */

     p->usp = (segsize-2*12); 
     p->uss = segment;
 
     

  }

  printf("Proc %d kforked a child %d at segment=%x\n",
          running->pid, p->pid, segment);
  return p;
}
int goUmode();

int fork(){
   PROC *p;
  int  i, child;
  u16  segment,segsize;
  p = kfork(0);
 if(p){
  segment = 0x1000*(p->pid+1);
  segsize = 0x1000;
  p->uss = segment;
  copy_image(segment);
  p->kstack[SSIZE-1] = (int)goUmode;
 
     for (i=1; i<=10; i++){         // write 0's to ALL of them

         put_word(0, segment, (segsize-2*i));
     }
     
     put_word(0x0200,   segment, (segsize-2*1));   /* flag */  
     put_word(segment,  segment, (segsize-2*2));   /* uCS */  
     put_word(segment,  segment, (segsize-2*11));  /* uES */  
     put_word(segment,  segment, (segsize-2*12));  /* uDS */  
     // YOU WRITE CODE TO FILL IN uDS, uES, uCS

     /* initial USP relative to USS */

     p->usp = segsize-2*12; 
     printf("Proc %d kforked a child %d at segment=%x\n",
          running->pid, p->pid, segment);
     return p->pid;
   }
  return -1;
     

  }
int kexec(char* filename){
  u16 segment, segsize;
  int i;
  char str[64];
 // char c;
  //segment = 0x2000;
  i =0;
  //get exec file
  do{
    //i++;
    str[i]=get_byte(running->uss,filename+i);
    //str[i]=c;
    i++;
    printf("Name: %c\n",str[i]);
    printf("%d\n",i);
  }while(str[i-1]!='\0' && i<64);
  //printf("Name1: %s\n",&filename);
  //Check if no exec file entered
  if(str[0]==0){
    printf("Exec file is null\n");
    return -1;
  }
  //printf("Name2: %s\n",str);
  //Check if load fails
  if(load(str,running->uss)==0){
    printf("Load failed on file %s\n",str);
    return -1;
  }
 // printf("Name3: %s\n",str[0]);
  segment = running->uss;
  segsize = 0x1000;
 // p->uss = segment;
  //p->kstack[SSIZE-1] = (int)goUmode;
 
     for (i=1; i<=10; i++){         // write 0's to ALL of them

         put_word(0, segment, (segsize-2*i));
     }
     
     put_word(0x0200,   segment, (segsize-2*1));   /* flag */  
     put_word(segment,  segment, (segsize-2*2));   /* uCS */  
     put_word(segment,  segment, (segsize-2*11));  /* uES */  
     put_word(segment,  segment, (segsize-2*12));  /* uDS */  
     // YOU WRITE CODE TO FILL IN uDS, uES, uCS

     /* initial USP relative to USS */
     printf("execed %d at segment=%x\n",
          running->pid, segment);
     running->usp = segsize - 2*12 ;
     return 1;
   
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