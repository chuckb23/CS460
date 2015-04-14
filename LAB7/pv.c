struct semaphore{
  int value;
  PROC *queue;
};

int P(struct semaphore *s)
{

  int sr=int_off();
  s->value--;
  printf("Process blocked\n");
  if(s->value<0)
  	wait(s->queue);
  int_on(sr);

}

int V(struct semaphore *s)
{
	int sr = int_off();
  s->value++;
  printf("Process dequeued");
  if(s->value<=0)
  	signal(s->queue);
  int_on(sr);
}
