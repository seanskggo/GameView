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
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here

// Local #defines

#define MAX_PLAY_LENGTH 8 // Extra Space for Null Terminator

// TODO: ADD YOUR OWN STRUCTS HERE

// Linked list for storing history. NOTE: When storing history,
// the most recent history is added to the front of the list
typedef struct history {
	char *play;
	struct history *next;
} History;

// Character struct for storing character information
typedef struct character {
	int health;
	History *moves;
	// Queue trail;
} Character;

typedef struct location {
	int traps;
	bool vamp;
} Location;

// Gameview struct
struct gameView {
	// TODO: ADD FIELDS HERE
	int round;
	int score;
	Player current;
	Location *places;
	Character *player;
};

//------------------------- Local Functions ----------------------------

// Updates the gameview struct with the information from past plays
static void gameUpdate(GameView gv, char *plays);
static void updateScores(GameView gv, char *now);
static void convertPlay(GameView gv, char *currPlay);
static void updateHistory(GameView gv, Player player, char *currPlay);
static History *loop(History *ptr, int counter);
static Player updateCurrent(GameView gv);

//----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	new->round = 0; 
	new->score = GAME_START_SCORE;
	new->current = PLAYER_LORD_GODALMING;
	new->player = malloc(5 * sizeof(*new->player));
	new->places = malloc(NUM_REAL_PLACES * sizeof(*new->places));
	
	// initialise players
	for (int i = 0; i < 4; i++) {
		new->player[i].health = GAME_START_HUNTER_LIFE_POINTS;
		new->player[i].moves = NULL;
	}
	new->player[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS;
	new->player[PLAYER_DRACULA].moves = NULL;

	// Update game state
	gameUpdate(new, pastPlays);
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	for (int i = 0; i < 5; i++) {
		Character *tmp = &gv->player[i];
		free(tmp);
	}
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

Player GvGetPlayer(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return PLAYER_LORD_GODALMING;
}

int GvGetScore(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
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

// TODO

// Updates the status of game state
// This function is tested and works
static void gameUpdate(GameView gv, char *plays) {

	gv->current = PLAYER_LORD_GODALMING;
	Player test = updateCurrent(gv);
	printf("%d", test);
	
	updateHistory(GameView gv, Player player, char *currPlay);
	updateScores(GameView gv, char *currPlay);
	convertPlay(GameView gv, char *currPlay);

	// // Tokenise past plays to single plays
	// char currPlay[8];
	// int counter = 0;
	// for (int i = 0; plays[i] != '\0'; i++) {
	// 	if (plays[i] != ' ') {
	// 		currPlay[counter] = plays[i];
	// 		counter++;
	// 	} else {
	// 		currPlay[8] = '\0';
	// 		// Update current charater
			
	// 		convertPlay(gv, currPlay);
	// 		updateScores(gv, currPlay);
	// 		counter = 0;
	// 	}
	// }
}

// Update health and scores of players and game
static void updateScores(GameView gv, char *currPlay) {
	// Update history here

	char place[2];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	
	// Create ID number for location
	PlaceId location = placeAbbrevToId(place);
	assert(placeIsReal(location));
	if (currPlay[0] == 'D') {
		
		// If in sea, lose lifepoints
		if (placeIsSea(location)) 
			
			// Check if double back works
			gv->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
		
		// If in Castle Dracula, regain 10 lifepoints
		//else if ()
	}
	// Update history at the end?
}

// Converts Dracula's play. If the string contains special moves,
// this function modifies the string with the relevant place. 
// THIS FUNCTION ONLY CHANGES PLAY FOR DRACULA
// Tested and 100% works
static void convertPlay(GameView gv, char *currPlay) {
	char place[2];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	if (strcmp(place, "TP") == 0) {
		currPlay[1] = 'C';
		currPlay[2] = 'D';
	} else if (strcmp(place, "HI") == 0) {
		if (gv->player[PLAYER_DRACULA].moves == NULL) exit(EXIT_FAILURE);
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D1") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 1);
		char *tmp = ptr->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D2") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 2);
		char *tmp = ptr->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D3") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 3);
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D4") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 4);
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D5") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 5);
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} 
}

// Dont forget to free this memeory
// I don't know why pointer doesnt update the array here...
// This function is tested and works
static void updateHistory(GameView gv, Player player, char *currPlay) {
	History *new = malloc(sizeof(*new));
	new->play = currPlay;
	new->next = NULL;
	if (gv->player[player].moves == NULL) {
		gv->player[player].moves = new;
	} else {
		History *temp = gv->player[player].moves;
		gv->player[player].moves = new;
		new->next = temp;
	}
}

//Tested and works
static History *loop(History *ptr, int counter) {
	for (int i = 0; i < counter; i++) {
		if (ptr->next == NULL) {
			printf("Not enough history\n");
			exit(EXIT_FAILURE);
		}
		ptr = ptr->next;
	}
	return ptr;
}

static Player updateCurrent(GameView gv) {
	Player past = gv->current;
	if (past < PLAYER_DRACULA) return past++;
	else return PLAYER_LORD_GODALMING;
}

//-------------------------------------------------
//Test suite
/*
	//Test for update history

	char currPlay[8] = "DD2....";
	updateHistory(gv, PLAYER_DRACULA, "DGE...."); 
	updateHistory(gv, PLAYER_DRACULA, "DFE....");
	updateHistory(gv, PLAYER_DRACULA, "DPP....");
	convertPlay(gv, currPlay);
	printf("%s\n", currPlay);

*/