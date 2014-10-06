#ifdef CHANGED

#ifndef ELEVATOR_H
#define ELEVATOR_H


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



class Elevator {
  public:
    Elevator(const char* debugName);	// set initial value
    ~Elevator();   					// de-allocate 
    const char* getName() { return name;}			// debugging assist
        
    void ArrivingGoingFromTo(int atFloor, int toFloor);
    void RunElevator();
	void insert_ordered(struct SCB *insert);
	void behead_list()

  private:
    const char* name;        // useful for debugging
    Semaphore count;
    std::list onElevator;
    personWaiting *waitingForElevator;
    int currentFloor;
};


#endif
#endif