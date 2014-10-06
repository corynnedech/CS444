#ifdef CHANGED

#ifndef ALARM_H
#define ALARM_H


#include "copyright.h"
#include "thread.h"
#include "system.h"
#include "list.h"
#include "timer.h"
#include "synch.h"
#include <new>

#include <string>
#include <iostream>
#include <list>


class Alarm {
  public:
    Alarm(const char* debugName);	// set initial value
    ~Alarm();   					// de-allocate semaphore
    const char* getName() { return name;}			// debugging assist
    
    void GoToSleepFor(int howLong);

    
    
    void insert_ordered(struct SCB *insert);
    void behead_list();


 // private:
    const char* name;        // useful for debugging
    int systime;			 // current time
    struct SCB *list;       // threads waiting in P() for the value to be > 0
    Timer *alarmTimer;
    VoidFunctionPtr handler;	
};

#endif
#endif