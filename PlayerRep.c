// Check if in Dracula's Trail
// Input: player (Dracula), the abbreviated location
// Output: NOWHERE if not on the trail, DoubleBack ID if double back was 
    // done at that location, Hide location if done at that location, PlaceId
    // otherwise.
PlaceId PlayerRepCheckTrail(PlayerRep player, const char *LocationAbb)
{
    int i = 0;
    int checkDouble = -1;
    int checkHide = -1;
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
            player->HunterPOVTrail[i] = strdup(player->trail[i]);
        } else {
            i++;
            checkDouble--;
            checkHide--;
        }
    }
    return NOWHERE;
}
