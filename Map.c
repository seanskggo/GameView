//////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// Map.c: an implementation of a Map type
// You can change this as much as you want!
// If you modify this, you should submit the new version.
//
// 2017-11-30	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "Map.h"
#include "Places.h"

struct map {
	int nV; // number of vertices
	int nE; // number of edges
	ConnList connections[NUM_REAL_PLACES];
};

static void addConnections(Map m);
static void addConnection(Map m, PlaceId v, PlaceId w, TransportType type);
static inline bool isSentinelEdge(Connection c);
static bool connListContains(ConnList l, PlaceId v, TransportType type);

// Our static functions:

// helper function for MapGetRailReachable

static ConnList getConnectionsToCheck(Map m, ConnList list, int iteration,
	int added[NUM_REAL_PLACES]);
// frees a given ConnList
static void freeConnList(ConnList list);

////////////////////////////////////////////////////////////////////////

// Creates a new map. 
// #vertices always same as NUM_REAL_PLACES
Map MapNew(void)
{
	Map m = malloc(sizeof(*m));
	if (m == NULL) {
		fprintf(stderr, "Couldn't allocate Map!\n");
		exit(EXIT_FAILURE);
	}

	m->nV = NUM_REAL_PLACES;
	m->nE = 0;
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		m->connections[i] = NULL;
	}

	addConnections(m);
	return m;
}

// Frees all memory allocated for the given map. 
void MapFree(Map m)
{
	assert (m != NULL);

	for (int i = 0; i < m->nV; i++) {
		ConnList curr = m->connections[i];
		while (curr != NULL) {
			ConnList next = curr->next;
			free(curr);
			curr = next;
		}
	}
	free(m);
}

////////////////////////////////////////////////////////////////////////

// Prints a map to `stdout`. 
void MapShow(Map m)
{
	assert(m != NULL);

	printf("V = %d, E = %d\n", m->nV, m->nE);
	for (int i = 0; i < m->nV; i++) {
		for (ConnList curr = m->connections[i]; curr != NULL; 
            curr = curr->next) {
			printf("%s connects to %s by %s\n",
                placeIdToName((PlaceId) i),
                placeIdToName(curr->p),
                transportTypeToString(curr->type)
			);
		}
	}
}

////////////////////////////////////////////////////////////////////////

// Gets the number of places in the map. 
int MapNumPlaces(Map m)
{
	assert(m != NULL);
	return m->nV;
}

// Gets the number of connections of a particular type. 
int MapNumConnections(Map m, TransportType type)
{
	assert(m != NULL);
	assert(transportTypeIsValid(type) || type == ANY);

	int nE = 0;
	for (int i = 0; i < m->nV; i++) {
		for (ConnList curr = m->connections[i]; curr != NULL; 
            curr = curr->next) {
			if (curr->type == type || type == ANY) {
				nE++;
			}
		}
	}
	return nE;
}


/// Insert a node into an adjacency list.
ConnList MapConnListInsert(ConnList l, PlaceId p, TransportType type)
{
	assert(placeIsReal(p));
	assert(transportTypeIsValid(type));

	ConnList new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate ConnNode");
		exit(EXIT_FAILURE);
	}

	new->p = p;
	new->type = type;
	new->next = l;
	return new;
}

////////////////////////////////////////////////////////////////////////

// Add edges to Graph representing map of Europe
static void addConnections(Map m)
{
	assert(m != NULL);

	for (int i = 0; !isSentinelEdge(CONNECTIONS[i]); i++) {
		addConnection(m, CONNECTIONS[i].v, CONNECTIONS[i].w, CONNECTIONS[i].t);
	}
}

// Add a new edge to the Map/Graph
static void addConnection(Map m, PlaceId start, PlaceId end, TransportType type)
{
	assert(m != NULL);
	assert(start != end);
	assert(placeIsReal(start));
	assert(placeIsReal(end));
	assert(transportTypeIsValid(type));

	// don't add edges twice
	if (connListContains(m->connections[start], end, type)) return;

	m->connections[start] = MapConnListInsert(m->connections[start], end, type);
	m->connections[end]   = MapConnListInsert(m->connections[end], start, type);
	m->nE++;
}

