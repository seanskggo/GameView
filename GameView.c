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
#define MAX_TRAIL_LENGTH 6

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
	History *revealedMoves;
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
// Helper function for gameUpdate
static void helperGameUpdate(GameView gv, char *currPlay);
// Helper for convertPlay function
static void helperConvertPlay(GameView gv, char *currPlay, int counter);
// Updates the revealed version of history
static void updateRevealedHistory(GameView gv, Player player, char *currPlay);
// Helper function for updateScores
static void hunterUpdateScores(GameView gv, PlaceId location, char a);

// helper for the "reachable" type functions
static void convertPlay2(GameView gv, char *currMove, History *curr);
// Checks whether or not a given PlaceId refers to a DOUBLE_BACK move
static bool isDoubleBack(PlaceId p);


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
	// Extra slot for CITY_UNKNOWN
	new->places = malloc((NUM_REAL_PLACES + 1) * sizeof(*new->places));
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
		new->player[i].revealedMoves = NULL;
	}
	new->player[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS;
	new->player[PLAYER_DRACULA].moves = NULL;
	new->player[PLAYER_DRACULA].revealedMoves = NULL;

	// Update game state
	gameUpdate(new, pastPlays);
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// Free the characters
	for (int i = 0; i < 5; i++) {
	   // Free the history of players
	   History *curr = gv->player[i].moves;
	   History *prev = curr;
	   while (curr != NULL) {
	      curr = curr->next;
	      free(prev);
	      prev = curr;
	   }
	   // Free the revealed history of players
	   curr = gv->player[i].revealedMoves;
	   prev = curr;
	   while (curr != NULL) {
	      curr = curr->next;
	      free(prev);
	      prev = curr;
	   }
	   Character *tmp = &gv->player[i];
	   free(tmp);
	}
	// Free the location struct array
	for (int i = 0; i <= NUM_REAL_PLACES; i++) {
	   Location *tmp = &gv->places[i];
	   free(tmp);
	}
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
	strcpy(currPlay, gv->player[player].revealedMoves->play);
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
	if (gv->places[NUM_REAL_PLACES].vamp == true) return CITY_UNKNOWN;
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId *placesWTraps = malloc(MAX_TRAIL_LENGTH * sizeof(*placesWTraps));
	int j = 0;  // counter for number of locations found which contains traps
	int k = 0;  // counter for number of traps
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if ((!gv->places[i].vamp) && gv->places[i].traps > 0) {
			placesWTraps[j] = i;
	      	k = k + gv->places[i].traps;
	      	j++;
		}
	}
	// If city unknown has a trap
	if ((!gv->places[NUM_REAL_PLACES].vamp) &&
		gv->places[NUM_REAL_PLACES].traps > 0) {
		placesWTraps[j] = CITY_UNKNOWN;
		k = k + gv->places[NUM_REAL_PLACES].traps;
	}
	*numTraps = k;
	return placesWTraps;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// Count num of moves
	int total = 0;
	History *current = gv->player[player].moves;
	while (current != NULL) {
	    total++;
	    current = current->next;
	}
	// if there are 0 moves, it should return an empty array
	if (total == 0) {
	    PlaceId *MoveHistory = malloc(sizeof(*MoveHistory)*1);
	    MoveHistory = NULL;
	    *canFree = true;
	    *numReturnedMoves = total;
	    return MoveHistory;
	} else {
	    // create dynamically allocated array where there are more than 1 moves
	    PlaceId *MoveHistory = malloc((total)*sizeof(*MoveHistory));

	    History *curr = gv->player[player].moves;
	    for (int i = total - 1; i >= 0; i--) {
	        char currMove[3];
	        currMove[0] = curr->play[1];
	        currMove[1] = curr->play[2];
	        currMove[2] = '\0';
	        PlaceId currPlace = placeAbbrevToId(currMove);
	        MoveHistory[i] = currPlace;
	        curr = curr->next;
	    }
	    *numReturnedMoves = total;
	    *canFree = true;
	    return MoveHistory;
	}

}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	int total = 0;
	History *current = gv->player[player].moves;
	while (total < numMoves && current != NULL) {
	    total++;
	    current = current->next;
	}

	if (total == 0) {
	    PlaceId *MoveHistory = malloc(sizeof(*MoveHistory)*1);
	    MoveHistory = NULL;
	    *canFree = true;
	    *numReturnedMoves = total;
	    return MoveHistory;
	} else {
	    PlaceId *MoveHistory = malloc((total)*sizeof(*MoveHistory));

	    History *curr = gv->player[player].moves;
	    for (int i = total - 1; i >= 0; i--) {
	        char currMove[3];
	        currMove[0] = curr->play[1];
	        currMove[1] = curr->play[2];
	        currMove[2] = '\0';
	        PlaceId currPlace = placeAbbrevToId(currMove);
	        MoveHistory[i] = currPlace;
	        curr = curr->next;
	    }
	    *numReturnedMoves = total;
	    *canFree = true;
	    return MoveHistory;
	}
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	int total = 0;
	*numReturnedLocs = total;
	*canFree = true;
	History *current = gv->player[player].revealedMoves;
	while (current != NULL) {
	    total++;
	    current = current->next;
	}
    if (total == 0) {
        PlaceId *LocsHistory = malloc(sizeof(*LocsHistory)*1);
        LocsHistory = NULL;
        *canFree = true;
        *numReturnedLocs = total;
        return LocsHistory;
    } else {
        // create dynamically allocated array where there are more than 1 moves
        PlaceId *LocsHistory = malloc((total)*sizeof(*LocsHistory));
        History *curr = gv->player[player].revealedMoves;
        for (int i = total - 1; i >= 0; i--) {
            char currMove[3];
            currMove[0] = curr->play[1];
            currMove[1] = curr->play[2];
            currMove[2] = '\0';
            PlaceId currPlace = placeAbbrevToId(currMove);
            LocsHistory[i] = currPlace;
            curr = curr->next;
        }
        *numReturnedLocs = total;
        *canFree = true;
        return LocsHistory;
    }
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	int total = 0;
	*numReturnedLocs = total;
	*canFree = true;
	History *current = gv->player[player].revealedMoves;
	while (total < numLocs && current != NULL) {
	    total++;
	    current = current->next;
	}
    if (total == 0) {
        PlaceId *LocsHistory = malloc(sizeof(*LocsHistory)*1);
        LocsHistory = NULL;
        *canFree = true;
        *numReturnedLocs = total;
        return LocsHistory;
    } else {
        // create dynamically allocated array where there are more than 1 moves
        PlaceId *LocsHistory = malloc((total)*sizeof(*LocsHistory));
        History *curr = gv->player[player].revealedMoves;
        for (int i = total - 1; i >= 0; i--) {
            char currMove[3];
            currMove[0] = curr->play[1];
            currMove[1] = curr->play[2];
            currMove[2] = '\0';
            PlaceId currPlace = placeAbbrevToId(currMove);
            LocsHistory[i] = currPlace;
            curr = curr->next;
        }
        *numReturnedLocs = total;
        *canFree = true;
        return LocsHistory;
    }
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	PlaceId *reachableLocs = NULL;
	switch(player) {
		case PLAYER_LORD_GODALMING:
		case PLAYER_DR_SEWARD:
		case PLAYER_VAN_HELSING:
		case PLAYER_MINA_HARKER:

			reachableLocs = GvGetReachableByType(gv, player, round, from,
				true, true, true, numReturnedLocs);

			return reachableLocs;

			break;


		case PLAYER_DRACULA:
			// he can go anywhere nearby...unless a double back or hide
			// was done recetnly
			reachableLocs = GvGetReachableByType(gv, player, round, from,
				true, false, true, numReturnedLocs);
			return reachableLocs;
			break;

		default:
			break;

	}
	*numReturnedLocs = 0;

	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
    *numReturnedLocs = 0;
    ConnList reachableLocs = NULL;
	ConnList curr = MapGetConnections(gv->map, from);
	ConnList temp = curr;
	// we have an added array to check if we've already realised
	// we can access that location
	int *added = malloc(NUM_REAL_PLACES * sizeof(int));

	// initialise added
	for(int i = 0; i < NUM_REAL_PLACES; i++) {
		added[i] = -1;
	}

	switch(player) {
		case PLAYER_LORD_GODALMING:
		case PLAYER_DR_SEWARD:
		case PLAYER_VAN_HELSING:
		case PLAYER_MINA_HARKER:

			// hunters can always stay in the same city
			added[from] = 1;
			reachableLocs = connListInsert(reachableLocs, from, ROAD);
			*numReturnedLocs += 1;
			break;

		case PLAYER_DRACULA:

			assert(rail == false);

			History *curr2 = gv->player[PLAYER_DRACULA].moves;
			int numMoves = 0;
			bool foundDoubleBack = false;
			bool foundHide = false;
			char move[3];
			for(; curr2 != NULL && numMoves < 5; curr2 = curr2->next) {

				// we need to get the abreviation from the history
				move[0] = curr2->play[1];
				move[1] = curr2->play[2];
				move[2] = '\0';

				// then we check if it's a real location
				PlaceId moveId = placeAbbrevToId(move);
				if (placeIsReal(moveId) && added[moveId] != 1) {

					added[moveId] = 2;
					numMoves++;
				} else if (isDoubleBack(moveId)) {
					foundDoubleBack = true;
					numMoves++;
					convertPlay2(gv, move, curr2);
					PlaceId doubleBackLoc = placeAbbrevToId(move);
					added[doubleBackLoc] = 2;
				} else if (moveId == HIDE) {
					foundHide = true;
					convertPlay2(gv, move, curr2);
					PlaceId hideLoc = placeAbbrevToId(move);
					added[hideLoc] = 2;
					numMoves++;

				} else if (moveId == TELEPORT) {
					added[CASTLE_DRACULA] = 2;
					numMoves++;
				}
			}

			// deal with the "from"
			if (foundHide && foundDoubleBack) {
				added[from] = 1;
			} else {
				// we should be able to hide/doubleBack to current loc
				reachableLocs = connListInsert(reachableLocs, from, ROAD);
				*numReturnedLocs += 1;
			}

			// If we haven't double backed yet, we can actually
			// go to any location in our trail...
			if (foundDoubleBack == false) {
				for(int i = 0; i < NUM_REAL_PLACES; i++) {
					if (added[i] == 2) {
						added[i] = -1;
					}
				}
			}
			// else we have doublebacked, in which case we can't go back
			// to locations we have already visited (except for using a HIDE)
			else {
				for(int i = 0; i < NUM_REAL_PLACES; i++) {
					if (added[i] == 2) {
						added[i] = 1;
					}
				}
			}

			break;

		default:
			*numReturnedLocs = 0;

			return NULL;

			break;
	}

	// Get all locs reachable by boat/road (same for all players)

	if (road == true) {
		for(; curr != NULL; curr = curr->next) {
			if (curr->type == ROAD && added[curr->p] != 1) {
				reachableLocs = connListInsert(reachableLocs,
					curr->p, curr->type);
				added[curr->p] = 1;
				*numReturnedLocs += 1;
			}
		}
	}

	curr = temp;
	if (boat == true) {
		for(; curr != NULL; curr = curr->next) {
			if (curr->type == BOAT && added[curr->p] != 1) {
				reachableLocs = connListInsert(reachableLocs,
					curr->p, curr->type);
				added[curr->p] = 1;
				*numReturnedLocs += 1;
			}
		}
	}

	// this has to come after the previous two ifs...
	// MapGetRailReachable behaves badly otherwise

	if (rail == true) {
		int canTravelDist = round + player % 4;
		reachableLocs = MapGetRailReachable(gv->map, from,
			canTravelDist, reachableLocs, numReturnedLocs, added);
	}


	int rLocsLength = MapConnListLength(reachableLocs);
	PlaceId *reachableLocsArray = malloc(rLocsLength * sizeof(PlaceId));
	// populate the reachableLocsArray
	for (int i = 0; i < rLocsLength; i++) {
		reachableLocsArray[i] = reachableLocs->p;
		reachableLocs = reachableLocs->next;
	}

    free(added);
	return reachableLocsArray;
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
			helperGameUpdate(gv, currPlay);
			counter = 0;
		}
	}
	helperGameUpdate(gv, currPlay);
}

