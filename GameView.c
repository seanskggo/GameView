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
	char play[8];
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
// Updates the scores of the Gameview e.g. health and scores of players 
// as well as the locations of traps
static void updateScores(GameView gv, char *now);
// Converts special moves. E.g. if double back (D1) is played, this function
// checks relevant history to determine the place and thus modifies the
// input string accordingly
static void convertPlay(GameView gv, char *currPlay);
// Updates the history of moves for the specified player
static void updateHistory(GameView gv, Player player, char *currPlay);
// Helper function for updateHistory
static History *loop(History *ptr, int counter);
// Update the current player i.e. the next player after the past play
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

	// Tokenise past plays to single plays
	char currPlay[8];
	int counter = 0;
	for (int i = 0; plays[i] != '\0'; i++) {
		if (plays[i] != ' ') {
			currPlay[counter] = plays[i];
			counter++;
		} else {
			currPlay[8] = '\0';
			// Update current charater
			convertPlay(gv, currPlay);
			// Update history of the immediate player with tokenised string
			updateHistory(gv, gv->current, currPlay);
			// Update Gamescores and encounter history.
			updateScores(gv, currPlay);
			// Update current player as the next in line
			gv->current = updateCurrent(gv);
			counter = 0;
		}
	}
}

// Currently still under develpment. This function updates the location of traps
static void updateScores(GameView gv, char *currPlay) {
	
	// Create ID number for location
	char place[2];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	PlaceId location = placeAbbrevToId(place);
	assert(placeIsReal(location));

	if (gv->current == PLAYER_DRACULA) {
		// If in sea, lose lifepoints
		if (placeIsSea(location)) 
			gv->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
		// If in Castle Dracula, gain 10 lifepoints
		else if (strcmp(place, "TP") == 0 || location == CASTLE_DRACULA) 
			gv->player[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA;
		// We assume that the vampire is on land since T is transcribed in the play
		else if (currPlay[3] == 'T')
			gv->places[location].traps++;
		else if (currPlay[4] == 'V') {
			gv->places[location].traps++;
			gv->places[location].vamp = true;
		// If immature vampire matures, lose 13 points	
		} else if (currPlay[5] == 'V') {
			gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
			gv->places[location].vamp = false;
			gv->places[location].traps--;
		// If a trap leaves the trail, adjust trap count
		// This scenario is not understood perfectly at this stage
		// Waiting for answers from jaz
		} else if (currPlay[5] == 'M') {
			// Loop to the last in trail and find the location 
			//gv->places[location].traps--;
		}
		// Decrease score
		gv->score--;
		// Increase round
		gv->round++;
	// For Hunters
	} else {

	}
}

// Converts Dracula's play. If the string contains special moves,
// this function modifies the string with the relevant place. 
// THIS FUNCTION ONLY CHANGES PLAY FOR DRACULA
// Tested and 100% works
static void convertPlay(GameView gv, char *currPlay) {
	if (gv->current != PLAYER_DRACULA) return;
	char place[3];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	place[2] = '\0';
	if (strcmp(place, "TP") == 0) {
		currPlay[1] = 'C';
		currPlay[2] = 'D';
	} else if (strcmp(place, "HI") == 0) {
		if (gv->player[PLAYER_DRACULA].moves == NULL) {
			printf("No recorded history of player %d\n", gv->current);
			exit(EXIT_FAILURE);
		}
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D1") == 0) {
		if (gv->player[PLAYER_DRACULA].moves == NULL) {
				printf("No recorded history of player %d\n", gv->current);
			exit(EXIT_FAILURE);
		}
		char *tmp = gv->player[PLAYER_DRACULA].moves->play;
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
		char *tmp = ptr->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D4") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 4);
		char *tmp = ptr->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} else if (strcmp(place, "D5") == 0) {
		History *ptr = gv->player[PLAYER_DRACULA].moves;
		ptr = loop(ptr, 5);
		char *tmp = ptr->play;
		currPlay[1] = tmp[1];
		currPlay[2] = tmp[2];
	} 
}

// Dont forget to free this memeory
// I don't know why pointer doesnt update the array here...
// This function is tested and works
static void updateHistory(GameView gv, Player player, char *currPlay) {
	History *new = malloc(sizeof(*new));
	strcpy(new->play, currPlay);
	new->next = NULL;
	if (gv->player[player].moves == NULL) {
		gv->player[player].moves = new;
	} else {
		new->next = gv->player[player].moves;
		gv->player[player].moves = new;
	}
}

//Tested and works
static History *loop(History *ptr, int counter) {
	for (int i = 0; i < counter - 1; i++) {
		if (ptr->next == NULL) {
			printf("Not enough history\n");
			exit(EXIT_FAILURE);
		}
		ptr = ptr->next;
	}
	return ptr;
}

//Tested and works
static Player updateCurrent(GameView gv) {
	Player past = gv->current;
	if (past < PLAYER_DRACULA) return past + 1;
	else return PLAYER_LORD_GODALMING;
}

//-------------------------------------------------
//Test suite
/*
	//Test for update history


	//Test for updatecurrent

	gv->current = PLAYER_VAN_HELSING;
	Player test = updateCurrent(gv);
	printf("%d\n", test);
	
	// Place holders
	updateHistory(gv, PLAYER_DRACULA, "test");
	updateScores(gv, "test");
	convertPlay(gv, "test");

	char currPlay[8] = "DD2....";
	updateHistory(gv, PLAYER_DRACULA, "DGE...."); 
	updateHistory(gv, PLAYER_DRACULA, "DFE....");
	updateHistory(gv, PLAYER_DRACULA, "DPP....");
	convertPlay(gv, currPlay);
	printf("%s\n", currPlay);

	History *ptr = gv->player[PLAYER_MINA_HARKER].moves;
	while (ptr != NULL) {
		printf("%s\n", ptr->play);
		ptr = ptr->next;
	}

	SUPER BUG test
			currPlay[8] = '\0';
			// Update current charater
			printf("*** %d ***\n", gv->current);
			printf("before: %s\n", currPlay);
			convertPlay(gv, currPlay);
			printf("after: %s\n", currPlay);
			// Update history of the immediate player with tokenised string
			updateHistory(gv, gv->current, currPlay);
				History *ptr = gv->player[gv->current].moves;
				printf("In memory: ");
				while (ptr != NULL) {
					printf("%s ", ptr->play);
					ptr = ptr->next;
				}
				printf("\n");
			// Update Gamescores
			updateScores(gv, currPlay);
			// Update current player as the next in line
			gv->current = updateCurrent(gv);
			counter = 0;
			printf("***********\n");
*/