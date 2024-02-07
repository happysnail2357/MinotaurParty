# Assignment 2: Minotaur Party

The purpose of this assignment is to demonstrate a solution to two different parallel processing problems.

## Problems

### Problem 1: Cupcakes

The Minotaur invites guests one at a time into his labyrinth. Each guest may eat a cupcake from a platter, have a cupcake placed on the platter, 
or leave the platter alone. The guests must devise a strategy to determine when all guests have visited the labyrinth.

### Problem 2: Crystal Vase

The Minotaur has his favorite crystal vase in a showroom with a single door. In order to keep the vase safe, 
he has his guests choose from three different strategies to ensure that only one guest is in the showroom at any given time. 

1. Guests can attempt to enter the showroom whenever they like and see if anybody is there.
1. Place a sign at the showroom door. When entering the room, the guests must set the sign to "BUSY", and when they leave, 
they must set the sign to "AVAILABLE".
1. Guests can line up in a queue. Whenever a guest leaves they let the guest at the front of the queue know the showroom is available. 
Guests can enqueue themselves multiple times.

## Solutions

### Problem 1 Solution

A strategy the guests could use is as follows:

- Have one guest act as a "counter".
- The rest of the guests will eat a cupcake if there is a cupcake on the platter and they haven't had one yet; otherwise, they leave the platter alone.
- When the counting guest comes to the platter, if the cupcake is gone, 1 is added to the count, and the cupcake is replaced; otherwise, they leave the platter alone.
- When the count reaches the number of guests minus 1, it can be assured all the guests have visited the labyrinth at least once.

### Problem 2 Solution

