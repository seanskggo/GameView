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
typedef struct hunter {
	PlaceId location;
} Hunter;

struct hunterView {
	int round;
	GameView gv;
	Player current;
	Hunter *hunter;
	Map map;
};

////////////////////////////////////////////////////////////////////////
// Helper Function Declarations
int helperComparePlaceIds(const void *a, const void *b);

////////////////////////////////////////////////////////////////////////
// Constructor
HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);
	new->round = GvGetRound(new->gv);
	new->current = GvGetPlayer(new->gv);

	new->hunter = malloc(NUM_PLAYERS * sizeof(*new->hunter));

	// initialise hunters
	for (int i = 0; i < NUM_PLAYERS; i++) {
		new->hunter[i].location = GvGetPlayerLocation(new->gv, i);
	}

	return new;
}

// Deconstructor
void HvFree(HunterView hv)
{
	GvFree(hv->gv);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information
Round HvGetRound(HunterView hv)
{
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv)
{
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player)
{
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	return GvGetPlayerLocation(hv->gv, player);
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions
PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	int numReturnedLocs = 0;
	bool canFree = false;
	PlaceId *DracRevealedLocations = GvGetLocationHistory(hv->gv, 
	    PLAYER_DRACULA, &numReturnedLocs, &canFree);
    
    // Dracula has made a turn
	int roundNum = numReturnedLocs - 1;
	while (roundNum >= 0) {
	    // Dracula has a last known location
	    if (DracRevealedLocations[roundNum] != CITY_UNKNOWN && 
	            DracRevealedLocations[roundNum] != SEA_UNKNOWN) {
	        *round = roundNum;
	        PlaceId LastKnownLocation = DracRevealedLocations[roundNum];
	        free(DracRevealedLocations);
	        return LastKnownLocation;
	    } else roundNum--;
	}
	
	// Dracula has not been spotted yet
	// Returns NOWHERE
	free(DracRevealedLocations);
	return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
    int *pathLength)
{
	if (hv->hunter[hunter].location == NOWHERE) {
		*pathLength = 0;
		return NULL;
	}
	
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
			}
			int numPlaces = 0;
			PlaceId *reachable = GvGetReachable(hv->gv, hunter, round,
                p, &numPlaces);

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
		// reorder the array
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
	if (hv->hunter[PLAYER_DRACULA].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	
	return GvGetReachable(hv->gv, hv->current, hv->round,
        hv->hunter[hv->current].location, numReturnedLocs);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
    bool boat, int *numReturnedLocs)
{
	if (hv->hunter[PLAYER_DRACULA].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	
	return GvGetReachableByType(hv->gv, hv->current, hv->round,
        hv->hunter[hv->current].location,
        road, rail, boat, numReturnedLocs);
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
    int *numReturnedLocs)
{
	if (hv->hunter[player].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	switch(player) {
		PlaceId dracCurrentLoc;
		case PLAYER_LORD_GODALMING:
		case PLAYER_MINA_HARKER:
		case PLAYER_VAN_HELSING:
		case PLAYER_DR_SEWARD:
			return GvGetReachable(hv->gv, player, hv->round,
                hv->hunter[player].location,
                numReturnedLocs);
			break;
		case PLAYER_DRACULA:
		// check whether Drac's current location is revealed
		dracCurrentLoc = GvGetPlayerLocation(hv->gv, player);
		if (placeIsReal(dracCurrentLoc)) {
			return GvGetReachable(hv->gv, player, hv->round,
                dracCurrentLoc, numReturnedLocs);
		}
		
			break;
	}
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
    bool road, bool rail, bool boat, int *numReturnedLocs)
{
	if (hv->hunter[player].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	switch(player) {
		PlaceId dracCurrentLoc;
		case PLAYER_LORD_GODALMING:
		case PLAYER_MINA_HARKER:
		case PLAYER_VAN_HELSING:
		case PLAYER_DR_SEWARD:
			return GvGetReachableByType(hv->gv, player, hv->round,
                hv->hunter[player].location, road, rail,
                boat, numReturnedLocs);
			break;
		case PLAYER_DRACULA:
			// need to check whether Drac's current loc is revealed
			dracCurrentLoc = GvGetPlayerLocation(hv->gv, player);
			if (placeIsReal(dracCurrentLoc)) {
				return GvGetReachableByType(hv->gv, player, hv->round,
                    dracCurrentLoc, road, rail,
                    boat, numReturnedLocs);
			}
			break;
	}
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
