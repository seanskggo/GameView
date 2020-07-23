// Implementation of the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PlayerRep.h"
#include "Places.h"

struct playerRep
{
    int health;
    PlaceId MoveHistory[MAX_ROUNDS];
    PlaceId trail[TRAIL_LENGTH];
    PlaceId location;
};

PlayerRep PlayerRepNew(int health, PlaceId trail[TRAIL_LENGTH], PlaceId location)
{
    PlayerRep new = malloc(sizeof(struct playerRep));
    new->health = health;
    new->location = location;
    for(int i = 0; i < TRAIL_LENGTH; i++) 
    {
        new->trail[i] = trail[i];
    }
    // initialise all elements of move history to be NOWHERE
    // Note: a player is physically incapable of being NOWHERE
    for (int i = 0; i < MAX_ROUNDS; i++) 
    {
        new->MoveHistory[i] = NOWHERE;
    }
    return new;
}

void PlayerRepFree(PlayerRep player)
{
    free(player);
    return;
}

int PlayerRepGetHealth(PlayerRep player)
{
    return player->health;
}

PlaceId PlayerRepGetLocation(PlayerRep player)
{
    return player->location;
}

// Update a player's trail AND current location
void PlayerRepUpdatePlayerTrail(PlayerRep player, char *currPlay)
{
    char *currentLocation[2];
    strncpy(currentLocation[2], currPlay + 1, 2);
    PlaceId LocationID = placeAbbrevToId(currentLocation[2]);
    for (int i = 6; i > 0; i--) 
    {
        player->trail[i] = player->trail[i-1];
    }
    player->trail[0] = LocationID;
    player->location = LocationID;
    return;
}

// Updates a player's move history
void PlayerRepUpdateMoveHistory(PlayerRep player, char *currPlay)
{
    char *currentLocation[2];
    strncpy(currentLocation[2], currPlay + 1, 2);
    PlaceId LocationID = placeAbbrevToId(currentLocation[2]);
    int i = 0;
    for (int i = 0; player->MoveHistory[i] != NOWHERE && i < MAX_ROUNDS; i++)
    {
    }
    player->MoveHistory[i] = LocationID;
}
