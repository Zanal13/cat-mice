

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

extern int initialize_bowls(unsigned int bowlcount);

extern void cat_eat(unsigned int bowlnumber);
extern void mouse_eat(unsigned int bowlnumber);
extern void cat_sleep(void);
extern void mouse_sleep(void);

volatile int numMiceEating;
volatile int numCatEating;
volatile int numMiceWaiting, numCatWaiting;
volatile int *bowlInUse;
volatile int turn;

struct semaphore *CatMouseWait;
struct cv * bowlEmpty;
struct lock * mutex;

int NumBowls;  
int NumCats;   
int NumMice;   
int NumLoops;  



static
void
cat_simulation(void * unusedpointer, 
	       unsigned long catnumber)
{
  int i, j;
  unsigned int bowl;

  /* avoid unused variable warnings. */
  (void) unusedpointer;
  (void) catnumber;

  numCatWaiting += 1;


  for(i=0;i<NumLoops;i++) {
    cat_sleep();

	lock_acquire(mutex);
	while( (turn != 1 && turn != 2 && (numMiceEating > 0 || numMiceWaiting > 0 ||  numCatEating == NumBowls)) ||
			( (turn == 1 || turn == 2) && numCatEating == NumBowls ) )	{

		cv_wait(bowlEmpty, mutex);
	}
	turn = 1;
	for(j = 0; j<NumBowls; j+=1 ) {
		if (bowlInUse[j] == 0) {
			bowlInUse[j] = 1;
			bowl = j+1;
			numCatEating += 1;
			break;
		}
	}
	lock_release(mutex);

    cat_eat(bowl);

	lock_acquire(mutex);
	numCatEating -= 1;
	bowlInUse[bowl-1] = 0;
	if (numCatEating == 0) {
		turn = 0;
		cv_broadcast(bowlEmpty, mutex);
	}
	lock_release(mutex);
  }

  numCatWaiting -= 1;


  V(CatMouseWait); 
}
	
struct cv * bowlEmpty;
struct lock * mutex;


static
void
mouse_simulation(void * unusedpointer,
          unsigned long mousenumber)
{
  int i, j;
  unsigned int bowl;

 
  (void) unusedpointer;
  (void) mousenumber;

  numMiceWaiting += 1;

  for(i=0;i<NumLoops;i++) {

    mouse_sleep();

	lock_acquire(mutex);
	while( (turn != 0 && turn != 2 && (numCatEating > 0 || numCatWaiting > 0 ||  numMiceEating == NumBowls)) ||
			( (turn == 0 || turn == 2) && numMiceEating == NumBowls ) )	{
		cv_wait(bowlEmpty, mutex);
	}
	turn = 0;
	for(j = 0; j<NumBowls; j+=1 ) {
		if (bowlInUse[j] == 0) {
			bowlInUse[j] = 1;
			bowl = j+1;
			numMiceEating += 1;
			break;
		}
	}
	lock_release(mutex);
    
    mouse_eat(bowl);

	lock_acquire(mutex);
	numMiceEating -= 1;
	bowlInUse[bowl-1] = 0;
	if (numMiceEating == 0) {
		turn = 1;
		cv_broadcast(bowlEmpty, mutex);
	}
	lock_release(mutex);
  }

  numMiceWaiting -= 1;
  /* indicate that this mouse is finished */
  V(CatMouseWait); 
}


int
catmouse(int nargs,
	 char ** args)
{
  int index, error;
  int i;
  numMiceEating = 0;
  numCatEating = 0;
  numMiceWaiting = 0;
  numCatWaiting = 0;
  turn = 2;

  /* check and process command line arguments */
  if (nargs != 5) {
    kprintf("Usage: <command> NUM_BOWLS NUM_CATS NUM_MICE NUM_LOOPS\n");
    return 1;  // return failure indication
  }

  /* check the problem parameters, and set the global variables */
  NumBowls = atoi(args[1]);
  if (NumBowls <= 0) {
    kprintf("catmouse: invalid number of bowls: %d\n",NumBowls);
    return 1;
  }
  NumCats = atoi(args[2]);
  if (NumCats < 0) {
    kprintf("catmouse: invalid number of cats: %d\n",NumCats);
    return 1;
  }
  NumMice = atoi(args[3]);
  if (NumMice < 0) {
    kprintf("catmouse: invalid number of mice: %d\n",NumMice);
    return 1;
  }
  NumLoops = atoi(args[4]);
  if (NumLoops <= 0) {
    kprintf("catmouse: invalid number of loops: %d\n",NumLoops);
    return 1;
  }
  kprintf("Using %d bowls, %d cats, and %d mice. Looping %d times.\n",
          NumBowls,NumCats,NumMice,NumLoops);

  /* create the semaphore that is used to make the main thread
     wait for all of the cats and mice to finish */
  CatMouseWait = sem_create("CatMouseWait",0);
  if (CatMouseWait == NULL) {
    panic("catmouse: could not create semaphore\n");
  }

   bowlEmpty = cv_create("bowlEmpty");
   mutex = lock_create("mutex");
  

  if (initialize_bowls(NumBowls)) {
    panic("catmouse: error initializing bowls.\n");
  }
  bowlInUse = (int*)kmalloc(sizeof(int)*NumBowls);
  for(i = 0; i<NumBowls; i+=1 ) {
  	bowlInUse[i] = 0;
  }

  numMiceEating = 0;


  for (index = 0; index < NumCats; index++) {
    error = thread_fork("cat_simulation thread",NULL,index,cat_simulation,NULL);
    if (error) {
      panic("cat_simulation: thread_fork failed: %s\n", strerror(error));
    }
  }


  for (index = 0; index < NumMice; index++) {
    error = thread_fork("mouse_simulation thread",NULL,index,mouse_simulation,NULL);
    if (error) {
      panic("mouse_simulation: thread_fork failed: %s\n",strerror(error));
    }
  }

  /* wait for all of the cats and mice to finish before
     terminating */  
  for(i=0;i<(NumCats+NumMice);i++) {
    P(CatMouseWait);
  }

  sem_destroy(CatMouseWait);

  return 0;
}

