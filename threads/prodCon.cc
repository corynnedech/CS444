#ifdef CHANGED
// prodCon.cc 
//
//     

#include "prodCon.h"
#include "thread.h"
#include <string>
#include <iostream>
#include <new>
#include <list>

Lock* lock = new(std::nothrow) Lock("buffer lock"); 

std::list<char> buffer;
unsigned int BUFFER_SIZE = 3;
Condition *condFull = new(std::nothrow) Condition("condition full prodcon"); 
Condition *condEmpty = new(std::nothrow) Condition("condition empty prodcon");
std::string content("Hello World");
unsigned int currentIndex = 0;
unsigned int finished = 0;

void consumerThread(int which)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~~Consumer Thread~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    std::cout << "in consumerThread\n";
  
    while(1)
    {
      lock->Acquire();
      if (finished && buffer.size() == 0)
      {
          break;
      }
      if (buffer.size() == 0)
      {
          condEmpty->Wait(lock);
      }
      std::cout << buffer.front();
      buffer.pop_front();  
      if (buffer.size() == BUFFER_SIZE -1)
      { 
          condFull->Signal(lock);
      }
      lock->Release();      
    }  
    std::cout<<"OUT\n";  
}

void producerThread (int which)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~~Producer Thread~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    std::cout << "in prod\n";

    while (currentIndex < content.size())
    {
       lock->Acquire();
       if (buffer.size() == BUFFER_SIZE)
       {
            condFull->Wait(lock);
       }
       buffer.push_back(content[currentIndex]);
      if (buffer.size() == 1)
       {
            condEmpty->Signal(lock);
       }     
       currentIndex ++;
       lock->Release();       
    } 
    condEmpty->Broadcast(lock);
    finished = 1;
}

void ProdCon()
{
    DEBUG('p', "Entering prodCon\n");

    Thread *conThread = new(std::nothrow) Thread ("Consumer1");
    Thread *conThread2 = new(std::nothrow) Thread ("Consumer2");
    Thread *prodThread = new(std::nothrow) Thread ("Producer"); 
    Thread *prodThread2 = new(std::nothrow) Thread ("Producer2"); 

    Thread *conThread3 = new(std::nothrow) Thread ("Consumer1");
    Thread *conThread4 = new(std::nothrow) Thread ("Consumer2");
    Thread *prodThread3 = new(std::nothrow) Thread ("Producer"); 
    Thread *prodThread4 = new(std::nothrow) Thread ("Producer2"); 

    prodThread2->Fork(producerThread, 1);
    conThread->Fork(consumerThread, 1);
    conThread2->Fork(consumerThread, 1);
    prodThread->Fork(producerThread, 1);

     prodThread3->Fork(producerThread, 1);
    conThread3->Fork(consumerThread, 1);
    conThread4->Fork(consumerThread, 1);
    prodThread4->Fork(producerThread, 1);
}

#endif
