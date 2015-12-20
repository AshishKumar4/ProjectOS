#include <window.cpp>
unsigned int kstackend;
#define NULL ( (void *) 0)
class Process
{
public:
    unsigned int pid;
  unsigned char state;
  unsigned int esp;    //actual position of esp
  unsigned int ss;     //actual stack segment.
  unsigned int kstack; //stacktop of kernel stack
  unsigned int ustack; //stacktop of user stack
  unsigned int cr3;
  unsigned int owner;
  unsigned int group;
  unsigned int timetorun;
  unsigned int sleep;
  unsigned int priority;
  Process* next;
};

typedef struct q_tt {
  Process *proc;
  q_tt *next;
} q_t;


static q_t *ready_f = 0;
static q_t *ready_l = 0;

Process *startproc,*lastproc,*nullProc,*task_curr,*task_new;

typedef void (*entry_t)(void);
Process *task_create(entry_t entry,int priority){
  //various initialization stuff may be done before reaching the below described operations.
  //filling in the kstack for start up:
  unsigned int  *stacksetup; //temporary pointer - will be set to the kstacktop
                             //and afterwards saved in esp of proc structure
  stacksetup=((unsigned int*)kmalloc(16))+16;  // ugh.. hope this works..
  *stacksetup--;
  *stacksetup--=0x0202;
  *stacksetup--=0x08;
  *stacksetup--=(unsigned int)entry; //This is the adress of the process' entry point (i.e. main());
  *stacksetup--=0;    //ebp
  *stacksetup--=0;    //esp
  *stacksetup--=0;    //edi
  *stacksetup--=0;    //esi
  *stacksetup--=0;    //edx
  *stacksetup--=0;    //ecx
  *stacksetup--=0;    //ebx
  *stacksetup--=0;    //eax
  *stacksetup--=0x10; //ds
  *stacksetup--=0x10; //es
  *stacksetup--=0x10; //fs
  *stacksetup--=0x10; //gs

  // creating and allocating the struct
  Process *newproc;
  if ((newproc = (Process*)kmalloc(sizeof(Process))) == NULL)
    return nullProc;

  newproc->esp = (unsigned int) stacksetup;
  newproc->ss = 0x10;
  //newproc->kstack = (unsigned int)&kstacks[d][KSTACKTOP];
  //newproc->ustack = (unsigned int)&stacks[d][USTACKTOP];
  newproc->next = (Process*)NULL;

  lastproc->next = newproc; // put the process in the last addr ptr
  lastproc = newproc;

  return newproc;
}

void q_ready_add(Process *p)
{
  if (ready_l)
  {
    ready_l->proc = p;
    ready_l->next =(q_tt*)kmalloc(sizeof(q_t));
    ready_l = ready_l->next;
  } else {
    ready_f = (q_tt*)kmalloc(sizeof(q_t));
    ready_f->next = (q_tt*)kmalloc(sizeof(q_t));
    ready_f->proc = p;
    ready_l = ready_f->next;
  }
}

q_t *q_ready_getf()
{
  q_t *ret;
  ret = ready_f;
  ready_f = ready_f->next;
  return ret;
}
void TaskSwitcher()
{
  // does the actual context switch
  __asm__ ("cli"); // disable interrupts

  // 1. save the things to the task_curr

  __asm__ ("movl %%esp, %0;"
	   :"=r"(task_curr->esp)        /* output */
	   );

  __asm__ ("movl %%ss, %0;"
	   :"=r"(task_curr->ss)        /* output */
	   );

  task_curr = task_new; // change the task_curr pointer

  // 2. restore things from task_new

  __asm__ ("movl %0, %%esp;"
	   :
	   :"r" (task_curr->esp)
	   );

  __asm__ ("movl %0, %%ss;"
	   :
	   :"r" (task_curr->ss)
	   );

  __asm__ ("sti"); // enable interrupts again
}

void schedule() {

  if ( task_curr->timetorun<1 ) {
    q_ready_add(task_curr);
    task_new = q_ready_getf()->proc;
    task_new->timetorun = 1000;
    TaskSwitcher();
  }
}

//the isr
void timer_handler2 (regs* r)
{
  if(task_curr->timetorun>0)
    {
        task_curr->timetorun--;
    }
  else
    {
    task_curr->esp = r->esp;
    task_curr->ss = r->ss;
    schedule();
  }
}

void idle()
{
    int i=0;
    while(i++<10000)
    {
        printf("Test ");
    }
}

void initMultiTasking()
{
    nullProc = task_create(idle,1);
    q_ready_add(nullProc);
    __asm__ ("movl %%esp,%0;"
	   :"=r" ((unsigned int) kstackend)
	   );
    irq_register_install(0,&timer_handler2);
}

