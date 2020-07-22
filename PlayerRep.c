// Implementation of the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PlayerRep.h"

struct playerRep
{
    int health;
    char *trail[TRAIL_LENGTH];
    char *location;
};

PlayerRep PlayerRepNew(int health, char *trail[TRAIL_LENGTH], char *location)
{
    PlayerRep new = malloc(sizeof(struct playerRep));
    new->health = health;
    new->location = strdup(location);
    for(int i = 0; i < TRAIL_LENGTH; i++) {
        new->trail[i] = strdup(trail[i]);
    }
    return new;
}

void PlayerRepFree(PlayerRep player)
{
    free(player->location);
    free(player);
    return;
}

int PlayerRepGetHealth(PlayerRep player)
{
    return player->health;
}

char *PlayerRepGetLocation(PlayerRep player)
{
    return player->location;
}

// Update a player's trail AND current location
void PlayerRepUpdatePlayerTrail(PlayerRep player, char *currPlay)
{

    // TO-DO: write this
    return;
}
