////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testHunterView.c: test the HunterView ADT
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
#include <time.h>

#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "testUtils.h"

#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"
#define RED   "\x1B[31m"

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

		printf(YEL "Checking HvWhereCanTheyGoByType if no moves made)\n" RESET);

		char *trail ="";

		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *dracLocs = HvWhereCanTheyGoByType(hv, PLAYER_DRACULA,
            true, false, false, &numLocs);
		PlaceId *helsingLocs = HvWhereCanTheyGoByType(hv, PLAYER_VAN_HELSING,
            true, false, false, &numLocs);
		assert(dracLocs == 0);
		assert(helsingLocs == 0);
		free(dracLocs);
		free(helsingLocs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}
	
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking HvWhereCanTheyGoif no moves made)\n" RESET);

		char *trail ="";

		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *dracLocs = HvWhereCanTheyGo(hv, PLAYER_DRACULA,
			&numLocs);
		PlaceId *helsingLocs = HvWhereCanTheyGo(hv, PLAYER_VAN_HELSING,
			&numLocs);
		assert(dracLocs == 0);
		assert(helsingLocs == 0);
		free(dracLocs);
		free(helsingLocs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}
	
	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking HvGetShortestPathTo if no moves made)\n" RESET);

		char *trail ="";
		Message messages[1] = {};
		
		HunterView hv = HvNew(trail, messages);
		int pathLength = -1;
		PlaceId *path = HvGetShortestPathTo(hv, PLAYER_VAN_HELSING,
			ROME, &pathLength);
		assert(pathLength == 0);
		assert(path == NULL);
		free(path);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}
	
	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Basic initialisation\n" RESET);

		char *trail = "";
		Message messages[] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) 
            == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "After Lord Godalming's turn\n" RESET);
		
		char *trail =
            "GST....";
		Message messages[1] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_DR_SEWARD);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "After Van Helsing's turn\n" RESET);
		
		char *trail =
            "GST.... SAO.... HZU....";
		Message messages[3] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_MINA_HARKER);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "After Dracula's turn\n" RESET);
		
		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";

		Message messages[] = {
	        "Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};
		
		HunterView hv = HvNew(trail, messages);
		assert(HvGetRound(hv) == 1);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Encountering Dracula\n" RESET);
		
		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..";
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) ==
            GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == GENEVA);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == GENEVA);
		assert(round == 0);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
    
		printf(YEL "Test for Dracula doubling back at sea, "
			   "and losing blood points\n" RESET);

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetRound(hv) == 2);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == SEA_UNKNOWN);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Testing a hunter 'dying'\n" RESET);
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD";
		Message messages[21] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetScore(hv) == GAME_START_SCORE
            - 4 * SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == 0);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == STRASBOURG);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Testing Dracula doubling back to Castle Dracula\n" RESET);
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";
		Message messages[10] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CASTLE_DRACULA);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Testing vampire location\n" RESET);
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE....";
		Message messages[18] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == CASTLE_DRACULA);
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

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
		
		HunterView hv = HvNew(trail, messages);
		assert(HvGetScore(hv) == GAME_START_SCORE
            - 7 * SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_VAMPIRE_MATURES);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}
	
	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Testing Dracula's last known location 1\n" RESET);
		
		char *trail =
			"GGE.... SGE.... HVI.... MGE.... DCD.V.. "
			"GGE.... SGE.... HBD.... MGE.... DKLT... "
			"GGE.... SGE.... HSZ.... MGE.... DC?T... "
			"GGE.... SGE.... HKLT... MGE.... DC?T... "
			"GGE.... SGE.... HCDV... MGE.... DD1T...";
		Message messages[25] = {};

		HunterView hv = HvNew(trail, messages);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == KLAUSENBURG);
		assert(round == 1);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
	
		printf(YEL "Testing Dracula's last known location CITY_UNKNOWN\n" RESET);
		
		char *trail =
			"GGE.... SGE.... HVI.... MGE.... DC?.V.. "
			"GGE.... SGE.... HBD.... MGE.... DC?T... "
			"GGE.... SGE.... HSZ.... MGE.... DC?T... "
			"GGE.... SGE.... HKLT... MGE.... DC?T... "
			"GGE.... SGE.... HCDV... MGE.... DD1T...";
	
		Message messages[25] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		assert(round == -1);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
	
		printf(YEL "Testing Dracula's last known location no moves\n" RESET);
		
		char *trail =
			"GGE.... SGE.... HVI.... MGE....";
	
		Message messages[4] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		assert(round == -1);
		
		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////
		
		printf(YEL "Testing shortest path 1\n" RESET);

		char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";

		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);

		{
			printf("\tLisbon -> Barcelona (Lord Godalming, Round 1)");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING,
                BARCELONA, &pathLength);
			assert(pathLength == 2);
			assert(path[0] == MADRID);
			assert(path[1] == BARCELONA);
			free(path);
			printf(GRN "\t passed\n" RESET);
		}

		{
			printf("\tLisbon -> Cologne (Lord Godalming, Round 1)");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING,
                COLOGNE, &pathLength);
           
			assert(pathLength == 3);
			assert(path[0] == MADRID);
			assert(path[1] == BORDEAUX);
			assert(path[2] == COLOGNE);
			free(path);
			printf(GRN "\t passed\n" RESET);
		}

		{
			printf("\tSwansea -> Hamburg (Van Helsing, Round 1)");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_VAN_HELSING,
                HAMBURG, &pathLength); 
			assert(pathLength == 3);
			assert(path[0] == EDINBURGH);
			assert(path[1] == NORTH_SEA);
			assert(path[2] == HAMBURG);
			free(path);
			printf(GRN "\t passed\n" RESET);
		}

		{
			printf("\tMarseilles -> Constanta (Mina Harker, Round 1)");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_MINA_HARKER,
                CONSTANTA, &pathLength);
			assert(pathLength == 4);
			assert(path[0] == GENOA || path[0] == MILAN);
			assert(path[1] == VENICE);
			assert(path[2] == BUDAPEST);
			assert(path[3] == CONSTANTA);
			free(path);
			printf(GRN "\t passed\n" RESET);
		}

		{
			printf("\tLisbon -> Castle Dracula (Dr. Seward, Round 1)");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_DR_SEWARD,
                CASTLE_DRACULA, &pathLength);
			assert(pathLength == 7);
			assert(path[0] == SARAGOSSA);
			assert(path[1] == MARSEILLES);
			assert(path[2] == GENOA || path[2] == MILAN);
			assert(path[3] == VENICE);
			assert(path[4] == BUDAPEST);
			assert(path[5] == GALATZ || path[5] == KLAUSENBURG);
			assert(path[6] == CASTLE_DRACULA);
			free(path);
			printf(GRN "\t passed\n" RESET);
		}

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking Galatz road connections "
		       "(Lord Godalming, Round 1)\n" RESET);

		char *trail = "GGA....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
            true, false, false, &numLocs);

		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BUCHAREST);
		assert(locs[1] == CASTLE_DRACULA);
		assert(locs[2] == CONSTANTA);
		assert(locs[3] == GALATZ);
		assert(locs[4] == KLAUSENBURG);
		free(locs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking Paris rail connections "
		       "(Lord Godalming, Round 1)\n" RESET);

		char *trail = "GPA....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
            false, true, false, &numLocs);

		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == MARSEILLES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking Adriatic Sea boat connections "
		       "(Lord Godalming, Round 1)\n" RESET);

		char *trail = "GAS....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
            false, false, true, &numLocs);

		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ADRIATIC_SEA);
		assert(locs[1] == BARI);
		assert(locs[2] == IONIAN_SEA);
		assert(locs[3] == VENICE);
		free(locs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;	}

	{///////////////////////////////////////////////////////////////////

		printf(YEL "Checking Szeged road connections "
		       "(Dracula, Round 1)\n" RESET);

		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..";

		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);

		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_DRACULA,
            true, false, false, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);
		free(locs);

		HvFree(hv);
		delay(i, speed);
		i++;
		printf(GRN "Test passed!\n" RESET);
		delay(i, speed);
		i++;
	}
	printf("\nTotal test time: %ld seconds", clock()/CLOCKS_PER_SEC);
	printf("\nVerdict: " GRN "Passed\n\n" RESET);
	return EXIT_SUCCESS;
}
