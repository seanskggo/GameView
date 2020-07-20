// Interface for the PlayerRep ADT. 
// For the 2020 COMP2521 T2 Assignment Fury OF Dracula.
// File written by Sam Schreyer (add contributors names as they contribute)

#ifndef PLAYER_REP_H_
#define PLAYER_REP_H_

typedef struct playerRep *PlayerRep;
    
PlayerRep newPlayerRep(int health, char *location);

void freePlayerRep(PlayerRep player);

int getPlayerHealth(PlayerRep player);

char *getPlayerLocation(PlayerRep player);

#endif