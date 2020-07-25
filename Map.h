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
ConnList connListInsert(ConnList l, PlaceId p, TransportType type);

/** Return the length of a connList. */
int MapConnListLength(ConnList l);

/**
 *  Finds the distance between two RAIL locations
 *  Locations are assumed to be RAIL locations
 *	Code adapated from Sam Schreyer's code from the
 *  week 7 COMP2521 T2 2020 lab. Uses a Queue ADT written by
 *	John Shepherd, March 2013, that was supplied
 * 	with the week 7 COMP2521 T2 2020 lab. The Queue ADT
 *  relies on a slightly altered version of
 *  Item.h, also written by John Shepherd, March 2013.
 */
int MapGetRailDistance(Map m, PlaceId src, PlaceId dest);

#endif // !defined(FOD__MAP_H_)
