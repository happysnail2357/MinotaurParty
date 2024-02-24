//
// selectLock.hpp
//
//   by Paul Puhnaty - 2/07/2024
//
//   A custom spin lock where a master thread decides the
//   order in which threads will acquire the lock.
//


#ifndef SELECT_LOCK_HPP
#define SELECT_LOCK_HPP


// int32_t
#include <cstdint>


class selectLock
{
public:

	// Attempt to acquire the lock
	void lock(int32_t id);
	
	// Release the lock for the next thread
	void unlock();
	
	
	// The selecting thread will spin until it is needed
	bool wait();
	
	// The selecting thread chooses the next thread to acquire the lock
	void select(int32_t id);
	
	
	// Allows all threads into their critical sections
	void disable();
	
	
	int32_t numSelections();

private:

	bool lockActive = true;

	bool needToSelect = true;
	
	int32_t currentSelection = -1;
	
	int32_t selections{};
};


#endif
