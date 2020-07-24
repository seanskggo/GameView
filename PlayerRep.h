// Interface for the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#ifndef PLAYER_REP_H_
#define PLAYER_REP_H_

#define TRAIL_LENGTH 6
#define MAX_ROUNDS 366

#include "Places.h"
#include "Game.h"

typedef struct playerRep *PlayerRep;

PlayerRep PlayerRepNew(int health, char *trail[TRAIL_LENGTH], PlaceId location);

void PlayerRepFree(PlayerRep player);

int PlayerRepGetHealth(PlayerRep player);

PlaceId PlayerRepGetLocation(PlayerRep player);

// Updates everything
void PlayerRepUpdate(PlayerRep player, char *currPlay, int RoundNumber);

// Update a player's trail
void PlayerRepUpdatePlayerTrail(PlayerRep player, const char *LocationAbb);

// Update a player's location
void PlayerRepUpdatePlayerLocation(PlayerRep player, PlaceId LocationID);

// Updata a player's move history
void PlayerRepUpdateMoveHistory(PlayerRep player, const char *LocationAbb,
    int RoundNumber);

// Update a player's HunterPOV trail
void PlayerRepUpdateHunterPOVTrail(PlayerRep player, 
    const char *LocationAbb, PlaceType LocationType);

// Update a player's HunterPOV Move History
void PlayerRepUpdateHunterPOVMoveHistory (PlayerRep player, 
    const char *LocationAbb, PlaceType LocationType, int RoundNumber); 

// Check if in Dracula's Trail
PlaceId PlayerRepCheckTrail(PlayerRep player, const char *LocationAbb,
    int roundNumber);
    
// Reveal Dracula's trail and move history
void PlayerRepRevealDracula(PlayerRep player, int TrailNumber, 
    int RoundNumber);

// Makes an array with entire move History
char **PlayerRepGameMoveHistory(PlayerRep player1, PlayerRep player2, 
    PlayerRep player3, PlayerRep player4, PlayerRep player5, int Turns);

// Get a player's HunterPOV MoveHistory
char **PlayerRepMoveHistory(PlayerRep player);

// Get a player's HunterPOV trail
char **PlayerRepGetTrail(PlayerRep player);

// Checks if player is in same location as Dracula
int PlayerRepSameLocation(PlayerRep player1, PlayerRep Dracula);
#endif
