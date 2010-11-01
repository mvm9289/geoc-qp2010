#ifndef __EXER_3__
#define __EXER_3__

#include<string>
using namespace std;

// Message constants
#define INITIAL -1

// Point positions constants
#define INSIDE 0
#define OUTSIDE 1
#define ON_SIDE 2

void exer3stepBack(); // Decrement the counter of next point to analyze (last point has been removed)
void exer3reset(); // Reset the next point to analyze
void exer3initialization(bool fromFile = false, string fileName = "Nothing"); // Print initial messages and execute first test if needed
void exer3executeTest(); // Execute test

#endif