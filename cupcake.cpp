//
// cupcake.cpp
//
//   by Paul Puhnaty - 2/23/2024
//
//   A program to simulate the Minotaur's Cupcake Problem
//


/*/        Includes        /*/

// IO
#include <iostream>

// C++ Strings
#include <string>

// Multi-threading
#include <thread>

// Vectors
#include <vector>

// Random Generator
#include <cstdlib>
#include <time.h> 

// Custom Spin Lock
#include "selectLock.hpp"


/*/  Forward Declarations  /*/

bool parseArguments(int32_t argc, char* argv[], int32_t* numThreads);
void printUsage();
void guestLeaderProtocol(int32_t id, bool* cupcake, selectLock* minotaur, bool* breakout, int32_t numGuests);
void guestProtocol(int32_t id, bool* cupcake, selectLock* minotaur, bool* breakout);


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
	
	
	bool cupcake = true;
	
	selectLock minotaur;
	
	bool breakout = false;
	
	
	std::vector<std::thread> thrds;
	
	thrds.push_back(std::thread(guestLeaderProtocol, 1, &cupcake, &minotaur, &breakout, numThreads));
	
	for (int32_t threadID = 2; threadID <= numThreads; threadID++)
	{
		thrds.push_back(std::thread(guestProtocol, threadID, &cupcake, &minotaur, &breakout));
	}
	
	// Begin selecting threads...
	//
	while (minotaur.wait())
	{
		minotaur.select((rand() % numThreads) + 1);
	}
	
	// Sync with threads
	//
	for (auto it = thrds.begin(); it != thrds.end(); it++)
	{
		(*it).join();
	}
	
	
	int32_t numVisits = minotaur.numSelections();
	
	std::cout << "There were " << numVisits << " visits to the labyrinth in total\n\n";
	
	
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
	std::cout << "\n  Usage: ./cupcake [optional: number of threads]\n\n";
}


void guestLeaderProtocol(int32_t id, bool* cupcake, selectLock* minotaur, bool* breakout, int32_t numGuests)
{
	int32_t numGuestsVisited = 0;
	
	while (true)
	{
		// Wait for selection by the Minotaur
		minotaur->lock(id);
		
		if (*breakout)
			return;
		
		if (!(*cupcake))
		{
			numGuestsVisited++;
			
			// Report visit
			std::cout << "Guest " << id << ": count " << numGuestsVisited << "\n";
			
			if (numGuestsVisited == numGuests - 1)
			{
				std::cout << "\nGuest " << id << ": All guests have visited the Labyrinth!\n\n";
				
				// Signal all threads to terminate
				*breakout = true;
				
				// Allow all threads to terminate
				minotaur->disable();
				
				return;
			}
			
			*cupcake = true;
		}
		else
		{
			// Report visit
			std::cout << "Guest " << id << ": count " << numGuestsVisited << "\n";
		}
		
		minotaur->unlock();
	}
}

void guestProtocol(int32_t id, bool* cupcake, selectLock* minotaur, bool* breakout)
{
	bool hadCupcake = false;
	
	while (true)
	{
		// Wait for selection by the Minotaur
		minotaur->lock(id);
		
		if (*breakout)
			return;
		
		//std::cout << "Guest " << id << ": In the labyrinth\n";
		
		// Only eat the cupcake if
		//   1. haven't had one yet, and
		//   2. there is a cupcake to eat
		//
		if (!hadCupcake && *cupcake)
		{
			*cupcake = false;
			
			hadCupcake = true;
		}
		
		minotaur->unlock();
	}
}
