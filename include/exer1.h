#ifndef __EXER_1__
#define __EXER_1__

#include<string>

// Message constants
#define INITIAL -1
#define NO_INTERSECT 0
#define BOTH_INTERIOR 1
#define BOTH_ENDPOINT 2
#define ENDPOINT_INTERIOR 3
#define BOTH_SUBSET_SEGMENT 4
#define SUBSET_OF_ANOTHER 5

void exer1initialization(bool fromFile = false, std::string fileName = "nothing"); // Exercice 1 initialization
void exer1executeTest(); // Execute intersection test
void exer1stepBack(); // A segment removed
void exer1clear(); // All segments removed

#endif
