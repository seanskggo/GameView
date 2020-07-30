// Item.h  ... definition for items in Queues
// Written by John Shepherd, March 2013

// Modified by Sam Schreyer.

#ifndef ITEM_H
#define ITEM_H

#include "Map.h"

typedef PlaceId Item;

#define ItemCopy(i) (i)
#define ItemEQ(i1, i2) ((i1) == (i2))
#define ItemShow(i) printf ("%d", (i))

#endif
