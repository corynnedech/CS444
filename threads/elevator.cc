#ifdef CHANGED

#include "elevator.h"

struct personWaiting {
	Semaphore *mutex;
	int location;
	struct personWaiting *next;
} typedef personWaiting;

Elevator::Elevator(const char* debugName)
{
	name = debugName;
	count  = new(std::nothrow) Semaphore("empty condition", 0);
    onElevator = new(std::nothrow) std::list;
    waitingForElevator =  NULL;
    currentFloor = 0;//basement
}

void Elevator::ArrivingGoingFromTo(int atFloor, int toFloor)
{
	count->V();
	personWaiting person;
	person->mutex = new(std::nothrow) Semaphore("wait condition", 0);
	person->location = atFloor;

	waitingForElevator->insert_ordered(person);
	person->mutex->P();
	onElevator->push_back(toFloor);
}

void Elevator::RunElevator()
{
	while (1)
	{
   		count->P();
   		if (onElevator==NULL)
   		{
   			//wait for (waitingForElevator->location - currentLocation)*100
   			currentLocation = waitingForElevator->location;
   			waitingForElevator->mutex->V();
   			waitingForElevator->behead_list();
   		}


   		onElevator->sort();
   		

	}
}

//----------------------------------------------------------------------
// Alarm::insert_ordered
//      Inserts a new SCB object onto the linked list of SCB's that are
// sleeping in order by waketime.
//----------------------------------------------------------------------
void Elevator::insert_ordered(personWaiting *insert){
	personWaiting *temp;
	personWaiting *prev;
	if (list == NULL){
		std::cout<<"INSERTYING TO HEAD\n";
		list = insert;
		return;
	}
	temp=list;
	while((temp != NULL) && (temp->location < insert->location)){
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
void Elevator::behead_list(){
	if (list->next != NULL){
		list = list->next;
	}
	else{
		list = NULL;
	}
}



#endif