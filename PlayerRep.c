// Implementation of the PlayerRep ADT.
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PlayerRep.h"
#include "Places.h"
#include "Game.h"

struct playerRep
{
    int health;
    PlaceId location;
    char *MoveHistory[MAX_ROUNDS];
    char *trail[TRAIL_LENGTH];
    
    char *HunterPOVTrail[TRAIL_LENGTH];
    char *HunterPOVMoveHistory[MAX_ROUNDS];
};

PlayerRep PlayerRepNew(int health, char *trail[TRAIL_LENGTH], PlaceId location)
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
        new->MoveHistory[i] = "??";
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

// Updates everything
void PlayerRepUpdate(PlayerRep player, char *currPlay)
{
    char currentLocation[2];
    strncpy(currentLocation, currPlay + 1, 2);
    PlaceId LocationID = placeAbbrevToId(currentLocation);    
    const char *LocationAbb = placeIdToAbbrev(LocationID);
    PlaceType LocationType = placeIdToType(LocationID);
    
    // Update Trail
    PlayerRepUpdatePlayerTrail(player, currentLocation);
    // Update Location
    PlayerRepUpdatePlayerLocation(player, LocationID);
    // Update Move History
    PlayerRepUpdateMoveHistory(player, LocationAbb);
    // Update HunterPOV trail
    PlayerRepUpdateHunterPOVTrail(player, LocationAbb, LocationType);
    // Update HunterPOV move history
    PlayerRepUpdateHunterPOVMoveHistory(player, LocationAbb, LocationType); 
    return;
}

// Update a player's trail
void PlayerRepUpdatePlayerTrail(PlayerRep player, char *currentLocation)
{
    for (int i = TRAIL_LENGTH - 1; i > 0; i--) 
    {
        player->trail[i] = player->trail[i-1];
    }
    player->trail[0] = currentLocation;
    return;
}

// Update a player's location
void PlayerRepUpdatePlayerLocation(PlayerRep player, PlaceId LocationID)
{
    player->location = LocationID;
    return;
}

// Updates a player's move history
void PlayerRepUpdateMoveHistory(PlayerRep player, const char *LocationAbb)
{
    int i = 0;
    for (; strcmp(player->MoveHistory[i], "??") == 0 && i < MAX_ROUNDS; i++)
    {
    }
    player->MoveHistory[i] = strdup(LocationAbb);
}

// Update HunterPOV Trail
void PlayerRepUpdateHunterPOVTrail(PlayerRep player, 
    const char *LocationAbb, PlaceType LocationType)
{
    for (int i = TRAIL_LENGTH - 1; i > 0; i--) 
    {
        player->HunterPOVTrail[i] = player->HunterPOVTrail[i - 1];
    }    
    if (LocationType == LAND) {
        player->HunterPOVTrail[0] = "C?";
    } else if (LocationType == SEA) {
        player->HunterPOVTrail[0] = "S?";
    } else {
        player->HunterPOVTrail[0] = strdup(LocationAbb);
    }
    return;
}

// Update HunterPOV Move History
void PlayerRepUpdateHunterPOVMoveHistory(PlayerRep player, 
    const char *LocationAbb, PlaceType LocationType) 
{
    int i = 0;
    for (; strcmp(player->HunterPOVMoveHistory[i], "??") == 0 && 
        i < MAX_ROUNDS; i++)
    {
    }
    if (LocationType == LAND) {
        player->HunterPOVTrail[i] = "C?";
    } else if (LocationType == SEA) {
        player->HunterPOVTrail[i] = "S?";
    } else {
        player->HunterPOVTrail[i] = strdup(LocationAbb);
    }    
    return;
}

// Check if in Dracula's Trail
int PlayerRepCheckTrail(PlayerRep player, const char *LocationAbb)
{
    int i = 0;
    while (i < TRAIL_LENGTH) {
        if (strcmp(player->trail[i], LocationAbb)) {
            return true;
        } else {
            i++;
        }
    }
    return false;
}
