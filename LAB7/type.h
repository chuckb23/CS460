typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NULL      0
#define NPROC     9
#define SSIZE   512

#define FREE      0    /* PROC status */
#define READY     1 
#define SLEEP     2
#define ZOMBIE    3
#define BLOCK     4

typedef struct proc{
        struct proc *next;
        int    *ksp; 
        int    uss,usp;
        int    inkmode;

        int     pid;
        int     ppid;
  struct proc *parent;
        int     status;
        int     priority;          
        char    name[16];    
        int     event;
        int     exitCode;
        int     size;        
  int kstack[SSIZE];
}PROC;        
