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


class selectLock
{
public:

	// Attempt to acquire the lock
	void waitForTurn(int id);
	
	// Release the lock for the next thread
	void finishTurn();
	
	
	// The selecting thread will spin until it is needed
	bool masterWait();
	
	// The selecting thread chooses the next thread to acquire the lock
	void masterSelect(int id);
	
	
	// Allows all threads into their critical sections
	void disable();

private:

	bool lockActive = true;

	bool needToSelect = true;
	
	int currentSelection = -1;
};


#endif
