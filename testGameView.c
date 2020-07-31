////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testGameView.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "testUtils.h"

#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"
#define RED   "\x1B[31m"	

// When trap is located in CITY_UNKNOWN and you call GvGetTraplocaiton
static void delay(int i, bool speed) {
	if (speed) return;
	long long int time = i*CLOCKS_PER_SEC;
	while (clock() < time);
}

int main(void)
{
	int i = 1;
	char b;
	bool speed = false;
	printf("*** Starting tests ***\n\n");
	printf("Would you like to speed test? (y or n)?\nType here: ");
	scanf("%c", &b);
	printf("\n");
	if (b == 'y') {
		speed = true;
	} else if (b == 'n') {
		speed = false;	
		printf(RED "This testing process has been time delayed\n\n" RESET);
	}
	else {
		printf("Invalid answer. Terminating...\n");
		exit(EXIT_FAILURE);
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Hunter Part of GvGetReachable() No1.\n" RESET);

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 3;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DR_SEWARD, round,
			FRANKFURT, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Cologne") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Leipzig") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Nuremburg") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Strasbourg") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Frankfurt") == 0);
		reachableLength++;
 		assert(returnedLocs == 5 && reachableLength == 5);
		free(reachable);

		GvFree(gv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Hunter Part of GvGetReachable() No.2\n" RESET);

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 0;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DR_SEWARD, round,
			CLERMONT_FERRAND, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Bordeaux") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Geneva") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Marseilles") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Nantes") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Paris") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[5]), "Toulouse") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[6]), 
			"Clermont-Ferrand") == 0);
		reachableLength++;

 		assert(returnedLocs == 7 && reachableLength == 7);
		free(reachable);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Hunter Part of GvGetReachable() No.3\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MVI.... DEC.... "
			"GST.... SST.... HST.... MBD.... DD1.... "
			"GFR.... SFR.... HFR.... MSZ.... DPL.... "
			"GFR.... SFR.... HFR.... MSZ....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = GvGetRound(gv);
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_MINA_HARKER, round,
			SZEGED, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Varna") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Salonica") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Venice") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Prague") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Vienna") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[5]), "Galatz") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[6]), "Constanta") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[7]), "Sofia") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[8]), "Bucharest") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[9]), "Belgrade") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[10]), "Budapest") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[11]), "Klausenburg") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[12]), 
			"St Joseph and St Mary") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[13]), "Zagreb") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[14]), "Szeged") == 0);
		reachableLength++;

 		assert(returnedLocs == 15 && reachableLength == 15);
		free(reachable);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Drac Part of GvGetReachable() No.1\n" RESET);

		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DHA.... "
			"GGA.... SGA.... HGA.... MGA.... DHI.... "
			"GCD.... SCD.... HCD.... MCD.... DCO.... "
			"GGA.... SGA.... HGA.... MGA.... DFR.... "
			"GCD.... SCD.... HCD.... MCD.... DLI.... "
			"GGA.... SGA.... HGA.... MGA.... DD1....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 1;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, round,
			LEIPZIG, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Berlin") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Nuremburg") == 0);
		reachableLength++;

 		assert(returnedLocs == 2 && reachableLength == 2);
		free(reachable);
		
		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Drac Part of GvGetReachable() No.2\n" RESET);

		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DHA....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 1;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, round,
			HAMBURG, &returnedLocs);
		int reachableLength = 0;
		
		assert(strcmp(placeIdToName(reachable[0]), "North Sea") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Berlin") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Cologne") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Leipzig") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Hamburg") == 0);
		reachableLength++;

 		assert(returnedLocs == 5 && reachableLength == 5);
		free(reachable);
		
		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Drac Part of GvGetReachable() No.3\n" RESET);

		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DSR.... "
			"GGA.... SGA.... HGA.... MGA.... DBA.... "
			"GCD.... SCD.... HCD.... MCD.... DHI.... "
			"GGA.... SGA.... HGA.... MGA.... DAL.... "
			"GCD.... SCD.... HCD.... MCD.... DMA....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 1;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, round,
			MADRID, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Alicante") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Cadiz") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Granada") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Lisbon") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Santander") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[5]), "Saragossa") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[6]), "Madrid") == 0);
		reachableLength++;

 		assert(returnedLocs == 7 && reachableLength == 7);
		free(reachable);
		
		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////
		printf(YEL "Testing Drac Part of GvGetReachable() No.4\n" RESET);

		char *trail =
			"GCD.... SCD.... HCD.... MCD.... DMA.... "
			"GGA.... SGA.... HGA.... MGA.... DD1....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		int round = 1;
		int returnedLocs = 0;
		PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, round,
			MADRID, &returnedLocs);
		int reachableLength = 0;

		assert(strcmp(placeIdToName(reachable[0]), "Alicante") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[1]), "Cadiz") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[2]), "Granada") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[3]), "Lisbon") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[4]), "Santander") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[5]), "Saragossa") == 0);
		reachableLength++;
		assert(strcmp(placeIdToName(reachable[6]), "Madrid") == 0);
		reachableLength++;

 		assert(returnedLocs == 7 && reachableLength == 7);
		free(reachable);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetPlayerLocation\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DTP.... "
			"GFR.... SFR.... HFR.... MFRTTTD";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);
		assert(GvGetPlayerLocation(gv, 
            PLAYER_MINA_HARKER) == ST_JOSEPH_AND_ST_MARY);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Individual Testing\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DD1.... "
			"GFR.... SFR.... HFR.... MFR....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}


	{///////////////////////////////////////////////////////////////////

		printf(YEL "Basic initialisation\n" RESET);

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetHealth(gv, 
            PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "After Lord Godalming's turn\n" RESET);

		char *trail =
			"GST....";

		Message messages[1] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DR_SEWARD);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == NOWHERE);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "After Mina Harker's turn\n" RESET);

		char *trail =
			"GST.... SAO.... HZU.... MBB....";

		Message messages[4] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == NOWHERE);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "After Dracula's turn\n" RESET);

		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";

		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};

		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 1);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) 
			== GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == CITY_UNKNOWN);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Encountering Dracula\n" RESET);

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..";

		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};

		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
            GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GENEVA);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for Dracula doubling back at sea, "
            "and losing blood points (Hunter View)\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";

		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};

		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == 
		   GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == SEA_UNKNOWN);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}


	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for Dracula doubling back at sea, "
            "and losing blood points (Dracula View)\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DD1.... "
			"GFR.... SFR.... HFR.... MFR....";

		Message messages[14] = {
			"Hello", "Rubbish", "Stuff", "", "Mwahahah",
			"Aha!", "", "", "", "Back I go"};

		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == ENGLISH_CHANNEL);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking that hunters' health points are capped\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE....";

		Message messages[6] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) 
		   == GAME_START_HUNTER_LIFE_POINTS);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing a hunter 'dying'\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD";

		Message messages[21] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetScore(gv) == GAME_START_SCORE
            - 4 * SCORE_LOSS_DRACULA_TURN
            - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == 0);
		assert(GvGetPlayerLocation(gv, 
            PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == STRASBOURG);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

        printf(YEL "Test for when trap leaves the trail\n" RESET);

        char *trail =
            "GCD.... SCD.... HCD.... MCD.... DSRT... "
            "GGA.... SGA.... HGA.... MGA.... DBAT... "
            "GCD.... SCD.... HCD.... MCD.... DHIT... "
            "GGA.... SGA.... HGA.... MGA.... DALT... "
            "GCD.... SCD.... HCD.... MCD.... DMAT... "
            "GCD.... SCD.... HCD.... MCD.... DD1V... "
            "GCD.... SCD.... HCD.... MCD.... DHI..M.";
        Message messages[] = {};
        GameView gv = GvNew(trail, messages);

        assert(GvGetRound(gv) == 7);
        assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
        int numTraps = 0;
        PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
        assert(numTraps == 4);
        sortPlaces(traps, numTraps);
        assert(traps[0] == ALICANTE);
        assert(traps[1] != SARAGOSSA && traps[2] != SARAGOSSA &&
            traps[3] != SARAGOSSA);
        free(traps);

        delay(i, speed);
        i++;
        GvFree(gv);
        printf(GRN "Test passed!\n" RESET);
        delay(i, speed);
        i++;
    }

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing Dracula doubling back to Castle Dracula\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";

		Message messages[10] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing vampire/trap locations\n" RESET);

		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GSZ.... SGE.... HGE.... MGE....";

		Message messages[19] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GALATZ);
		assert(GvGetVampireLocation(gv) == CASTLE_DRACULA);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == GALATZ && traps[1] == KLAUSENBURG);
		free(traps);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing trap locations after one is destroyed\n" RESET);

		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DBC.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GBE.... SGE.... HGE.... MGE.... DCNT... "
			"GKLT... SGE.... HGE.... MGE....";

		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
            GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(GvGetVampireLocation(gv) == BUCHAREST);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == CONSTANTA && traps[1] == GALATZ);
		free(traps);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing a vampire maturing\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.";

		Message messages[35] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetScore(gv) == GAME_START_SCORE - 7 * 
            SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_VAMPIRE_MATURES);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing move/location history\n" RESET);

		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";

		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) ==
            GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		// Lord Godalming's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
                &numMoves, &canFree);
			assert(numMoves == 7);
			assert(moves[0] == LISBON);
			assert(moves[1] == CADIZ);
			assert(moves[2] == GRANADA);
			assert(moves[3] == ALICANTE);
			assert(moves[4] == SARAGOSSA);
			assert(moves[5] == SANTANDER);
			assert(moves[6] == MADRID);
			if (canFree) free(moves);
		}

		// Dracula's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_DRACULA,
                &numMoves, &canFree);
			assert(numMoves == 6);
			assert(moves[0] == STRASBOURG);
			assert(moves[1] == CITY_UNKNOWN);
			assert(moves[2] == CITY_UNKNOWN);
			assert(moves[3] == DOUBLE_BACK_3);
			assert(moves[4] == HIDE);
			assert(moves[5] == CITY_UNKNOWN);
			if (canFree) free(moves);
		}

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing move/location history (Extended)\n" RESET);

		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";

		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) ==
            GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		// Lord Godalming's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
                &numMoves, &canFree);
			assert(numMoves == 7);
			assert(moves[0] == LISBON);
			assert(moves[1] == CADIZ);
			assert(moves[2] == GRANADA);
			assert(moves[3] == ALICANTE);
			assert(moves[4] == SARAGOSSA);
			assert(moves[5] == SANTANDER);
			assert(moves[6] == MADRID);
			if (canFree) free(moves);
		}

		// Dracula's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_DRACULA,
                &numMoves, &canFree);
			assert(numMoves == 6);
			assert(moves[0] == STRASBOURG);
			assert(moves[1] == CITY_UNKNOWN);
			assert(moves[2] == CITY_UNKNOWN);
			assert(moves[3] == DOUBLE_BACK_3);
			assert(moves[4] == HIDE);
			assert(moves[5] == CITY_UNKNOWN);
			if (canFree) free(moves);
		}

		{
			int numLocs = 0; bool canFree = false;
			PlaceId *locs = GvGetLocationHistory(gv, PLAYER_DRACULA,
                &numLocs, &canFree);
			assert(numLocs == 6);
			assert(locs[0] == STRASBOURG);
			assert(locs[1] == CITY_UNKNOWN);
			assert(locs[2] == CITY_UNKNOWN);
			assert(locs[3] == STRASBOURG);
			assert(locs[4] == STRASBOURG);
			assert(locs[5] == CITY_UNKNOWN);
			if (canFree) free(locs);
		}

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetMoveHistory\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
        PlaceId *HLG = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
            &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 4);
        assert(HLGcanFree == true);
        assert(HLG[0] == GENEVA);
        assert(HLG[1] == STRASBOURG);
        assert(HLG[2] == FRANKFURT);
        assert(HLG[3] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
        PlaceId *HDS = GvGetMoveHistory(gv, PLAYER_DR_SEWARD, &HDStotal, 
            &HDScanFree);
        assert(HDStotal == 4);
        assert(HDScanFree == true);
        assert(HDS[0] == BERLIN);
        assert(HDS[1] == PRAGUE);
        assert(HDS[2] == VIENNA);
        assert(HDS[3] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
        PlaceId *HVH = GvGetMoveHistory(gv, PLAYER_VAN_HELSING,
            &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
        PlaceId *HMH = GvGetMoveHistory(gv, PLAYER_MINA_HARKER, 
            &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 4);
        assert(HMHcanFree == true);
        assert(HMH[0] == VENICE);
        assert(HMH[1] == MILAN);
        assert(HMH[2] == ZURICH);
        assert(HMH[3] == STRASBOURG);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
        PlaceId *DRA = GvGetMoveHistory(gv, PLAYER_DRACULA, 
            &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 4);
        assert(DRAcanFree == true);
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == CITY_UNKNOWN);
        assert(DRA[2] == DOUBLE_BACK_1);
        assert(DRA[3] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetLastMoves where "
            "numMoves < total moves\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
		int HLGnumMoves = 3;
        PlaceId *HLG = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,
            HLGnumMoves, &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 3);
        assert(HLGcanFree == true);
        assert(HLG[0] == STRASBOURG);
        assert(HLG[1] == FRANKFURT);
        assert(HLG[2] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
		int HDSnumMoves = 2;
        PlaceId *HDS = GvGetLastMoves(gv, PLAYER_DR_SEWARD,
            HDSnumMoves, &HDStotal, &HDScanFree);
        assert(HDStotal == 2);
        assert(HDScanFree == true);
        assert(HDS[0] == VIENNA);
        assert(HDS[1] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
		int HVHnumMoves = 4;
        PlaceId *HVH = GvGetLastMoves(gv, PLAYER_VAN_HELSING,
            HVHnumMoves, &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
		int HMHnumMoves = 0;
        PlaceId *HMH = GvGetLastMoves(gv, PLAYER_MINA_HARKER,
            HMHnumMoves, &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 0);
        assert(HMHcanFree == false);
        assert(HMH == NULL);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
		int DRAnumMoves = 3;
        PlaceId *DRA = GvGetLastMoves(gv, PLAYER_DRACULA,
            DRAnumMoves, &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 3);
        assert(DRAcanFree == true);;
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == DOUBLE_BACK_1);
        assert(DRA[2] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetLastMoves where "
            "numMoves > total moves\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
		int HLGnumMoves = 5;
        PlaceId *HLG = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,
            HLGnumMoves, &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 4);
        assert(HLGcanFree == true);
        assert(HLG[0] == GENEVA);
        assert(HLG[1] == STRASBOURG);
        assert(HLG[2] == FRANKFURT);
        assert(HLG[3] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
		int HDSnumMoves = 7;
        PlaceId *HDS = GvGetLastMoves(gv, PLAYER_DR_SEWARD,
            HDSnumMoves, &HDStotal, &HDScanFree);
        assert(HDStotal == 4);
        assert(HDScanFree == true);
        assert(HDS[0] == BERLIN);
        assert(HDS[1] == PRAGUE);
        assert(HDS[2] == VIENNA);
        assert(HDS[3] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
		int HVHnumMoves = 9;
        PlaceId *HVH = GvGetLastMoves(gv, PLAYER_VAN_HELSING,
            HVHnumMoves, &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
		int HMHnumMoves = 10;
        PlaceId *HMH = GvGetLastMoves(gv, PLAYER_MINA_HARKER,
            HMHnumMoves, &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 4);
        assert(HMHcanFree == true);
        assert(HMH[0] == VENICE);
        assert(HMH[1] == MILAN);
        assert(HMH[2] == ZURICH);
        assert(HMH[3] == STRASBOURG);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
		int DRAnumMoves = 500;
        PlaceId *DRA = GvGetLastMoves(gv, PLAYER_DRACULA,
            DRAnumMoves, &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 4);
        assert(DRAcanFree == true);
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == CITY_UNKNOWN);
        assert(DRA[2] == DOUBLE_BACK_1);
        assert(DRA[3] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetLocationHistory\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
        PlaceId *HLG = GvGetLocationHistory(gv, PLAYER_LORD_GODALMING,
             &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 4);
        assert(HLGcanFree == true);
        assert(HLG[0] == GENEVA);
        assert(HLG[1] == STRASBOURG);
        assert(HLG[2] == FRANKFURT);
        assert(HLG[3] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
        PlaceId *HDS = GvGetLocationHistory(gv, PLAYER_DR_SEWARD,
             &HDStotal, &HDScanFree);
        assert(HDStotal == 4);
        assert(HDScanFree == true);
        assert(HDS[0] == BERLIN);
        assert(HDS[1] == PRAGUE);
        assert(HDS[2] == VIENNA);
        assert(HDS[3] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
        PlaceId *HVH = GvGetLocationHistory(gv, PLAYER_VAN_HELSING,
            &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
        PlaceId *HMH = GvGetLocationHistory(gv, PLAYER_MINA_HARKER,
            &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 4);
        assert(HMHcanFree == true);
        assert(HMH[0] == VENICE);
        assert(HMH[1] == MILAN);
        assert(HMH[2] == ZURICH);
        assert(HMH[3] == STRASBOURG);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
        PlaceId *DRA = GvGetLocationHistory(gv, PLAYER_DRACULA,
            &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 4);
        assert(DRAcanFree == true);
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == CITY_UNKNOWN);
        assert(DRA[2] == CITY_UNKNOWN);
        assert(DRA[3] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetLastLocations where "
            "numMoves < total moves\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
		int HLGnumMoves = 3;
        PlaceId *HLG = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,
            HLGnumMoves, &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 3);
        assert(HLGcanFree == true);
        assert(HLG[0] == STRASBOURG);
        assert(HLG[1] == FRANKFURT);
        assert(HLG[2] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
		int HDSnumMoves = 2;
        PlaceId *HDS = GvGetLastLocations(gv, PLAYER_DR_SEWARD,
            HDSnumMoves, &HDStotal, &HDScanFree);
        assert(HDStotal == 2);
        assert(HDScanFree == true);
        assert(HDS[0] == VIENNA);
        assert(HDS[1] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
		int HVHnumMoves = 4;
        PlaceId *HVH = GvGetLastLocations(gv, PLAYER_VAN_HELSING,
            HVHnumMoves, &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
		int HMHnumMoves = 0;
        PlaceId *HMH = GvGetLastLocations(gv, PLAYER_MINA_HARKER,
            HMHnumMoves, &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 0);
        assert(HMHcanFree == false);
        assert(HMH == NULL);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
		int DRAnumMoves = 3;
        PlaceId *DRA = GvGetLastLocations(gv, PLAYER_DRACULA,
            DRAnumMoves, &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 3);
        assert(DRAcanFree == true);
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == CITY_UNKNOWN);
        assert(DRA[2] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

	}
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Test for GvGetLastLocations where "
            "numMoves > total moves\n" RESET);

		char *trail =
			"GGE.... SBR.... HMA.... MVE.... DC?.... "
			"GST.... SPR.... HGR.... MMI.... DC?.... "
			"GFR.... SVI.... HMA.... MZU.... DD1.... "
			"GCO.... SBD.... HSR.... MST.... DC?....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		bool HLGcanFree = false;
		int HLGtotal = 0;
		int HLGnumMoves = 5;
        PlaceId *HLG = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,
            HLGnumMoves, &HLGtotal, &HLGcanFree);
        assert(HLGtotal == 4);
        assert(HLGcanFree == true);
        assert(HLG[0] == GENEVA);
        assert(HLG[1] == STRASBOURG);
        assert(HLG[2] == FRANKFURT);
        assert(HLG[3] == COLOGNE);
        free(HLG);

		bool HDScanFree = false;
		int HDStotal = 0;
		int HDSnumMoves = 7;
        PlaceId *HDS = GvGetLastLocations(gv, PLAYER_DR_SEWARD,
            HDSnumMoves, &HDStotal, &HDScanFree);
        assert(HDStotal == 4);
        assert(HDScanFree == true);
        assert(HDS[0] == BERLIN);
        assert(HDS[1] == PRAGUE);
        assert(HDS[2] == VIENNA);
        assert(HDS[3] == BUDAPEST);
        free(HDS);

		bool HVHcanFree = false;
		int HVHtotal = 0;
		int HVHnumMoves = 9;
        PlaceId *HVH = GvGetLastLocations(gv, PLAYER_VAN_HELSING,
            HVHnumMoves, &HVHtotal, &HVHcanFree);
        assert(HVHtotal == 4);
        assert(HVHcanFree == true);
        assert(HVH[0] == MADRID);
        assert(HVH[1] == GRANADA);
        assert(HVH[2] == MADRID);
        assert(HVH[3] == SARAGOSSA);
        free(HVH);

		bool HMHcanFree = false;
		int HMHtotal = 0;
		int HMHnumMoves = 10;
        PlaceId *HMH = GvGetLastLocations(gv, PLAYER_MINA_HARKER,
            HMHnumMoves, &HMHtotal, &HMHcanFree);
        assert(HMHtotal == 4);
        assert(HMHcanFree == true);
        assert(HMH[0] == VENICE);
        assert(HMH[1] == MILAN);
        assert(HMH[2] == ZURICH);
        assert(HMH[3] == STRASBOURG);
        free(HMH);

		bool DRAcanFree = false;
		int DRAtotal = 0;
		int DRAnumMoves = 500;
        PlaceId *DRA = GvGetLastLocations(gv, PLAYER_DRACULA,
            DRAnumMoves, &DRAtotal, &DRAcanFree);
        assert(DRAtotal == 4);
        assert(DRAcanFree == true);
        assert(DRA[0] == CITY_UNKNOWN);
        assert(DRA[1] == CITY_UNKNOWN);
        assert(DRA[2] == CITY_UNKNOWN);
        assert(DRA[3] == CITY_UNKNOWN);
        free(DRA);

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;

		}
	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Testing connections\n" RESET);

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		{
			printf("\tChecking Galatz road connections "
                "(Lord Godalming, Round 1)");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
                1, GALATZ, true, false, false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
			printf(GRN "\tPassed\n" RESET);
		}

		{
			printf("\tChecking Ionian Sea boat connections "
                "(Lord Godalming, Round 1)");

			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
                1, IONIAN_SEA, false, false, true, &numLocs);

			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
			printf(GRN "\tPassed\n" RESET);
		}

		{
			printf("\tChecking Paris rail connections "
			       "(Lord Godalming, Round 2)");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
                2, PARIS, false, true, false, &numLocs);

			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BORDEAUX);
			assert(locs[1] == BRUSSELS);
			assert(locs[2] == COLOGNE);
			assert(locs[3] == LE_HAVRE);
			assert(locs[4] == MARSEILLES);
			assert(locs[5] == PARIS);
			assert(locs[6] == SARAGOSSA);
			free(locs);
			printf(GRN "\tPassed\n" RESET);
		}

		{
			printf("\tChecking Athens rail connections (none)");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
                1, ATHENS, false, true, false, &numLocs);

			assert(numLocs == 1);
			assert(locs[0] == ATHENS);
			free(locs);
			printf(GRN "\t\t\t\tPassed\n" RESET);
		}

		delay(i, speed);
		i++;
		GvFree(gv);
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}

	printf("\nTotal test time: %ld seconds", clock()/CLOCKS_PER_SEC);
	printf("\nVerdict: " GRN "Passed\n\n" RESET);
	return EXIT_SUCCESS;
}