// Is this the magic 'sentinel' edge?
static inline bool isSentinelEdge(Connection c)
{
	return c.v == -1 && c.w == -1 && c.t == ANY;
}

// Does this adjacency list contain a particular value?
static bool connListContains(ConnList l, PlaceId p, TransportType type)
{
	assert(placeIsReal(p));
	assert(transportTypeIsValid(type));

	for (ConnList curr = l; curr != NULL; curr = curr->next) {
		if (curr->p == p && curr->type == type) {
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////

ConnList MapGetConnections(Map m, PlaceId p)
{
	assert(placeIsReal(p));
	return m->connections[p];
}

////////////////////////////////////////////////////////////////////////

/// Functions we added

int MapConnListLength(ConnList l)
{
	ConnList curr = l;
	int length = 0;;
	for (;curr != NULL;curr = curr->next) {
		length++;
	}
	return length;
}

ConnList MapGetRailReachable(Map m, PlaceId src, int dist,
	ConnList reachableLocs, int *numReturnedLocs, int *added)
{
	// We add the source to our added array
	added[src] = 1;

	ConnList connsToCheck =  m->connections[src];
	ConnList curr = NULL;
	ConnList prev = NULL;
	int i = 0;
	for(; i < dist; i++) {
		// getConnectionsToCheck relies upon connsToCheck, which is why
		// we need to separate it from curr
	    connsToCheck = getConnectionsToCheck(m, connsToCheck, i, added);
	    curr = connsToCheck;
	    
		while (curr != NULL) {
			// if we haven't added the element, and the connection type
			// if rail, then we can add it!!
			if (curr->type == RAIL && added[curr->p] == -1) {
				// add the location to our reachableLocs array!
				reachableLocs = MapConnListInsert(reachableLocs, curr->p,
					curr->type);
				added[curr->p] = 1;
				*numReturnedLocs += 1;
			}
			curr = curr->next;
		}
		if (i > 1) {
		    ConnList tempList;
		    while (prev != NULL) {
		        tempList = prev;
		        prev = prev->next;
		        free(tempList);
		    }
		}
		prev = connsToCheck;
	}
	
	if (i == 2) {
		freeConnList(connsToCheck);
	}
	return reachableLocs;
}

bool MapConnListContains(ConnList list, PlaceId place)
{
	ConnList curr = list;
	while (curr != NULL) {
		if (list->p == place) {
			return true;
		}
		curr = curr->next;
	}

	return false;
}

// Static helper functions we have added

static ConnList getConnectionsToCheck(Map m, ConnList list, int iteration,
	int added[NUM_REAL_PLACES])
{
	if (iteration == 0) {
		return list;
	} else {
		// loop through the list we are given and
		// add all of the connections of that connlist
		// so (map->connections[ConnList->p]) to a new list that we return!

		// only need to add it if it's a rail conneciton.
		ConnList connsToCheck = NULL;
		int i = 0;
		for(ConnList curr = list; curr != NULL; curr = curr->next) {
			// loop through all the connections of the outer curr,
			// and add them to our connsToCheck if their type is RAIL
			ConnList connecs = m->connections[curr->p];
			for (; connecs != NULL; connecs = connecs->next) {
				if (curr->type == RAIL && connecs->type == RAIL && 
					added[connecs->p] != 1) {
					connsToCheck = MapConnListInsert(connsToCheck, connecs->p,
						connecs->type);
				}
			}
			i++;
		}
		return connsToCheck;
	}
}

static void freeConnList(ConnList list) 
{
	ConnList curr = list;
	ConnList prev = NULL;
	for(; curr != NULL;) {
		prev = curr;
        curr = curr->next;
        free(prev);
	}
	return;
}

