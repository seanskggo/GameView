////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testMap.c: a simple program that checks the Map ADT
// You can change this as much as you want!
// You do not need to submit this file.
//
// 2020-07-10	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Map.h"
#include "Places.h"

#define MAX_LINE 1024

int main(void)
{
	Map m = MapNew();
	// MapShow(m);

	char buffer[MAX_LINE];

	printf("\nType a location name to see its connections: ");
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0'; // remove newline character

		PlaceId place = placeNameToId(buffer);
		if (place == NOWHERE) {
			printf("Unknown location '%s'\n", buffer);
		} else {
			for (ConnList c = MapGetConnections(m, place); c != NULL; c = c->next) {
				const char *dest = placeIdToName(c->p);
				const char *transportType = transportTypeToString(c->type);
				printf("%s connects to %s by %s\n", buffer, dest, transportType);
			}
		}

		//printf("\nType a location name to see its connections: ");
	}

	printf("\n");

	// Additional tests written by Sam for MapGetRailReachable():

	int numReturnedLocs = 0;
	ConnList reachableLocs = NULL;
	
	int *added = malloc(NUM_REAL_PLACES * sizeof(int));

	// initialise added
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}

	// Test 1: test that the function returns no values when
	// there are no places reachable by rail
	int dist = 3;
	reachableLocs = MapGetRailReachable(m, GALWAY, dist, reachableLocs, 	
		&numReturnedLocs, added);
	assert(numReturnedLocs == 0 && reachableLocs == NULL);
	printf("Test 1 Passed!\n\n");

	// Test 2: test that the function returns the correct number
	// of values for a specified location with distance 1

	// Reset test variables
	dist = 1;
	reachableLocs = NULL;
	numReturnedLocs = 0;
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}
	// End resetting test variables
	reachableLocs = MapGetRailReachable(m, MADRID, dist, reachableLocs, 
		&numReturnedLocs, added);
	ConnList curr = reachableLocs;
	printf("Test 2:\n");
	while (curr != NULL) {
		fputs(placeIdToName(curr->p), stdout);
		printf("\n");
		curr = curr->next;
	}

	assert(numReturnedLocs == 4 && MapConnListLength(reachableLocs) == 4);
	printf("Test 2 Passed!\n\n");
	
	// Test 3: test that the function returns the correct number
	// of values for a specified location with distance 2

	// Reset test variables
	dist = 2;
	reachableLocs = NULL;
	numReturnedLocs = 0;
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}
	// End resetting test variables
	reachableLocs = MapGetRailReachable(m, PARIS, dist, reachableLocs, 
		&numReturnedLocs, added);
	curr = reachableLocs;
	printf("Test 2:\n");
	while (curr != NULL) {
		fputs(placeIdToName(curr->p), stdout);
		printf("\n");
		curr = curr->next;
	}

	assert(numReturnedLocs == 6 && MapConnListLength(reachableLocs) == 6);
	printf("Test 3 Passed!\n\n");

	// Test 4: test that the function returns the correct number of 
	// values for a specified location with distance 3
	
	// Reset test variables
	dist = 3;
	reachableLocs = NULL;
	numReturnedLocs = 0;
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}
	// End resetting test variables
	reachableLocs = MapGetRailReachable(m, VIENNA, dist, reachableLocs, 
		&numReturnedLocs, added);
	curr = reachableLocs;
	printf("Test 4:\n");
	printf("Actual test: \n");
	while (curr != NULL) {
		fputs(placeIdToName(curr->p), stdout);
		printf("\n");
		curr = curr->next;
	}
	
	assert(numReturnedLocs == 9 && MapConnListLength(reachableLocs) == 9);
	printf("Test 4 Passed!\n\n");
	
	// Test 5: test that the function returns the correct number of 
	// values for a specified location with distance 3
	
	// Reset test variables
	dist = 3;
	reachableLocs = NULL;
	numReturnedLocs = 0;
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}
	reachableLocs = MapGetRailReachable(m, ZURICH, dist, reachableLocs, 
		&numReturnedLocs, added);
	curr = reachableLocs;
	printf("Test 5:\n");
	while (curr != NULL) {
		fputs(placeIdToName(curr->p), stdout);
		printf("\n");
		curr = curr->next;
	}
	
	assert(numReturnedLocs == 9 && MapConnListLength(reachableLocs) == 9);
	printf("Test 5 Passed!\n\n");
	
	
	// Test 6: testing a specific, odd case that caused an issue 
	// in my tests for GameView.c
	
	dist = 2;
	reachableLocs = NULL;
	numReturnedLocs = 0;
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}
	reachableLocs = MapGetRailReachable(m, VIENNA, dist, reachableLocs, 
		&numReturnedLocs, added);
	curr = reachableLocs;
	printf("Test 6:\n");
	while (curr != NULL) {
		fputs(placeIdToName(curr->p), stdout);
		printf("\n");
		curr = curr->next;
	}
	
	assert(numReturnedLocs == 5 && MapConnListLength(reachableLocs) == 5);
	printf("Test 6 Passed!\n\n");

}

