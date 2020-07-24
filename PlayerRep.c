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
    PlayerRepUpdatePlayerTrail(player, LocationAbb);
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
// Input: player, Location abbreviated
void PlayerRepUpdatePlayerTrail(PlayerRep player, const char *LocationAbb)
{
    for (int i = TRAIL_LENGTH - 1; i > 0; i--) 
    {
        player->trail[i] = player->trail[i-1];
    }
    player->trail[0] = strdup(LocationAbb);
    return;
}

// Update a player's location
// Input: player, location id
void PlayerRepUpdatePlayerLocation(PlayerRep player, PlaceId LocationID)
{
    player->location = LocationID;
    return;
}

// Updates a player's move history
// Input: player, abbreviated location
void PlayerRepUpdateMoveHistory(PlayerRep player, const char *LocationAbb)
{
    int i = 0;
    for (; strcmp(player->MoveHistory[i], "??") == 0 && i < MAX_ROUNDS; i++)
    {
    }
    player->MoveHistory[i] = strdup(LocationAbb);
}

// Update HunterPOV Trail
// Input: player, abbreviated location, type of location
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
// Input: player, abbreviated location, type of location
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
// Input: player (Dracula), the abbreviated location
// Output: NOWHERE if not on the trail, DoubleBack ID if double back was 
    // done at that location, Hide location if done at that location, PlaceId
    // otherwise.
PlaceId PlayerRepCheckTrail(PlayerRep player, const char *LocationAbb, 
    int roundNumber)
{
    int i = 0;
    int checkDouble = -1;
    int checkHide = -1;
    int tempRoundNumber = roundNumber;
    char *tempChar = strdup(LocationAbb);
    char *tempDoub = strdup(LocationAbb);
    char *tempHide = strdup(LocationAbb);
    while (i < TRAIL_LENGTH) {
        if (strcmp(player->trail[i], "D1") == 0) {
            tempDoub = strdup(player->trail[i]);
            checkDouble = 1;
        } else if (strcmp(player->trail[i], "D2") == 0) {
            tempDoub = strdup(player->trail[i]);
            checkDouble = 2;
        } else if (strcmp(player->trail[i], "D3") == 0) {
            tempDoub = strdup(player->trail[i]);
            checkDouble = 3;
        } else if (strcmp(player->trail[i], "D4") == 0) {
            tempDoub = strdup(player->trail[i]);
            checkDouble = 4;
        } else if (strcmp(player->trail[i], "D5") == 0) {
            tempDoub = strdup(player->trail[i]);
            checkDouble = 5;
        } else if (strcmp(player->trail[i], "HI") == 0) {
            tempHide = strdup(player->trail[i]);
            checkHide = 1;
        }
        if (strcmp(player->trail[i], LocationAbb)) {
            PlayerRepRevealDracula(player, i, tempRoundNumber);
            if (checkDouble == 0) {
                // a double back move was done at this location
                return placeAbbrevToId(tempDoub); 
            } else if (checkHide == 0) {
                // a hide move was done at this location
                return placeAbbrevToId(tempHide);
            } else {
                // Otherwise
                return placeAbbrevToId(tempChar);            
            }
            
        } else {
            i++;
            checkDouble--;
            checkHide--;
        }
    }
    return NOWHERE;
}

// Reveal Dracula's trail and move History
// Input: player (Dracula), int trail (where in the trail, 0-5) and #round
void PlayerRepRevealDracula(PlayerRep player, int TrailNumber, 
    int RoundNumber) {
    // Reveal Trail
    player->HunterPOVTrail[TrailNumber] = strdup(player->trail[TrailNumber]); 
    // Reveal Move History
    int revealMH = RoundNumber + TRAIL_LENGTH - 1 - TrailNumber;
    player->HunterPOVMoveHistory[revealMH] = 
        strdup(player->MoveHistory[revealMH]);
    return;
}

