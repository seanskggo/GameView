////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

typedef struct character {
	PlaceId location;
} Character;

struct draculaView {
	int round;
	GameView gv;
	Character *player;
};

#define MAX_DOUBLE_BACKS 5

// Helper Functions

static PlaceId *helperRemoveCurrentLocation(DraculaView dv, PlaceId *moves,
	int *currentNumMoves);
static PlaceId *helperGetDoubleBacks(DraculaView dv, PlaceId *reachableLocs,
	int numReachableLocs, PlaceId *lastLocs, int numLastLocs,
	PlaceId *availableMoves, int *currentNumMoves);
static PlaceId *helperRemoveLoc(PlaceId *availableMoves, int index, int *size);

////////////////////////////////////////////////////////////////////////
// Constructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	DraculaView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}
	
	new->gv = GvNew(pastPlays, messages);

	// initialise players
	new->player = malloc(NUM_PLAYERS * sizeof(*new->player));

	for (int i = 0; i < NUM_PLAYERS; i++) {
		new->player[i].location = GvGetPlayerLocation(new->gv, i);
	}
	return new;
}
// Destructor

void DvFree(DraculaView dv)
{
	free(dv->player);
	GvFree(dv->gv);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv)
{
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player)
{
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	*numReturnedMoves = 0;
	if (dv->player[PLAYER_DRACULA].location == NOWHERE) {
		return NULL;
	}
	
	int numReachableLocs;
	PlaceId *reachableLocs = GvGetReachable(dv->gv, PLAYER_DRACULA, 
        dv->round, dv->player[PLAYER_DRACULA].location, &numReachableLocs);

	// we get drac's last 5 moves
	int numLastMoves = 0;
	bool canFreeLastMoves = true;
	PlaceId *lastMoves = GvGetLastMoves(dv->gv, PLAYER_DRACULA, 5,
        &numLastMoves, &canFreeLastMoves);

	// we may also need his last 5 LOCATIONS
	int numLastLocs = 0;
	bool canFreeLastLocs = true;
	PlaceId *lastLocs = GvGetLastLocations(dv->gv, PLAYER_DRACULA, 5,
        &numLastLocs, &canFreeLastLocs);

	bool foundDoubleBack = false;
	bool foundHide = false;
	for (int i = 0; i < numLastMoves; i++) {
		if (GvIsDoubleBack(lastMoves[i])) {
			foundDoubleBack = true;
		} if (lastMoves[i] == HIDE) {
			foundHide = true;
		}
	}

	// add special moves and return
	*numReturnedMoves = numReachableLocs;

	PlaceId *availableMoves = reachableLocs;
	// if no double backs/hides, just return the reachableLocs
	// and all available DOUBLE_BACKs and the hide
	if (!foundDoubleBack && !foundHide && reachableLocs != NULL) {
		// so numReturnedMoves is 5 moves short
		// it includes the HIDE move, but not DOUBLE_BACK1
		// but we still need to realloc space for HIDE
		availableMoves = helperGetDoubleBacks(dv, reachableLocs, 
            numReachableLocs, lastLocs, numLastLocs, availableMoves,
			numReturnedMoves);

		// add HIDE move if not at sea
		if (!placeIsSea(dv->player[PLAYER_DRACULA].location)) {
			availableMoves = realloc(availableMoves, (1 + *numReturnedMoves) *
				sizeof(PlaceId));
			availableMoves[*numReturnedMoves] = HIDE;
			*numReturnedMoves += 1;
		}
		
		availableMoves = helperRemoveCurrentLocation(dv, availableMoves,
		    numReturnedMoves);
		
		if (canFreeLastLocs) {
			free(lastLocs);
		}
		
		if (canFreeLastMoves) {
			free(lastMoves);
		}
	
		return availableMoves;
	}


	// if both double back and hide, only available moves are
	// what reachableLocs describes
	if (foundDoubleBack && foundHide) {
		if (canFreeLastLocs) {
			free(lastLocs);
		}
		
		if (canFreeLastMoves) {
			free(lastMoves);
		}
		return reachableLocs;
	}

	// if a hide, add the double back moves
	if (foundHide) {
		// we have to swap the last element of available 
		availableMoves = helperGetDoubleBacks(dv, reachableLocs,
			numReachableLocs, lastLocs, numLastLocs, availableMoves,
			numReturnedMoves);
			
		availableMoves = helperRemoveCurrentLocation(dv, availableMoves,
		numReturnedMoves);
		
		if (canFreeLastLocs) {
			free(lastLocs);
		}
		
		if (canFreeLastMoves) {
			free(lastMoves);
		}

		return availableMoves;
	}

	// if we've found a DoubleBack, there is only one move to add: HIDE
	if (foundDoubleBack) {
		// add HIDE move if not at sea
		if (!placeIsSea(dv->player[PLAYER_DRACULA].location)) {
			availableMoves = realloc(availableMoves, (1 + *numReturnedMoves) *
				sizeof(PlaceId));
			availableMoves[*numReturnedMoves] = HIDE;
			*numReturnedMoves += 1;
		}
		
		availableMoves = helperRemoveCurrentLocation(dv, availableMoves,
		    numReturnedMoves);
		
		if (canFreeLastLocs) {
			free(lastLocs);
		}
		
		if (canFreeLastMoves) {
			free(lastMoves);
		}

		return availableMoves;
	}

	if (canFreeLastMoves) {
		free(lastMoves);
	}

	*numReturnedMoves = 0;
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	if (dv->player[PLAYER_DRACULA].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	
	return GvGetReachable(dv->gv, PLAYER_DRACULA, dv->round,
        dv->player[PLAYER_DRACULA].location,
        numReturnedLocs);
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
    int *numReturnedLocs)
{
	if (dv->player[PLAYER_DRACULA].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	return GvGetReachableByType(dv->gv, PLAYER_DRACULA, dv->round,
        dv->player[PLAYER_DRACULA].location, road,
        false, boat, numReturnedLocs);
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
    int *numReturnedLocs)
{
	if (dv->player[player].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	return GvGetReachable(dv->gv, player, dv->round,
        dv->player[player].location, numReturnedLocs);
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
    bool road, bool rail, bool boat,
    int *numReturnedLocs)
{
	if (dv->player[player].location == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	return GvGetReachableByType(dv->gv, player, dv->round,
        dv->player[player].location, road, rail,
        boat, numReturnedLocs);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions


////////////////////////////////////////////////////////////////////////
// Helper functions

static PlaceId *helperRemoveCurrentLocation(DraculaView dv, PlaceId *moves,
	int *currentNumMoves)
{
	int i = 0;
	for (; i < (*currentNumMoves); i++) {
		if (moves[i] == dv->player[PLAYER_DRACULA].location) {
			for (; i < (*currentNumMoves - 1); i++) {
				moves[i] = moves[i + 1];
			}
			*currentNumMoves -= 1;
			break;
		}
	}
	return moves;
}

static PlaceId *helperGetDoubleBacks(DraculaView dv, PlaceId *reachableLocs,
	int numReachableLocs, PlaceId *lastLocs, int numLastLocs,
	PlaceId *availableMoves, int *currentNumMoves)
{
	// what we need to do is loop through reachAbleLocs (outer loop)
	// and compare each of it's locs to those in (numLastLocs) (inner loop)
	// if something is in both, we can double back to it

	// we also need to store which "number" DOUBLE_BACK we need to do
	bool canDoDoubleBack[MAX_DOUBLE_BACKS] = {false};
	int numDoubleBacks = 0;
	
	int timesOuterLooped = 0;
	for (int i = (numLastLocs - 1); i >= 0; i--) {
		bool alreadyFound = false;
		for (int j = 0; j < *currentNumMoves; j++) {
			if (!alreadyFound && availableMoves[j] == lastLocs[i]) {
				alreadyFound = true;
				canDoDoubleBack[timesOuterLooped] = true;
				numDoubleBacks++;
				helperRemoveLoc(availableMoves, j, currentNumMoves);
			}
		}
		timesOuterLooped++;
	}

	// now we add our double backs
	int newNumMoves = *currentNumMoves + numDoubleBacks;
	availableMoves = realloc(availableMoves, newNumMoves * sizeof(PlaceId));

	// for loop adding the appropriate double backs
	for (int i = 0; i < numDoubleBacks; i++) {
		for (int j = 0; j < MAX_DOUBLE_BACKS; j++) {
			if (canDoDoubleBack[j]) {
				availableMoves[*currentNumMoves + i] = DOUBLE_BACK_1 + j;
				canDoDoubleBack[j] = false;
				break;
			}
		}
	}
	*currentNumMoves = newNumMoves;
	return availableMoves;
}

static PlaceId *helperRemoveLoc(PlaceId *availableMoves, int index, int *size) {
	
	for (int i = index; i < (*size - 1); i++) {
		availableMoves[i] = availableMoves[i + 1];
	}
	*size -= 1;
	return availableMoves;
}
