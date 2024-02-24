//
// selectLock.cpp
//
//   by Paul Puhnaty - 2/07/2024
//
//   A custom spin lock where a master thread decides the
//   order in which threads will acquire the lock.
//


#include "selectLock.hpp"
#include <thread>


void selectLock::lock(int32_t id)
{
	while (this->currentSelection != id && this->lockActive)
	{
		// There's no point checking over and over again here
		// since its not likely we'll be chosen soon, so
		// just yield to another process
		
		std::this_thread::yield();
	}
}
	
void selectLock::unlock()
{
	this->currentSelection = -1;
	
	this->needToSelect = true;
}


bool selectLock::wait()
{
	while (this->needToSelect == false)
		if (this->lockActive == false)
			return false;
		
	return true;
}

void selectLock::select(int32_t id)
{
	this->needToSelect = false;
	
	this->selections++;
	
	this->currentSelection = id;
}


void selectLock::disable()
{
	this->lockActive = false;
}


int32_t selectLock::numSelections()
{
	return this->selections;
}
