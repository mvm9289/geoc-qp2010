#ifndef __EXER_2__
#define __EXER_2__

#include<string>
using namespace std;

// Message constants
#define INITIAL -1

// Point positions constants
#define INSIDE 0
#define OUTSIDE 1
#define ON_SIDE 2
#define ON_VERTEX 3

void exer2stepBack(); // Decrement the counter of next point to analyze (last point has been removed)
void exer2reset(); // Reset the next point to analyze
void exer2initialization(bool fromFile = false, string fileName = "Nothing"); // Print initial messages and execute first test if needed
void exer2executeTest(); // Execute test

#endif