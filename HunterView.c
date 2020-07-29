////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
// #includes that we added here
// Queue ADT written by John Shepherd, March 2013
// Item ADT it relies on written by John Shepherd, March 2013,
// modified by Sam Schreyer.
#include "Queue.h"

#define NUM_PLAYERS 4

// Character struct for storing character information
typedef struct character {
	int health;
	int numMoves;
	// I need the location for my func
	PlaceId location;
	PlaceId *moves;
	bool canFreeMoves;
} Character;

struct hunterView {
	// need round for my func
	int round;
	int score;
	GameView gv;
	Player current;
	PlaceId vampLoc;
	Character *hunter;
	PlaceId *revealedDracMovesHistory;
	int numDracMovesHistory;
	bool canFreeDracHistory;
	Map map;
};

////////////////////////////////////////////////////////////////////////
// Helper Function Declarations
int helperComparePlaceIds(const void *a, const void *b);

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);
	new->round = GvGetRound(new->gv);
	new->score = GvGetScore(new->gv);
	new->current = GvGetPlayer(new->gv);

	new->hunter = malloc(NUM_PLAYERS * sizeof(*new->hunter));

	// initialise hunters
	for (int i = 0; i < NUM_PLAYERS; i++) {
		new->hunter[i].health = GvGetHealth(new->gv, i);
		new->hunter[i].moves = GvGetMoveHistory(new->gv, i,
			&new->hunter[i].numMoves, &new->hunter[i].canFreeMoves);
		new->hunter[i].location = GvGetPlayerLocation(new->gv, i);
	}
	new->revealedDracMovesHistory = GvGetMoveHistory(new->gv, PLAYER_DRACULA,
	                          &new->numDracMovesHistory,
							  &new->canFreeDracHistory);
	return new;
}

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return hv->round;
}

Player HvGetPlayer(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return hv->current;
}

int HvGetScore(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return hv->score;
}

int HvGetHealth(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// HvNew doesn't cover drac fyi!!!
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*round = 0;
	return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	// we have an added array to check if we've already realised
	// we can access that location
	int *visited = malloc(NUM_REAL_PLACES * sizeof(int));

	// initialise visited
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		visited[i] = -1;
	}
	
	// We add the source to our added array
	Queue q = newQueue();
	PlaceId hunterLoc = GvGetPlayerLocation(hv->gv, hunter);
	QueueJoin(q, hunterLoc);
	visited[hunterLoc] = hunterLoc;
	bool found = false;
	// round depends on the hunter...
	// i.e. if it is round 2 but PLAYER_DR_SEWARD was already moved,
	// his next move will be in round 3!!!

	// i.e.
	int round;
	if (hunter < hv->current) {
		round = hv->round++;
	} else {
		round = hv->round;
	}
	// int holds the number of locations at each "stage" of the BFS
	int numBFSStageLocs = 1;
	int nextStageLocs = 0;
	while (!QueueIsEmpty(q) && !found) {
		// this for loop runs once on the first pass through, 
		// but multiple times every other time
		for(int i = 0; i < numBFSStageLocs; i++) {
			PlaceId p = QueueLeave(q);

			if (p == dest) {
				found = true;
				//break;
				// do we need a break here? not in pseudocode...
			}
			// increment round every time? round++;
			int numPlaces = 0;
			PlaceId *reachable = GvGetReachable(hv->gv, hunter, round,
				                    p, &numPlaces);
				                    
		    
			// this MAY NOT work!!!
			for (int i = 0; i < numPlaces; i++) {
				if (visited[reachable[i]] == -1) {
					QueueJoin(q, reachable[i]);
					visited[reachable[i]] = p;
					nextStageLocs++;
				}
			}
			free(reachable);
		}
		numBFSStageLocs = nextStageLocs;
		nextStageLocs = 0;
		round++;
		
	}

	*pathLength = 0;
	int *tempPathArray = malloc(NUM_REAL_PLACES * sizeof(int));
	PlaceId* path = malloc(NUM_REAL_PLACES * sizeof(int));
	if (found) {
	
		tempPathArray[0] = dest;
		*pathLength += 1;
		int tempPathIndex = 1;
		
		for (int i = dest; visited[i] != hv->hunter[hunter].location;
			i = visited[i]) {
			tempPathArray[tempPathIndex] = visited[i];
			*pathLength += 1;
			tempPathIndex++;
		}
		tempPathIndex--;
		//reorder the array
		for(int i = 0; i < *pathLength; i++) {
			path[i] = tempPathArray[tempPathIndex];
			tempPathIndex--;
		}

	}
	
	if (*pathLength == 0) {
		free(path);
	}
	
	free(tempPathArray);
	free(visited);

	return (*pathLength != 0) ? path : NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

