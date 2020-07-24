
// Interface for the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#ifndef PLAYER_REP_H_
#define PLAYER_REP_H_

#define TRAIL_LENGTH 7
#define MAX_ROUNDS 366
#include "Places.h"
#include "Game.h"

typedef struct playerRep *PlayerRep;

PlayerRep PlayerRepNew(int health, char *trail[TRAIL_LENGTH], PlaceId location);

void PlayerRepFree(PlayerRep player);

int PlayerRepGetHealth(PlayerRep player);

PlaceId PlayerRepGetLocation(PlayerRep player);

void PlayerRepHealthUpdate(PlayerRep player, int health);

// Updates everything
void PlayerRepUpdate(PlayerRep player, char *currPlay);

// Update a player's trail
void PlayerRepUpdatePlayerTrail(PlayerRep player, const char *LocationAbb);

// Update a player's location
void PlayerRepUpdatePlayerLocation(PlayerRep player, PlaceId LocationID);

// Updata a player's move history
void PlayerRepUpdateMoveHistory(PlayerRep player, const char *LocationAbb);

// Update a player's HunterPOV trail
void PlayerRepUpdateHunterPOVTrail(PlayerRep player,
    const char *LocationAbb, PlaceType LocationType);

// Update a player's HunterPOV Move History
void PlayerRepUpdateHunterPOVMoveHistory (PlayerRep player,
    const char *LocationAbb, PlaceType LocationType);
#endif
