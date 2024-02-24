//
// queueLock.cpp
//
//   by Paul Puhnaty - 2/23/2024
//
//   An array-based spin lock where each thread waits in a queue.
//


#include "queueLock.hpp"


queueLock::queueLock(int32_t size)
{
	this->size = size;
	
	flag.resize(size * 4);
	
	flag.at(0) = true;
}


int32_t queueLock::lock()
{
	// Get the next open slot.
	// fetch_add() is an atomic get and increment operation
	//
	int32_t key = this->next.fetch_add(1) % this->size;
	
	// Spin...
	//
	while (this->flag.at(key * 4) == false);
	
	// Entering critical section...
	
	this->accesses.fetch_add(1);
	
	return key;
}


void queueLock::unlock(int32_t key)
{
	// Mark this slot as not running
	//
	this->flag.at(key * 4) = false;
	
	// Mark the next slot to run
	//
	this->flag.at(((key + 1) % this->size) * 4) = true;
}


int32_t queueLock::numberOfAccesses()
{
	return this->accesses.load();
}
