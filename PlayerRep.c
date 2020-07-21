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
    char *location;
};

PlayerRep PlayerRepNew(int health, char *location)
{
    PlayerRep new = malloc(sizeof(struct playerRep));
    new->health = health;
    new->location = strdup(location);
    return new;
};

void PlayerRepFree(PlayerRep player)
{
    free(player->location);
    free(player);
    return;
}

int getPlayerHealth(PlayerRep player)
{
    return player->health;
}

char *getPlayerLocation(PlayerRep player)
{
    return player->location;
}
