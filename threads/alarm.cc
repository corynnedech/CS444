#ifdef CHANGED
// alarm.cc 
//	Implements the alarm clock class. Each alarm clock objects initializes a new hardware Timer device, using a new
//	timer handler, "AlarmTimerHandler".  When the timer interrupt goes off, the AlarmTimerHandler 
//	checks to see if any thread that had been asleep needs to wake up now. Threads call 
//	Alarm::GoToSleepFor(int howLong)" to go to sleep for a period of time, where (howLong * 100) is the number 
//	of ticks that the thread should sleep for.
//     
#include "alarm.h"

// Sleep Control Block structure to save a linked list of all sleeping threads and the respective waketimes.
struct SCB {
	Condition *go;		// Condition that the thread waits on
	int waketime;		// systime value when thread should wake up
	struct SCB *next;
};

//----------------------------------------------------------------------
// AlarmTimerHandler
//      Handler that is called for each tick of the alarmTimer. Checks
//	the head of the waiting threads list for threads whose waketimes
//	are equal to the current time, and wakes up those threads.
//	"arg" is a int pointer to the current Alarm object
//----------------------------------------------------------------------
static void AlarmTimerHandler(int arg){
	std::cout<<"In alarm timer handler\n";
	    
	if (interrupt->getStatus() != IdleMode)
		interrupt->YieldOnReturn();

	Alarm *alarm = (Alarm *)arg;	// Casts the arg pointer to an Alarm object.
	//Timer *p = alarm->alarmTimer;

	alarm->systime++;				// Increments the systime
	printf("\nSystime: %i\n\n", alarm->systime);

	if (alarm->list != NULL){
		printf("\nSystime: %i   Waketime: %i\n\n", alarm->systime, alarm->list->waketime);
		if (alarm->list->waketime == alarm->systime){		// Checks if head is ready to be awoken.
			printf("\nWaking up thread.\n");
			alarm->list->go->Signal(alarmLock);				// Wakes up thread.
		}
	}
	//interrupt->Schedule(AlarmTimerHandler, (int) alarm, p->TimeOfNextInterrupt(), TimerInt);
	//p->TimerExpired();
}

//----------------------------------------------------------------------
// Alarm::Alarm
//      Initializes a new Alarm object. Creates a new Timer object and 
//	Lock for use in the Alarm. Sets the waiting thread list to null.
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------
Alarm::Alarm (const char* debugName)
{
	name = debugName;
	systime = 0;
	alarmTimer = new(std::nothrow) Timer(AlarmTimerHandler, (int) this, false);
	Lock *alarmLock = new(std::nothrow) Lock("alarm clock alarmLock"); 
	list = NULL;	//head of list of "sleep control blocks"
}

//----------------------------------------------------------------------
// Alarm::~Alarm
// 	De-allocate Alarm, when no longer needed.  Assume no one
//	is still on the waiting list.
//----------------------------------------------------------------------
Alarm::~Alarm ()
{
	delete list;
}

//----------------------------------------------------------------------
// Alarm::GoToSleepFor
// 		Thread goes to sleep for a specified amount of time. Is put on 
//	waiting list in order of shortest waketime and waits for a Signal
//	to wake up and be put back on the ready list.
//	"howLong" is number of ticks mod 100 that thread should sleep
//----------------------------------------------------------------------
void Alarm::GoToSleepFor(int howLong)
{	
	
	alarmLock->Acquire();
	printf("\nThread is sleeping for %i ticks.\n", (howLong * 100));
	struct SCB *mySCB;

	mySCB = new SCB;
	mySCB->go = new(std::nothrow) Condition("wake up from alarm");	// Condition to wait on.
	mySCB->waketime = systime+howLong;		// Set waketime to current time + desired wait time
	insert_ordered(mySCB);		//Insert mySCB onto linked list in incrementing order of waketime.

	printf("\nInserted new alarm for %i \n", list->waketime);

//interrupt->Enable();
	mySCB->go->Wait(alarmLock);		// Wait until waketime;
	printf("awoken from Wait()");
	behead_list();			// When awoken, removes from linked list
	delete mySCB;			// De-allocate current sleep control block
	std::cout<<list;

	//Checks if there are more threads waiting with an identical waketime
	if (list != NULL){		
		if (list->waketime == systime){
			list->go->Signal(alarmLock);
		}
	}
	alarmLock->Release();

}

//----------------------------------------------------------------------
// Alarm::insert_ordered
//      Inserts a new SCB object onto the linked list of SCB's that are
// sleeping in order by waketime.
//----------------------------------------------------------------------
void Alarm::insert_ordered(struct SCB *insert){
	struct SCB *temp;
	struct SCB *prev;

	if (list == NULL){		
		// Sets new SCB to be the head of the list.
		list = insert;
		return;
	}
	temp=list;
	// Finds correct position in ordered linked list of 
	while((temp != NULL) && (temp->waketime < insert->waketime)){
		prev = temp;
		temp = temp->next;
	}
	if (temp == list){
		insert->next = list;
		list = insert;
	}
	else{
		prev->next = insert;
		insert->next = temp;
	}
}

//----------------------------------------------------------------------
// Alarm::behead_list
//      Removes the first SCB from the linked list of SCB's. Is called
// when the next SCB is woken and thus must be at the head of the list.
//----------------------------------------------------------------------
void Alarm::behead_list(){
	if (list->next != NULL){
		list = list->next;
	}
	else{
		list = NULL;
	}
}

#endif