static void helperGameUpdate(GameView gv, char *currPlay) {
	currPlay[7] = '\0';
	// Update history of the immediate player with tokenised string
	updateHistory(gv, gv->current, currPlay);
	// Update Gamescores and encounter history.
	updateScores(gv, currPlay);
	// Update revealed history
	updateRevealedHistory(gv, gv->current, currPlay);
	// Update current player as the next in line
	gv->current = updateCurrent(gv);
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
	// If C?, make the location equal to CITY_UNKNOWN array index
	if (strcmp(place, "C?") == 0) location = NUM_REAL_PLACES;
	// Assert only if place is not C? or S?
	if (strcmp(place, "C?") != 0 && strcmp(place, "S?") != 0)
		assert(placeIsReal(location));

	if (gv->current == PLAYER_DRACULA) {
		// If in Castle Dracula, gain 10 lifepoints
		// Change depending on spec. Can castle dracula have traps? apparently yes
		if (strcmp(place, "TP") == 0 || location == CASTLE_DRACULA)
			gv->player[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA;

		// If in sea, lose lifepoints
		if (placeIsSea(location))
			gv->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
		// We assume that the vampire is on land since T is transcribed in the play
		else if (currPlay[3] == 'T')
			gv->places[location].traps++;
		else if (currPlay[4] == 'V') {
			gv->places[location].traps++;
			gv->places[location].vamp = true;
		}
		// If immature vampire matures, lose 13 points
		if (currPlay[5] == 'V') {
			gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
			for (int i = 0; i <= NUM_REAL_PLACES; i++) {
				if (gv->places[i].vamp == true) location = i;
			}
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
		hunterUpdateScores(gv, location, currPlay[3]);
		hunterUpdateScores(gv, location, currPlay[4]);
		hunterUpdateScores(gv, location, currPlay[5]);
		hunterUpdateScores(gv, location, currPlay[6]);
	}
}

// Helper function for updateScores. This function calculates hunter and
// and Dracula scores when there is an encounter as well as trap count
static void hunterUpdateScores(GameView gv, PlaceId location, char a) {
	if (a == 'T') {
			hunterEncounter(gv, 'T', location, gv->current);
		} else if (a == 'V') {
			hunterEncounter(gv, 'V', location, gv->current);
		} else if (a == 'D') {
			hunterEncounter(gv, 'D', location, gv->current);
		}
		if (gv->player[gv->current].health <= 0) {
			gv->player[gv->current].health = 0;
			gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
		}
		if (a == '.') return;
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
		helperConvertPlay(gv, currPlay, 1);
	} else if (strcmp(place, "D1") == 0) {
		helperConvertPlay(gv, currPlay, 1);
	} else if (strcmp(place, "D2") == 0) {
		helperConvertPlay(gv, currPlay, 2);
	} else if (strcmp(place, "D3") == 0) {
		helperConvertPlay(gv, currPlay, 3);
	} else if (strcmp(place, "D4") == 0) {
		helperConvertPlay(gv, currPlay, 4);
	} else if (strcmp(place, "D5") == 0) {
		helperConvertPlay(gv, currPlay, 5);
	}
}

static void helperConvertPlay(GameView gv, char *currPlay, int counter) {
	if (gv->player[PLAYER_DRACULA].moves == NULL) {
		printf("No recorded history of player %d\n", gv->current);
		exit(EXIT_FAILURE);
	}
	History *ptr = gv->player[PLAYER_DRACULA].revealedMoves;
	ptr = loop(ptr, counter);
	char *tmp = ptr->play;
	currPlay[1] = tmp[1];
	currPlay[2] = tmp[2];
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

// This function is tested and works
static void updateRevealedHistory(GameView gv, Player player, char *currPlay) {
	convertPlay(gv, currPlay);
	History *new = malloc(sizeof(*new));
	strcpy(new->play, currPlay);
	new->next = NULL;
	if (gv->player[player].revealedMoves == NULL) {
		gv->player[player].revealedMoves = new;
	} else {
		new->next = gv->player[player].revealedMoves;
		gv->player[player].revealedMoves = new;
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

// checks if a placeId is a doubleBack
static bool isDoubleBack(PlaceId p)
{
	return (p >= DOUBLE_BACK_1 && p <= DOUBLE_BACK_5) ? true : false;
}

// this version of convertPlay takes in a string with only two characters
// (not including Nul terminator), and a pointer to a history node.
// It is used by the Reachable functions.
static void convertPlay2(GameView gv, char *currMove, History *curr) {
	if (strcmp(currMove, "TP") == 0) {
		currMove[0] = 'C';
		currMove[1] = 'D';
	} else if (strcmp(currMove, "HI") == 0) {
		char *tmp = curr->next->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	} else if (strcmp(currMove, "D1") == 0) {
		char *tmp = curr->next->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	} else if (strcmp(currMove, "D2") == 0) {
		History *ptr = curr;
		ptr = loop(ptr, 2);
		char *tmp = ptr->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	} else if (strcmp(currMove, "D3") == 0) {
		History *ptr = curr;
		ptr = loop(ptr, 3);
		char *tmp = ptr->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	} else if (strcmp(currMove, "D4") == 0) {
		History *ptr = curr;
		ptr = loop(ptr, 4);
		char *tmp = ptr->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	} else if (strcmp(currMove, "D5") == 0) {
		History *ptr = curr;
		ptr = loop(ptr, 5);
		char *tmp = ptr->play;
		currMove[0] = tmp[1];
		currMove[1] = tmp[2];
	}
}

//-------------------------------------------------
/*

// Neglected functions
static bool isSpecial(char *tmp) {
	if (tmp[1] == 'H' && tmp[2] == 'I') return true;
	else if (tmp[1] == 'D' && tmp[2] == '1') return true;
	else if (tmp[1] == 'D' && tmp[2] == '2') return true;
	else if (tmp[1] == 'D' && tmp[2] == '3') return true;
	else if (tmp[1] == 'D' && tmp[2] == '4') return true;
	else if (tmp[1] == 'D' && tmp[2] == '5') return true;
	else return false;
}

//Test suite

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
