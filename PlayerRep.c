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
    int trail[TRAIL_LENGTH];
    int location;
};

PlayerRep PlayerRepNew(int health, int trail[TRAIL_LENGTH], int location)
{
    PlayerRep new = malloc(sizeof(struct playerRep));
    new->health = health;
    new->location = location;
    for(int i = 0; i < TRAIL_LENGTH; i++) {
        new->trail[i] = trail[i];
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

int PlayerRepGetLocation(PlayerRep player)
{
    return player->location;
}

// Update a player's trail AND current location
void PlayerRepUpdatePlayerTrail(PlayerRep player, char *currPlay)
{
    char *currentLocation[2];
    strncpy(currentLocation[2], currPlay + 1, 2);
    int LocationID = placeAbbrevToId(currentLocation[2]);
    for (int i = 6; i > 0; i--) 
    {
        player->trail[i] = player->trail[i-1];
    }
    player->trail[0] = LocationID;
    player->location = LocationID;
    return;
}
