// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include "resmanager.h" 
#include "alarm.h"
#include <new>
#include <unistd.h>

Thread *t[20];      // The processes contending for resource units. 
char pname[20][5]; 
ResManager *RM; 
int used=0;         // Keep track of units in use so we can check 
                    // validity of the system. 


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");

    Thread *thread1 = new(std::nothrow) Thread("forked thread");

    thread1->Fork(SimpleThread, 1);
    SimpleThread(0);
}

void 
RMProcess(int arg) 
{ 
  int i, delay, hold, numunits; 

  // Let's roll some random numbers for a process 

  delay = 1+(int) (100000.0*rand()/(RAND_MAX+1.0));  // Initial delay before 
                                                     // requesting units. 
  hold = 1+(int) (100000.0*rand()/(RAND_MAX+1.0));   // Time the units are held. 
  numunits = 1+(int) ((float)NUMUNITS*rand()/(RAND_MAX+1.0));   // Number of units. 

  // An initial, pre-request delay. 
  for (i=0; i<delay; i++){ 
    interrupt->SetLevel(IntOff); 
    interrupt->SetLevel(IntOn); 
  } 

  printf("p%d asking for %d units\n",arg, numunits); 
  RM->Request(numunits); 
  printf("p%d GOT %d units\n",arg, numunits); 
  used += numunits; 

  ASSERT(used <= NUMUNITS);                          // Sanity check! 

  // Process holds the resource units for a while. 
  for (i=0; i<hold; i++){ 
    interrupt->SetLevel(IntOff); 
    interrupt->SetLevel(IntOn); 
  } 
  printf("p%d returning %d units\n",arg, numunits); 
  used -= numunits; 
  RM->Release(numunits); 
}  

void 
TestLocksAndConditions()
{ 
  int i; 

  DEBUG('t', "Starting the RESOURCE MANAGER System"); 
  // Instantiate the resource manager. 
  RM = new(std::nothrow) ResManager; 
  // Instantiate the 20 threads. 
  for (i=0; i<20; i++) { 
    sprintf(pname[i], "p%d", i); 
    t[i] = new(std::nothrow) Thread(pname[i]); 
    t[i]->Fork(RMProcess,i); 
  } 
  // Having spawned 20 threads, the initial thread terminates. 
} 




//////////////////////////////////////////////////////////////
//
// Prints one character of the test string and then sleeps 
// for the set amount of time
//
//////////////////////////////////////////////////////////////

void 
InvokeAlarm()
{
  Alarm* alarm = new(std::nothrow) Alarm ("ourAlarm");

  std::string content("Testing the Alarm Clock\n\n");

  unsigned int i = 0;
  interrupt->Enable();
  //while (i < content.size())
  while (1)
  {
    interrupt->Enable();
    std::cout << content[i];
   alarm->GoToSleepFor(3);
    i++;
    if (i == content.size()) i=0;
   interrupt->SetLevel(IntOff);
  }

}