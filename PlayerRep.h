// Interface for the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#ifndef PLAYER_REP_H_
#define PLAYER_REP_H_

#define TRAIL_LENGTH 7

typedef struct playerRep *PlayerRep;

PlayerRep PlayerRepNew(int health, char *trail[TRAIL_LENGTH], char *location);

void PlayerRepFree(PlayerRep player);

int PlayerRepGetHealth(PlayerRep player);

char *PlayerRepGetLocation(PlayerRep player);

// Update a player's trail AND current location
void PlayerRepUpdatePlayerTrail(PlayerRep player, char *currPlay);

#endif
