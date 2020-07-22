////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
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
#include "Map.h"
#include "Places.h"
// add your own #includes here

#include "PlayerRep.h"

// TODO: ADD YOUR OWN STRUCTS HERE

struct gameView {
	// must include: #rounds, players ADT (must include health and location),
	// current score and the map.

	PlayerRep *players;
	int encounterLocations[NUM_REAL_PLACES];
	int currentPlayer;
	int round;
	int score;
	Map map;
};

// #defines here
#define NUM_HUNTERS 	4
#define PLAY_LENGTH 	8

#define ONE_TRAP		1
#define TWO_TRAPS		2
#define IM_VAMP			3
#define TRAP_IM_VAMP	4
#define TWO_IM_VAMP		5

// Local Function Declarations

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	/*
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}

	// initialise all variables inside gameview ADT

	new->score = calcGameScore(pastPlays);
	calcEncounterLocations(pastPlays);
	new->round = calcRound(pastPlays);

	new->map = MapNew();
	// I believe that we don't need to handle adding connections to the map
	// ourselves — see Map.c

	// NOTE: need to get player locations instead of having them all
	// be at Amsterdam
	// NOTE: this should *probably* be chucked in a function too
	// NOTE: since I added an additional "trail" element to PlayerRep,
	// that needs to be handled too. We will palm off the work of finding the
	// trail to calcGameState()...
	char **locations = findPlayerLocations(pastPlays);
	new->players = malloc(NUM_PLAYERS * sizeof(PlayerRep));
	for (int i = 0; i < NUM_HUNTERS; i++) {
		new->players[i] = PlayerRepNew(GAME_START_HUNTER_LIFE_POINTS,
			NULL, locations[i]
		);
	}

	new->players[PLAYER_DRACULA] = PlayerRepNew(GAME_START_BLOOD_POINTS,
		NULL, "AM\0"
	);

	free(locations);

	return new; */
	return NULL;
}

void GvFree(GameView gv)
{
	for(int i = 0; i < NUM_PLAYERS; i++) {
		PlayerRepFree(gv->players[i]);
	}

	MapFree(gv->map);
	free(gv);
	return;
}

////////////////////////////////////////////////////////////////////////
// Game State Information

// Modified by Sean
Round GvGetRound(GameView gv)
{
	// Input gv
	// Output return struct Round with integer to #rounds

	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
	// Input gv
	// Output player enumerator value of the player currently playing

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return PLAYER_LORD_GODALMING;
}

int GvGetScore(GameView gv)
{
	// Input gv
	// Output integer value of the current score

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int GvGetHealth(GameView gv, Player player)
{
	// Input gv, player
	// Output integer value of the current player's health

	// Player's health must not exceed 9 (0 to 9 inclusive)
	// Dracula's health >= 0

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// Input gv, player
	// Output the integer value assigned to the location (placeID)
		// of where the player is currently at. Return "NOWHERE" if the player
		// selected has not made a turn yet (before their first turn). Return
		// PlaceID if it is a hunter. Return PlaceID if it is Dracula AND his
		// current location is revealed in the play string. Return CITY_UNKNOWN
		// or SEA_UNKNOWN if player is dracula and has not been revealed.

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// Input gv
	// Output the integer value assigned to the location (placeID)
		// of where the immature vampire are currently at in a dynamically
		// allocated array. order does not matter. Return PlaceId if
		// location has been revealed. Return CITY_UNKNOWN if location
		// has not been revealed. Return NOWHERE if no vampires exist

	// Traverse the map in gv and copy the placeID in the dynamically allocated
		// array.

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// Input gv, numTraps
	// Output the location of the traps currently on the map in a
		// dynamically allocated array. Including multiple copies if multiple
		// traps in a single location and order does not matter.

	// Traverse the map in gv and copy the placeID in the dynamically allocated
		// array.

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

////////////////////////////////////////////////////////////////////////
// Local Helper Functions









// Old helper functions. They are no longer declared at the top of the file.

/*static int calcGameScore(char *pastPlays) {
	// TO-DO: write this!
	return 0;
}

static void calcEncounterLocations(char *pastPlays) {
	// TO-DO: write this!
	return;
}

static int calcRound(char *pastPlays) {
	// I think we start at round one...not zero...?
	int round = 1;
	for (int i = 0; pastPlays[i] != '\0'; i++) {
		// Round increments after all players have made their move
		if (i % NUM_PLAYERS * PLAY_LENGTH == 0) {
			round++;
		}
	}
	// PLAY_LENGTH takes into account spaces, yet the very last play doesn't
	// have a space (I THINK). So increment round.
	round++;

	return round;
}

char **findPlayerLocations(char *pastPlays) {
	 // TO-DO: write this!
	 // Note that it will probably require malloc() — I've already
	 // added the corresponding free in GvNew()
	 return NULL;
} */
