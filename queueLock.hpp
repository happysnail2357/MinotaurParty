//
// queueLock.hpp
//
//   by Paul Puhnaty - 2/23/2024
//
//   An array-based spin lock where each thread waits in a queue.
//


#ifndef QUEUE_LOCK_HPP
#define QUEUE_LOCK_HPP


// Vectors
#include <vector>

// Atomic variables
#include <atomic>


class queueLock
{
public:

	queueLock(int32_t size);

	// Attempt to acquire the lock
	int32_t lock();

	// Release the lock for the next thread
	void unlock(int32_t key);
	
	// Get the number of times a thread entered the critical section
	int32_t numberOfAccesses();

private:

	int32_t size;
	
	std::atomic<int32_t> next;
	
	std::vector<bool> flag;
	
	std::atomic<int32_t> accesses;
};


#endif
