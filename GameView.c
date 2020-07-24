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

struct locationEncounters {
	PlaceId location;
	int encounters;
	int roundsTilRemoval;
};

// #defines here
#define NUM_HUNTERS 		4
#define PLAY_LENGTH 		8

#define ONE_TRAP			1
#define TWO_TRAPS			2
#define IM_VAMP				3
#define TRAP_IM_VAMP		4
#define TWO_TRAPS_IM_VAMP	5
#define THREE_TRAPS			6

// Local Function Declarations
static void calcGameState(GameView Gv, char *pastPlays);
static void initialisePlayers(GameView Gv);
static void calcCurrPlayer(GameView Gv, char *currPlay);
static void updateScore(GameView Gv, char *currPlay);
static void updateStatus(GameView Gv, char *currPlay);

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
	// ourselves — see Map.c

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

	return gv->currentPlayer;
}

int GvGetScore(GameView gv)
{
	// Input gv
	// Output integer value of the current score

	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	// Input gv, player
	// Output integer value of the current player's health

	// Player's health must not exceed 9 (0 to 9 inclusive)
	// Dracula's health >= 0

	return PlayerRepGetHealth(gv->players[player]);
}

// sam working on this — incomplete

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
/*
	// Input gv, player
	// Output the integer value assigned to the location (placeID)
		// of where the player is currently at. Return "NOWHERE" if the player
		// selected has not made a turn yet (before their first turn). Return
		// PlaceID if it is a hunter. Return PlaceID if it is Dracula AND his
		// current location is revealed in the play string. Return CITY_UNKNOWN
		// or SEA_UNKNOWN if player is dracula and has not been revealed.

		// NOTE: VERY MUCH INCOMPLETE — see GameView.h

		switch(Gv->currentPlayer) {
			Case PLAYER_LORD_GODALMING:
			Case PLAYER_DR_SEWARD:
			Case PLAYER_VAN_HELSING:
			Case PLAYER_MINA_HARKER:
				PlayerRepGetLocation(gv->players[player])
				break;

			// Drac
			default:
				// if: Drac's current location is revealed to the hunter's
					// print the loc, unless it it a hide or double_back

					// if it is a hide or double_back, we check which move
					// it refers to, then print that (two if statements)

				// else if:


				// else: Dracula is NOWHERE
				break;
		}
*/
	return PlayerRepGetLocation(gv->players[player]);

}


// sam working on this

PlaceId GvGetVampireLocation(GameView gv)
{
/*
	// Input gv
	// Output the integer value assigned to the location (placeID)
		// of where the immature vampire are currently at in a dynamically
		// allocated array. order does not matter. Return PlaceId if
		// location has been revealed. Return CITY_UNKNOWN if location
		// has not been revealed. Return NOWHERE if no vampires exist

	// Traverse the map in gv and copy the placeID in the dynamically allocated
		// array.


	// NOTE: NOT A COMPLETE FUNCTION — see GameView.h

	// NOTE: could be made more efficient by storing the vamp's
	// location?

	switch(Gv->currentPlayer) {
		// if the round is in a certain mod range...there can NEVER
		// be an immature vamp...

		// if: (above but in code)
			// return NOWHERE;

		Case PLAYER_LORD_GODALMING:
		Case PLAYER_DR_SEWARD:
		Case PLAYER_VAN_HELSING:
		Case PLAYER_MINA_HARKER:
			// code here

			// for: find immature vamp location
			PlaceID vampLoc = NOWHERE;
			for(int i = 0; i < NUM_REAL_PLACES; i++) {
				if (gv->encounterLocations[i] == IM_VAMP ||
					gv->encounterLocations[i] == TRAP_IM_VAMP ||
					gv->encounterLocations[i] == TWO_TRAPS_IM_VAMP
				) {
					vampLoc = gv->encounterLocations[i];
				}
			}

			// for: loop through the move history to see if it's been revealed?
			// go to the round where the vamp was created in the
			// hunter POV for drac's trail.

				// If: it's a location, check if it's revealed

				// else if: it's a double_back, deal with that

				// else if: it's a hide, go back one move/round?


			break;

		// Drac
		default:
			// I think this is for Dracula
			// NOTE: repeated code...chuck in a function?
			for(int i = 0; i < NUM_REAL_PLACES; i++) {
				if (gv->encounterLocations[i] == IM_VAMP ||
					gv->encounterLocations[i] == TRAP_IM_VAMP ||
					gv->encounterLocations[i] == TWO_TRAPS_IM_VAMP
				) {
					return gv->encounterLocations[i];
				}
			}
			break;
	}
*/
	return NOWHERE;
}

