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
	Map map;
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
// Helper function for updateScores. Used for calculating hunter encounters
static void hunterEncounter(GameView gv, char a, PlaceId location, Player name);

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
	new->map = MapNew();

	//intialise places don't know if this is neccessary
	// for (int i = 0; i < NUM_REAL_PLACES; i++) {
	// 	new->places[i].traps = 0;
	// 	new->places[i].vamp = false;
	// }
	
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
	// for (int i = 0; i < 5; i++) {
	// 	Character *tmp = &gv->player[i];
	// 	free(tmp);
	// }
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->current;
}

int GvGetScore(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->player[player].health;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (gv->player[player].moves == NULL) return NOWHERE;
	char currPlay[8];
	strcpy(currPlay, gv->player[player].moves->play);
	// Temporarily convert current player to function convertPaly
	Player temp = gv->current;
	gv->current = player;
	convertPlay(gv, currPlay);
	gv->current = temp;
	char where[3];
	where[0] = currPlay[1];
	where[1] = currPlay[2];
	where[2] = '\0';
	return placeAbbrevToId(where);
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if (gv->places[i].vamp == true) return i;
	}
	return NOWHERE;

	// make the city unknown with an extra slot at the end. 
	// when adding vamp, if it is city unkonwn then add to last slot
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId *placesWTraps = malloc(NUM_REAL_PLACES * sizeof(*placesWTraps));
	int j = 0;  // counter for number of locations found which contains traps
	int k = 0;  // counter for number of traps
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
	   if (gv->places[i].traps > 0) {
	      k++;
	      placesWTraps[j] = i;
	      j++;
		}
	}
	*numTraps = k;
	return placesWTraps;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION

	// Hey Francis, so if you give the below function a current play, it
	// will convert to the real location by looking at the player's history

	// Update current charater. Currently currPlay is unintilaised
	char currPlay[8];
	convertPlay(gv, currPlay);

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

// Explanation of this function: First, the past play is tokenised.
// The tokenised, unmodified play is processed. Then the unmodified history
// is also added to the history of the player

// Updates the status of game state
// This function is tested and works 100%
static void gameUpdate(GameView gv, char *plays) {
	
	if (strcmp(plays, "") == 0) return;
	// Tokenise past plays to single plays
	char currPlay[8];
	int counter = 0;
	for (int i = 0; plays[i] != '\0'; i++) {
		if (plays[i] != ' ') {
			currPlay[counter] = plays[i];
			counter++;
		} else {
			currPlay[7] = '\0';
			char tempor[8];
			strcpy(tempor, currPlay);
			// Update Gamescores and encounter history.
			updateScores(gv, currPlay);
			// Update history of the immediate player with tokenised string
			updateHistory(gv, gv->current, tempor);
			// Update current player as the next in line
			gv->current = updateCurrent(gv);
			counter = 0;
		}
	}
	currPlay[7] = '\0';
	char tempor[8];
	strcpy(tempor, currPlay);
	// Update Gamescores and encounter history.
	updateScores(gv, currPlay);
	// Update history of the immediate player with tokenised string
	updateHistory(gv, gv->current, tempor);
	// Update current player as the next in line
	gv->current = updateCurrent(gv);
	counter = 0;
}

