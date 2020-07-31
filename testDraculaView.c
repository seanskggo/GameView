////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testDraculaView.c: test the DraculaView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DraculaView.h"
#include "Game.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{	
	// start tests we wrote
	{///////////////////////////////////////////////////////////////////
		printf("Testing DvGetValidMoves No.1\n");

		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DHA.... "
			"GGA.... SGA.... HGA.... MGA.... DHI.... "
			"GCD.... SCD.... HCD.... MCD.... DCO.... "
			"GGA.... SGA.... HGA.... MGA.... DFR.... "
			"GCD.... SCD.... HCD.... MCD.... DLI.... "
			"GGA.... SGA.... HGA.... MGA.... DD1....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);

		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 2);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BERLIN);
		assert(moves[1] == NUREMBURG);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing DvGetValidMoves No.2\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DSR.... "
			"GGA.... SGA.... HGA.... MGA.... DBA.... "
			"GCD.... SCD.... HCD.... MCD.... DHI.... "
			"GGA.... SGA.... HGA.... MGA.... DAL.... "
			"GCD.... SCD.... HCD.... MCD.... DMA....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 7);
		sortPlaces(moves, numMoves);
		assert(moves[0] == CADIZ);
		assert(moves[1] == GRANADA);
		assert(moves[2] == LISBON);
		assert(moves[3] == SANTANDER);
		assert(moves[4] == DOUBLE_BACK_1);
		assert(moves[5] == DOUBLE_BACK_2);
		assert(moves[6] == DOUBLE_BACK_5);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing DvGetValidMoves No.3\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DPA.... "
			"GGA.... SGA.... HGA.... MGA.... DLE.... "
			"GCD.... SCD.... HCD.... MCD.... DNA....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 7);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BAY_OF_BISCAY);
		assert(moves[1] == BORDEAUX);
		assert(moves[2] == CLERMONT_FERRAND);
		assert(moves[3] == HIDE);
		assert(moves[4] == DOUBLE_BACK_1);
		assert(moves[5] == DOUBLE_BACK_2);
		assert(moves[6] == DOUBLE_BACK_3);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	} 
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing DvGetValidMoves No.4 (can't move to hospital)\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DSZ....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 6);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BELGRADE);
		assert(moves[1] == BUDAPEST);
		assert(moves[2] == KLAUSENBURG);
		assert(moves[3] == ZAGREB);
		assert(moves[4] == HIDE);
		assert(moves[5] == DOUBLE_BACK_1);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	} 
	
	{///////////////////////////////////////////////////////////////////
		printf("Testing DvGetValidMoves No.5 (can't HIDE at sea)\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DEC....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 6);
		sortPlaces(moves, numMoves);
		assert(moves[0] == ATLANTIC_OCEAN);
		assert(moves[1] == LE_HAVRE);
		assert(moves[2] == LONDON);
		assert(moves[3] == NORTH_SEA);
		assert(moves[4] == PLYMOUTH);
		assert(moves[5] == DOUBLE_BACK_1);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	} 
	
	{///////////////////////////////////////////////////////////////////
		printf("Testing DvGetValidMoves No.6 (can't double back/hide)\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DEC.... "
			"GCD.... SCD.... HCD.... MCD.... DD1.... "
			"GCD.... SCD.... HCD.... MCD.... DLE.... "
			"GCD.... SCD.... HCD.... MCD.... DHI....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 3);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BRUSSELS);
		assert(moves[1] == NANTES);
		assert(moves[2] == PARIS);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	} 
	
	{///////////////////////////////////////////////////////////////////
		printf("Testing DvGetValidMoves No.7 (can't hide)\n");
		
		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DEC.... "
			"GCD.... SCD.... HCD.... MCD.... DLE.... "
			"GCD.... SCD.... HCD.... MCD.... DHI....";
		Message messages[] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 5);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BRUSSELS);
		assert(moves[1] == NANTES);
		assert(moves[2] == PARIS);
		assert(moves[3] == DOUBLE_BACK_1);
		assert(moves[4] == DOUBLE_BACK_3);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	} // end tests we've written


	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for basic functions, "
			   "just before Dracula's first move\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "..."
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 0);
		assert(DvGetScore(dv) == GAME_START_SCORE);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == ZURICH);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 0);
		free(traps);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD.. SAO.... HCD.... MAO....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahah",
			"Aha!", "", "", ""
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);
		assert(DvGetScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == 5);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == 30);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		assert(DvGetVampireLocation(dv) == NOWHERE);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula leaving minions 1\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DED.V.. "
			"GST.... SST.... HST.... MST.... DMNT... "
			"GST.... SST.... HST.... MST.... DLOT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 4);
		assert(DvGetVampireLocation(dv) == EDINBURGH);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		assert(traps[0] == LONDON);
		assert(traps[1] == LONDON);
		assert(traps[2] == MANCHESTER);
		free(traps);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 4);
		sortPlaces(moves, numMoves);
		assert(moves[0] == GALATZ);
		assert(moves[1] == KLAUSENBURG);
		assert(moves[2] == HIDE);
		assert(moves[3] == DOUBLE_BACK_1);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DKL.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GGE.... SGE.... HGE.... MGE.... DBCT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == GALATZ);
		assert(locs[3] == SOFIA);
		free(locs);
		
		printf("Test passed!\n");
		DvFree(dv);
	}

	return EXIT_SUCCESS;
}