// Sam working on THIS

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
/*
	// Input gv, numTraps
	// Output the location of the traps currently on the map in a
		// dynamically allocated array. Including multiple copies if multiple
		// traps in a single location and order does not matter.

	// Traverse the map in gv and copy the placeID in the dynamically allocated
		// array.


	// If player is not Dracula, just return. May add additional functionality
	// for players later, but the spec doesn't require it.

	if (Gv->currentPlayer != PLAYER_DRACULA) {
		return NOWHERE;
	}

	// could maybe be made more efficient...but it might cost elsewhere??

	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		if (gv->encounterLocations[i] == ONE_TRAP
			gv->encounterLocations[i] == TRAP_IM_VAMP
		) {
			*numTraps++;
		}

		else if (gv->encounterLocations[i] == TWO_TRAPS ||
			gvgv->encounterLocations[i] == TWO_TRAPS_IM_VAMP
		) {
			*numTraps += 2;
		}

		else if (gv->encounterLocations[i] == THREE_TRAPS) {
			*numTraps += 3;
		}
	}
*/
	// *numTraps = 0;
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

// Sam working on THIS

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
/*
	switch(Gv->currentPlayer) {
		Case PLAYER_LORD_GODALMING:
		Case PLAYER_DR_SEWARD:
		Case PLAYER_VAN_HELSING:
		Case PLAYER_MINA_HARKER:

			ConnList curr = MapGetConnections(gv->map, from);
			ConnList reachableLocs = NULL;
			for(; curr != NULL; curr = curr->next) {

				if (curr->type == ROAD ||
					curr->type == BOAT
				) {
					*numReturnedLocs++;
					reachableLocs = connListInsert(reachableLocs,
						curr->p, curr->type
					);
				}

				else if (curr->type == RAIL) {
					int canTravelDist = gv->round + gv->currentPlayer % 4;
					// need to check if curr is within canTravelDist
					// from "from"
					// we'll see if it needs to be < or <=...
					if (MapGetRAILDistance(gv->m, from, curr->p) <=
						canTravelDist
					) {
						*numReturnedLocs++;
						reachableLocs = connListInsert(reachableLocs,
							curr->p, curr->type
						);
					}

				}

			}
			// declare an array of PlaceIDs with size of *numReturnedLocs
			// will be malloced
			int rLocsLength = MapConnListLength(reachableLocs);

			PlaceId reachableLocsArray = malloc(rLocsLength * sizeof(PlaceID));

			// populate the reachableLocsArray
			ConnList curr = reachableLocs
			for (int i = 0; i < rLocsLength; i++) {
				reachableLocsArray[i] = curr->p;
				curr = curr->next;
			}

			return reachableLocsArray;

			//break;

		// Drac
		default:
			// TO-DO: write this part
			break;
	}

	*numReturnedLocs = 0;
	*/
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION

	// NOTE: USE ABOVE FUNCTION TO HELP?...OR...USE THIS Function
	// TO HELP THE ABOVE ONE?
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

////////////////////////////////////////////////////////////////////////
// Local Helper Functions

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

	// Set initial game score to 366
	Gv->score = GAME_START_SCORE;

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

			// Update encounter location and health profiles
			updateStatus(Gv, currPlay);

			// Update score based on the current play
			updateScore(Gv, currPlay);

			// Update current player based on the current play
			calcCurrPlayer(Gv, currPlay);

			// Update current player's trail
			PlayerRepUpdatePlayerTrail(Gv->players[Gv->currentPlayer],
				currPlay
			);


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

// Modified by Dabin
// Calculates and updates the health of all characters during this
// turn depending on the encounter locations etc.
// Assume the current play is not restricted
static void updateStatus(GameView Gv, char *currPlay) {
	// TO-DO: WRITE THIS
	if (currPlay[0] == 'D') {
		// Create location string
		char a[2];
		a[0] = currPlay[1];
		a[1] = currPlay[2];
		PlaceId temp = placeAbbrevToId(a);
		assert(!placeIsReal(temp));
		//
		if (placeIsSea(temp) == true)
			PlayerRepHealthUpdate(Gv->players[PLAYER_DRACULA], - LIFE_LOSS_SEA);
		else if (placeIsLand(temp) == true) {
			if(Gv->round % 13 == 0)
				Gv->encounterLocations[temp] = TRAP_IM_VAMP;
		}
	}
}

// Modified by Sean
// Calculate the score based on CurrPlay and Gv_>score
static void updateScore(GameView Gv, char *currPlay) {
	// TO-DO: WRITE THIS

	// If Dracula's turn
	if (currPlay[0] == 'D') {
		if (currPlay[4] == 'V') {
			Gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
		} else Gv->score -= SCORE_LOSS_DRACULA_TURN;
	// If hunter's turn
	} else {
		// If the player health below zero on the player's turn
		if (PlayerRepGetHealth(Gv->players[Gv->currentPlayer]) <= 0)
			Gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
	}
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
	 // Note that it will probably require malloc() — I've already
	 // added the corresponding free in GvNew()
	 return NULL;
} */