// Currently still under develpment. This function updates the location of traps
static void updateScores(GameView gv, char *currPlay) {
	
	// If player health is >= 0 at the start of turn, set back to max health
	if (gv->player[gv->current].health == 0) 
		gv->player[gv->current].health = GAME_START_HUNTER_LIFE_POINTS;
	
	// Convert Dracula special moves with specified location
	if (gv->current == PLAYER_DRACULA) convertPlay(gv, currPlay);
	
	// Create ID number for location
	char place[3];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	place[2] = '\0';
	PlaceId location = placeAbbrevToId(place);
	// Assert only if place is not C? or S?
	if (strcmp(place, "C?") != 0 && strcmp(place, "S?") != 0)
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
		} 
		if (currPlay[5] == 'V') {
			gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
			gv->places[location].vamp = false;
			gv->places[location].traps--;
		// If a trap leaves the trail, adjust trap count
		} else if (currPlay[5] == 'M') {
			History *ptr = gv->player[gv->current].moves;
			for (int i = 0; i < 6; i++) {
				ptr = ptr->next;
			}
			char *tmp = ptr->play;
			char where[3];
			where[0] = tmp[1];
			where[1] = tmp[2];
			where[2] = '\0';
			PlaceId name = placeAbbrevToId(where);
			gv->places[name].traps--;
		}
		// Decrease score
		gv->score--;
		// Increase round
		gv->round++;
	// For Hunters
	} else {
		// If trap is encountered, minus life points. If hunter life is 0 or
		// below,  
		if (currPlay[3] == 'T') {
			hunterEncounter(gv, 'T', location, gv->current);
		} else if (currPlay[3] == 'V') {
			hunterEncounter(gv, 'V', location, gv->current);
		} else if (currPlay[3] == 'D') {
			hunterEncounter(gv, 'D', location, gv->current);
		} 
		
		// Terminates the calculation of hunter once the health is depleted
		if (gv->player[gv->current].health <= 0) {
			gv->player[gv->current].health = 0;
			gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
			return;
		}

		if (currPlay[3] == '.') return;
		
		if (currPlay[4] == 'T') {
			hunterEncounter(gv, 'T', location, gv->current);
		} else if (currPlay[4] == 'V') {
			hunterEncounter(gv, 'V', location, gv->current);
		} else if (currPlay[4] == 'D') {
			hunterEncounter(gv, 'D', location, gv->current);
		} 
		
		if (gv->player[gv->current].health <= 0) {
			gv->player[gv->current].health = 0;
			gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
			return;
		}

		if (currPlay[4] == '.') return;

		if (currPlay[5] == 'T') {
			hunterEncounter(gv, 'T', location, gv->current);
		} else if (currPlay[5] == 'V') {
			hunterEncounter(gv, 'V', location, gv->current);
		} else if (currPlay[5] == 'D') {
			hunterEncounter(gv, 'D', location, gv->current);
		} 
		
		if (gv->player[gv->current].health <= 0) {
			gv->player[gv->current].health = 0;
			gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
			return;
		}

		if (currPlay[5] == '.') return;

		if (currPlay[6] == 'T') {
			hunterEncounter(gv, 'T', location, gv->current);
		} else if (currPlay[6] == 'V') {
			hunterEncounter(gv, 'V', location, gv->current);
		} else if (currPlay[6] == 'D') {
			hunterEncounter(gv, 'D', location, gv->current);
		} 

		if (gv->player[gv->current].health <= 0) {
			gv->player[gv->current].health = 0;
			gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
		}

		if (currPlay[6] == '.') return;
	}
}

// Helper function for hunter trap encounters
// TESTED AND WORKS 100%
static void hunterEncounter(GameView gv, char a, PlaceId location, Player name) {
	if (a == 'T') {
		gv->player[name].health -= LIFE_LOSS_TRAP_ENCOUNTER;
		gv->places[location].traps--;
	} else if (a == 'V') {
		gv->places[location].traps--;
		gv->places[location].vamp = false;
	} else if (a == 'D') {
		// Hunter encounter Dracula
		gv->player[name].health -= LIFE_LOSS_DRACULA_ENCOUNTER;
		gv->player[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;
		if (gv->player[PLAYER_DRACULA].health <= 0) {
			printf("Dracula has been vanquished\n");
			exit(EXIT_SUCCESS);
		}
	}
}

// Converts Dracula's play. If the string contains special moves,
// this function modifies the string with the relevant place. 
// THIS FUNCTION ONLY CHANGES PLAY FOR DRACULA
// Tested and Works 100%
static void convertPlay(GameView gv, char *currPlay) {
	char place[3];
	place[0] = currPlay[1];
	place[1] = currPlay[2];
	place[2] = '\0';
	// If player health is zero, convert the string with the hospital
	if (gv->current != PLAYER_DRACULA) { 
		if (gv->player[gv->current].health == 0) {
			currPlay[1] = 'J';
			currPlay[2] = 'M';
		}
	}

	// Else, the following applies to Dracula conversion
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

	UpdateScore Tests!
	// testing
	printf("*** TESTING ***\n");

	gv->current = PLAYER_LORD_GODALMING;
	updateHistory(gv, PLAYER_LORD_GODALMING, "GAMTTTD");
	updateScores(gv, "GAMTTTD");

	printf("%d\n", gv->places[AMSTERDAM].traps);
	printf("%d\n", gv->player[PLAYER_LORD_GODALMING].health);
	printf("*** END TEST ***\n");
	char currPlay[8] = "GAMTTTD";
	convertPlay(gv, currPlay);
	printf("%s\n", currPlay);

	gv->current = PLAYER_LORD_GODALMING;
	updateHistory(gv, PLAYER_LORD_GODALMING, "GAM....");
	updateScores(gv, "GAMVVVV");
	printf("%d\n", gv->places[AMSTERDAM].traps);
	printf("%d\n", gv->score);
	printf("%d\n", gv->player[PLAYER_LORD_GODALMING].health);
*/
