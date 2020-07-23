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

//--------------------- Local Function Declarations --------------------

static void calcGameState(GameView Gv, char *pastPlays);
static void initialisePlayers(GameView Gv);
static void calcCurrPlayer(GameView Gv, char *currPlay);
static void updateScore(GameView Gv, char *currPlay);
static void processEncounterLocations(GameView Gv, char *currPlay);

//----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{

	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}

	initialisePlayers(new);

	calcGameState(new, pastPlays);


	new->map = MapNew();
	// I believe that we don't need to handle adding connections to the map
	// ourselves \u2014 see Map.c

	return new;
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



//------------------------ Local Helper Functions ---------------------

static void initialisePlayers(GameView Gv) {

	Gv->players = malloc(NUM_PLAYERS * sizeof(PlayerRep));
	for (int i = 0; i < NUM_HUNTERS; i++) {
		Gv->players[i] = PlayerRepNew(GAME_START_HUNTER_LIFE_POINTS, NULL, 0);
	}

	Gv->players[PLAYER_DRACULA] = PlayerRepNew(GAME_START_BLOOD_POINTS, NULL, 
		0
	);

	return;
}

static void calcGameState(GameView Gv, char *pastPlays) {
	// TO-DO: write this function using the old helper functions below...
	// and writing the code for the helper functions that were never written!

	// I think we start from round 0
	int round = 0;
	char *currPlay = malloc(PLAY_LENGTH * sizeof(char));
	int currPlayCounter;
	int j = 0;
	for (int i = 0; pastPlays[i] != '\0'; i++) {

		currPlay[currPlayCounter] = pastPlays[i];

		if (i % PLAY_LENGTH == 0 && i != 0) {

			// Last char in currentPlay should be a space in most cases?
			currPlay[j] = '\0';

			// Process Round

			// Round increments after all players have made their move
			if (i % NUM_PLAYERS * PLAY_LENGTH == 0) {
				round++;
			}

			// Update score based on the current play
			updateScore(Gv, currPlay);

			// Update current player based on the current play
			calcCurrPlayer(Gv, currPlay);

			// Update current player's trail
			PlayerRepUpdatePlayerTrail(Gv->players[Gv->currentPlayer],
				currPlay
			);
			
			PlayerRepUpdateMoveHistory(Gv->players[Gv->currentPlayer],
				currPlay
			);
			processEncounterLocations(Gv, currPlay);

			// Reset currPlayCounter cos we just processed a whole play
			currPlayCounter = 0;
		}
		j++;
	}
	// Increment round as the last play doesn't have a space (I think)
	round++;

	Gv->round = round;

	free(currPlay);

	return;
}

// Modified by Sean
static void calcCurrPlayer(GameView Gv, char *currPlay) {
	// TO-DO: Write this
	char a = currPlay[0];
	if (a == 'G') Gv->currentPlayer = PLAYER_LORD_GODALMING;
	else if (a == 'S') Gv->currentPlayer = PLAYER_DR_SEWARD;
	else if (a == 'H') Gv->currentPlayer = PLAYER_VAN_HELSING;
	else if (a == 'M') Gv->currentPlayer = PLAYER_MINA_HARKER;
	else Gv->currentPlayer = PLAYER_DRACULA;
}

static void updateScore(GameView Gv, char *currPlay) {
	// TO-DO: WRITE THIS
	return;
}

static void processEncounterLocations(GameView Gv, char *currPlay) {
	// TO-DO: WRITE THIS
	return;
}

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
	 // Note that it will probably require malloc() \u2014 I've already
	 // added the corresponding free in GvNew()
	 return NULL;
} */
