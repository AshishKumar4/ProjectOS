
extern "C"
{
    int printf(const char* __restrict, ...);
    void printint(int in);
    void* kmalloc(unsigned int size);
    void kfree(void* block);
    unsigned int ReadFromCMOS ();
}
int *rn;
static unsigned long int next = 0;
void kfree(void* block);
unsigned int time;

int rand() // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}


void srand( unsigned int seed )
{
    next = (next+seed)%100000;
}
void srandInit()
{
    time=ReadFromCMOS();
    srand(time);
}

int seeder()
{
    //rn=(int*)kmalloc(sizeof(int));
    int r=rand();
   // r+=*rn;
   // kfree((void*)rn);

    return r;
}
int trueRandSeeder()
{
    rn=(int*)kmalloc(sizeof(int));
    int r=rand();
    r+=*rn;
    kfree((void*)rn);

    return r;
}

