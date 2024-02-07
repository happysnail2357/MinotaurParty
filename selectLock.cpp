//
// selectLock.cpp
//
//   by Paul Puhnaty - 2/07/2024
//
//   A custom spin lock where a master thread decides the
//   order in which threads will acquire the lock.
//


#include "selectLock.hpp"


void selectLock::waitForTurn(int id)
{
	while (this->currentSelection != id && this->lockActive);
}
	
void selectLock::finishTurn()
{
	this->currentSelection = -1;
	
	this->needToSelect = true;
}


bool selectLock::selectorWait()
{
	while (this->needToSelect == false)
		if (this->lockActive == false)
			return false;
		
	return true;
}

void selectLock::selectorSelect(int id)
{
	this->needToSelect = false;
	
	this->currentSelection = id;
}


void selectLock::disable()
{
	this->lockActive = false;
}

