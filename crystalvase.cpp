//
// crystalvase.cpp
//
//   by Paul Puhnaty - 2/23/2024
//
//   A program to simulate the Minotaur's Crystal Vase Problem
//


/*/        Includes        /*/

// IO
#include <iostream>

// Multi-threading
#include <thread>

// Vectors
#include <vector>

// Random Number Generator
#include <cstdlib>
#include <time.h>

// Atomic variables
#include <atomic>


// Custom Lock
#include "queueLock.hpp"


/*/  Forward Declarations  /*/

bool parseArguments(int32_t argc, char* argv[], int32_t* numThreads);
void printUsage();
void guest(int32_t id, queueLock* showroom, int32_t visits, std::atomic<bool>* viewingCrystalVase);


/*/  Constant Definitions  /*/

const int32_t DEFAULT_NUM_THREADS = 8;
const int32_t MAX_NUM_THREADS = 100;
const int32_t MIN_NUM_THREADS = 2;


/*/      Entry Point       /*/

int main(int argc, char* argv[])
{
	srand(time(0));
	
	
	int32_t numThreads = DEFAULT_NUM_THREADS;
	
	if (!parseArguments(argc, argv, &numThreads))
	{
		return -1;
	}
	
	
	std::cout << "\nNumber of guests: " << numThreads << "\n\n";
	
	
	// A shared value used to check if mutual-exclusion fails
	//
	std::atomic<bool> viewingCrystalVase(false);
	
	// The total number of times the threads want to access the critical section
	//
	int32_t visitsRequested = 0;
	
	// The lock used to enforce mutual exclusion
	//
	queueLock showroomEntranceStrategy(numThreads);
	
	std::cout << "Starting... ";
	
	std::vector<std::thread> thrds;
	
	for (int32_t threadID = 1; threadID <= numThreads; threadID++)
	{
		// Guests can request to visit between 1 and 3 times
		//
		int32_t visits = (rand() % 3) + 1;
		
		visitsRequested += visits;
		
		thrds.push_back(std::thread(guest, threadID, &showroomEntranceStrategy, visits, &viewingCrystalVase));
	}
	
	// Sync with the threads
	//
	for (auto it = thrds.begin(); it != thrds.end(); it++)
	{
		(*it).join();
	}
	
	std::cout << "Finished\n";
	
	int32_t numVisits = showroomEntranceStrategy.numberOfAccesses();
	
	std::cout << "\nNumber of showroom visits requested: " << visitsRequested;
	std::cout << "\nActual number of visits: " << numVisits << "\n\n";
	
	
	return 0;
}


/*/  Function Definitions  /*/


bool parseArguments(int32_t argc, char* argv[], int32_t* numThreads)
{
	if (argc == 2)
	{
		std::string arg(argv[1]);
		
		if (arg.find_first_not_of("0123456789") == std::string::npos)
		{
			try
			{
				*numThreads = std::stoi(arg);
				
				if (*numThreads < MIN_NUM_THREADS)
				{
					std::cout << "\n  E: argument smaller than minimum allowed value " << MIN_NUM_THREADS << "\n";
				}
				else if (*numThreads > MAX_NUM_THREADS)
				{
					std::cout << "\n  E: argument larger than maximum allowed value " << MAX_NUM_THREADS << "\n";
				}
				else
				{
					return true;
				}
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "\n  E: invalid argument\n";
			}
			catch (std::out_of_range const& e)
			{
				std::cout << "\n  E: argument too large for 32-bit integer\n";
			}
			catch (std::exception const& e)
			{
				std::cout << "\n  " << e.what() << "\n";
			}
		}
		else
		{
			std::cout << "\n  E: argument contains non-digit characters\n";
		}
	}
	else if (argc == 1)
	{
		return true;
	}
	else
	{
		std::cout << "\n  E: too many arguments\n";
	}
	
	printUsage();
	
	return false;
}

void printUsage()
{
	std::cout << "\n  Usage: ./crystalvase [optional: number of threads]\n\n";
}


void guest(int32_t id, queueLock* showroom, int32_t visits, std::atomic<bool>* viewingCrystalVase)
{
	for (int32_t i = 0; i < visits; i++)
	{
		int32_t key = showroom->lock();
		
		if (viewingCrystalVase->exchange(true))
		{
			std::cout << "\n\nGuest " << id << ": Yikes! Somebody is already in the showroom!\n";
			
			showroom->unlock(key);
			
			return;
		}
		else
		{
			//std::cout << "Guest " << id << ": Visiting the showroom\n";
			
			viewingCrystalVase->store(false);
		}
		
		showroom->unlock(key);
	}
}
