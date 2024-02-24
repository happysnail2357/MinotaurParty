# Assignment 2: Minotaur Party

The purpose of this assignment is to demonstrate a solution to two different parallel processing problems.

## Problems

### Problem 1: Cupcakes

The Minotaur invites guests one at a time into his labyrinth. Each guest may eat a cupcake from a platter, have a cupcake placed on the platter, 
or leave the platter alone. The guests must devise a strategy to determine when all guests have visited the labyrinth.

### Problem 2: Crystal Vase

The Minotaur has his favorite crystal vase in a showroom with a single door. In order to keep the vase safe, 
he has his guests choose from three different strategies to ensure that only one guest is in the showroom at any given time. 

1. Guests can attempt to enter the showroom whenever they like, but first, they must check to see if the door is open and nobody is already inside.
1. Place a sign at the showroom door. When entering the room, the guests must set the sign to "BUSY", and when they leave, 
they must set the sign to "AVAILABLE".
1. Guests can line up in a queue. Whenever a guest leaves they let the guest at the front of the queue know the showroom is available. 
Guests can queue multiple times.

## Solutions

### Problem 1 Solution

A strategy the guests could use is as follows:

- Have one guest act as a "counter".
- The rest of the guests will eat a cupcake if there is a cupcake on the platter and they haven't had one yet; otherwise, they leave the platter alone.
- When the counting guest comes to the platter, if the cupcake is gone, 1 is added to the count, and the cupcake is replaced; otherwise, they leave the platter alone.
- When the count reaches the number of guests minus 1, it can be assured all the guests have visited the labyrinth at least once.

### Problem 2 Solution

1. The first strategy is very similar to a TestAndSet lock. Guests will likely gather around the doorway trying to get in. This is not desirable since in the shuffle, not everyone is guaranteed a chance at getting in the showroom; furthermore, it will make it difficult for the person in the showroom to get out!
1. The second strategy is very similar to a TestandTestAndSet lock. Guests will wait while the sign says "BUSY" and mind their business in the meantime. But when the sign changes to "AVAILABLE", there will be a rush of guests trying to get into the showroom at the same time. So this strategy also devolves into chaos like the previous one.
1. The third strategy is very similar to a queue lock. In this strategy guests will line up and wait for the person in front of them to let them know when they can enter the showroom. There are several benefits to this strategy. The focus of waiting is split among all the guests so there is not a big rush when the time comes to let another person into the showroom. As long as everyone leaves the showroom at some point, everyone will get to enter the showroom at some point.

The third strategy appears to be the best solution and will be implemented in the code.

## Build Instructions

Make sure that a c++ compiler (g++) is installed on you device, and then execute the following (Linux) commands in a terminal:

To compile...

$ `g++ cupcake.cpp selectLock.cpp -o cupcake`

$ `g++ crystalvase.cpp queueLock.cpp -o crystalvase`

To run...

$ `./cupcake`

$ `./crystalvase`

Optionally, an integer representing the number of threads can be passed as a parameter (min 2, max 100)...

$ `./cupcake 8`

$ `./crystalvase 8`

