////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... The Fury of Dracula
// Map.h: an interface to a Map data type
// You can change this as much as you want!
// If you modify this, you should submit the new version.
//
// 2017-11-30   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31   v2.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdlib.h>

#include "Places.h"

#ifndef FOD__MAP_H_
#define FOD__MAP_H_

typedef struct connNode *ConnList;
struct connNode {
	PlaceId p; // ALICANTE, etc.
	TransportType type; // ROAD, RAIL, BOAT
	ConnList next; // link to next node
};

// Map representation is hidden
typedef struct map *Map;

/** Creates a new map. */
Map MapNew(void);

/** Frees all memory allocated for the given map. */
void MapFree(Map m);

/** Prints a map to `stdout`. */
void MapShow(Map m);

/** Gets the number of places in the map. */
int MapNumPlaces(Map m);

/** Gets the number of connections of a particular type. */
int MapNumConnections(Map m, TransportType type);

/**
 *  Gets a list of connections from the given place.
 *  The returned list should NOT be modified or freed.
 */
ConnList MapGetConnections(Map m, PlaceId p);


/** Create a new, empty connList. */
ConnList MapNewEmptyConnList(void);

/**
 *  Inserts a new connNode * into a "connList".
 *	Used to be a static function provided by "Team Dracula" in Map.c,
 *  But is useful as an interface function
 */
ConnList MapConnListInsert(ConnList l, PlaceId p, TransportType type);

/** Return the length of a connList. */
int MapConnListLength(ConnList l);

/**
 *  Finds the locations reachable by rail with a particular number of
 *  "hops" that is specified by the distance parameter
 */
ConnList MapGetRailReachable(Map m, PlaceId src, int dist,
	ConnList reachableLocs, int *numReturnedLocs, int *added);

/** Checks if a given PlaceId is in a ConnList */
bool MapConnListContains(ConnList list, PlaceId place);

#endif // !defined(FOD__MAP_H_)
