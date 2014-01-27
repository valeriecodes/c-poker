# include "montecarlo.h"
# include "players.h"

/* PLAYER FUNCTIONS */
void destroyPlayer(Player * player){
	free(player->hand);
	player->hand = NULL;
}

void printPlayers(Player * players, int num){
	int i;
	for (i = 0; i < num; i++){
		printf("Name: %s\n", players[i].name);
		printf("Money: %.2f\n", players[i].money);
		printf("Hand:\n");
		printHand(players[i].hand);
		printf("\n\n");
	}
}

void makeMove(Player * player, Deck * deck, void * move){
	/* Takes either a string ("a,b,c,d,e") for human player 
	or an int (0-31) for computer player indicating cards to discard*/
	if (!player->isHuman){
		int moveInt = *((int *) move);
		if (moveInt & 1){
			discardCard(&player->hand->cards[0], deck);	
		}
		if (moveInt & 2){
			discardCard(&player->hand->cards[1], deck);	
		}
		if (moveInt & 4){
			discardCard(&player->hand->cards[2], deck);	
		}
		if (moveInt & 8){
			discardCard(&player->hand->cards[3], deck);
		}
		if (moveInt & 16){
			discardCard(&player->hand->cards[4], deck);	
		}
		int i = 0;
		while (moveInt > 0){
			if (moveInt & 1){
				i++;
			}
			moveInt = moveInt >> 1;
		}
		printf("%s discarded %d cards\n", player->name, i);
	}
	else {
		char* moveString = (char *) move;
		char* mv = strtok(moveString, ",");
		while(mv){
			if (strcmp(mv, "0") == 0){
				break;
			}
			else if (strcmp(mv, "a") == 0){
				discardCard(&player->hand->cards[0], deck);
			}
			else if (strcmp(mv, "b") == 0){
				discardCard(&player->hand->cards[1], deck);	
			}
			else if (strcmp(mv, "c") == 0){
				discardCard(&player->hand->cards[2], deck);	
			}
			else if (strcmp(mv, "d") == 0){
				discardCard(&player->hand->cards[3], deck);	
			}
			else if (strcmp(mv, "e") == 0){
				discardCard(&player->hand->cards[4], deck);	
			}
			else {
				printf("Invalid input: %s. Skipping.\n", mv);
			}
			mv = strtok(NULL,",");
		}
	}
	handScore(player->hand);
}

int compareHands(const void * val1, const void * val2){
	/* Compares players by hand values */
	Player * player1 = (Player *) val1;
	Player * player2 = (Player *) val2;
	int result = player1->hand->class - player2->hand->class;
	int i = 0;
	while (result == 0){
		if (i == 5){
			return 0;
		}
		else {
			result = player1->hand->ranks[i] - player2->hand->ranks[i];
		}
		i++;
	}
	return result;
}

void sortPlayers(Player * players, int num){
	/* Sorts players by hand values */
	qsort(players, num, sizeof(Player), compareHands);
}

/* GAME FUNCTIONS */
Game * beginGame(char * names[], float moneyAmounts[]){
	/* Initializes a game */
	int humans[4] = {1, 0, 0, 0};
	Game * game = (Game *) malloc(sizeof(Game));
	game->playerCount = 4;
	game->draw = buildDeck();
	game->bet = 0;
	game->pot = 0;
	shuffleCards(game->draw->cards, game->draw->cardCount);
	game->players = (Player *) malloc(game->playerCount * sizeof(Player));
	int i, j;
	for (i = 0; i < game->playerCount; i++){
		Player * currentPlayer = &(game->players[i]);
		currentPlayer->name = names[i];
		currentPlayer->money = moneyAmounts[i];
		currentPlayer->hand = (Hand *) malloc(sizeof(Hand));
		currentPlayer->isHuman = humans[i];
		currentPlayer->bet = 0;
		Hand * currentHand = currentPlayer->hand;
		for (j = 0; j < 5; j++){
			currentHand->cards[j] = *(deal(game->draw));
			shrinkDeck(game->draw);
		}
		handScore(currentHand);
	}
	return game;
}

int removePlayer(Game * game, int playerNum){
	/* Removes player at game->players[playerNum] while
	preserving other player pointers (for when a 
	player folds or runs out of money)*/
	int lastPlayer = game->playerCount-1;
	int i;
	if (playerNum > lastPlayer){
		printf("Attempting to remove nonexistent player");
		return 1;
	}
	/* Shift player down until they are in last player pointer */
	for (i = playerNum; i < lastPlayer; i++){
		swap(&game->players[i], &game->players[i + 1], sizeof(Player));
	}
	/* Free their card pointer and decrement player count */
	destroyPlayer(&game->players[lastPlayer]);
	game->playerCount = lastPlayer;
	return 0;
}

void destroyGame(Game * game){
	/* Frees all game related pointers. */
	int i;
	for (i = 0; i < game->playerCount; i++){
		destroyPlayer(&game->players[i]);
	}
	destroyDeck(game->draw);
	free(game->players);
	free(game);
	game = NULL;
}

void printGame(Game * game){
	/* Prints deck, players and their hands
	(used for debugging, not in final game)*/
	printf("Players:\n\n");
	printPlayers(game->players, game->playerCount);
	printf("Deck:\n");
	printDeck(game->draw);
}

void showdown(Game * g){
	/* Decides winner and distributes pot */
	Player * winningOrder = malloc(sizeof(Player) * g->playerCount);
	memcpy(winningOrder, g->players, sizeof(Player) * g->playerCount);
	sortPlayers(winningOrder, g->playerCount);
	printf("\nFinal hands:\n");
	int i, j;
	for (i = 0; i < g->playerCount; i++){
		printf("%s's hand:\n", winningOrder[i].name);
		printHand(winningOrder[i].hand);
		printf("Score: %d\n", winningOrder[i].hand->score);
		printf("\n");
	}
	/* i is "non-winner count", for every matching hand at the end 
	of winning order it is decremented */
	i = g->playerCount-1;
	while (compareHands(&winningOrder[i-1], &winningOrder[i]) == 0){
		i--;
		if (i == 0){
			break;
		}
	}
	int winners = g->playerCount - i;
	if (winners > 1){
		printf("It's a %d-way tie!\n", winners);
	}
	for (i = 0; i < winners; i++){
		printf("%s wins the game\n", winningOrder[g->playerCount - 1 - i].name);
		/* Translate from the "winningOrder" player pointer back to the game player pointer */
		for (j = 0; j < g->playerCount; j++){
			if (strcmp(g->players[j].name, winningOrder[g->playerCount - 1 - i].name) == 0){
				(&g->players[j])->money = (&g->players[j])->money + (g->pot/winners);
			}
		}
	}
	free(winningOrder);
	/* Remove players who are out of money */
	for (i = 0; i < g->playerCount; i++){
		if (g->players[i].money == 0){
			removePlayer(g, i);
			i--;
		}
	}
}

void newRound(Game * g){
	/* Reset pot, hands, bets and deck, but leave players and money */
	g->pot = 0;
	g->bet = 0;
	destroyDeck(g->draw);
	g->draw = buildDeck();
	shuffleCards(g->draw->cards, g->draw->cardCount);
	int i, j;
	for (i = 0; i < g->playerCount; i++){
		Player * currentPlayer = &(g->players[i]);
		Hand * currentHand = currentPlayer->hand;
		currentPlayer->bet = 0;
		for (j = 0; j < 5; j++){
			currentHand->cards[j] = *(deal(g->draw));
			shrinkDeck(g->draw);
		}
		handScore(currentHand);
	}
}

void takeTurn(Player * player, Game * game){
	/* One turn in the discard round for the player.
	Prints prompts and runs monteCarloAdvisor for human, calls
	function to make the best move for computer. */
	char move[10];;
	if (player->isHuman){
		printHand(player->hand);
		monteCarloAdvisor(player->hand);
		printf("Enter the letters of cards you wish to discard, separated by commas with no spaces (e.g. a,b), enter 0 to keep all cards:\n");
		scanf("%s", move);
		makeMove(player, game->draw, &move);
	}
	else {
		float results[32];
		int mcMove = monteCarloSimulation(player->hand, results);
		makeMove(player, game->draw, &mcMove);
	}
}

void makeBet(Player * player, Game * g, int roundNum){
	/* One turn in the betting round for player. Asks for input from
	human player, makes a randomized guess for computer. */
	float bet;
	float currentBet = g->bet;
	if (player->isHuman){
		printf("It's time to place bets. Here are your cards:\n");
		printHand(player->hand);
		if (currentBet == 0){
			printf("How much would you like to bet? (%.1f to go all in, 0 to check)\n", player->money);
		}
		else if (player->bet != 0){
			printf("How much would you like to bet (including your current bet of %.1f)? (%.1f to go all in, %.1f to call, -1 to fold)\n", player->bet, player->money + player->bet, currentBet);
		}
		else{
			printf("How much would you like to bet? (%.1f to go all in, %.1f to call, 0 to check, -1 to fold)\n", player->money, currentBet);			
		}
		scanf("%f", &bet);
	}
	else {
		/* Betting logic for computers */
		float riskyness = 1 + (float) rand()/ (float) (RAND_MAX);
		float handGoodness;
		handGoodness = (float) handScore(player->hand)/130;
		float betPercent = handGoodness * riskyness;
		if (betPercent >= 1){
			bet = player->money + player->bet;
		}
		else {
			/* Rounding to the nearest .5 */
			bet = round(betPercent * (player->money + player->bet) * 2);
			bet = bet/2;
			/* In first round of betting be more cautious */
			if (roundNum == 1){
				if (bet > 2 * currentBet){
					bet = currentBet + .5;
				}
				else if (bet > currentBet && currentBet != 0){
					bet = currentBet;
				}
			}
		}
	}
	if (bet > player->money){
		bet = player->money - player->bet;
	}
	if (player->isHuman && bet == 0){
		printf("%s checks.\n", player->name);
	}
	else if (bet >= currentBet){
		if (bet == player->money - player->bet){
			player->money = 0;
			printf("%s goes all in.\n", player->name);
		}
		else {
			player->money = player->money - (bet - player->bet);
			if (bet == currentBet){
				printf("%s calls.\n", player->name);
			}
			else if (currentBet == 0){
				printf("%s opens with %.2f.\n", player->name, bet);
			}
			else{
				printf("%s raises to %.2f\n", player->name, bet);
			}
		}
		g->pot = g->pot + (bet - player->bet);
		player->bet = bet;
		if (player->bet > g->bet){
			g->bet = player->bet;
		}
	}
	else {
		printf("%s folds.\n", player->name);
		player->bet = -1;
	}
}

void betRound(Game * g, int roundNum){
	/* Runs a round of betting for the game */
	int i;
	int finished = 0;
	int opened = 0;
	g->bet = 0;
	for (i = 0; i < g->playerCount; i++){
		g->players[i].bet = 0;
	}
	/* Continue looping until all players have 
	had the chance to bet and all bets match */
	while (!opened || !finished){
		finished = 1;
		for (i = 0; i < g->playerCount; i++){
			float oldBet = g->bet;
			/* If a player hasn't had a chance to bet, hasn't folded, or has not called a raise */
			if (!opened || (g->players[i].bet != g->bet && g->players[i].bet != -1)){
				makeBet(&g->players[i], g, roundNum);
			}
			/* If there is a raise, run the loop again */
			if (g->bet > oldBet && oldBet != 0){
				finished = 0;
			}
			/* If a player checks, run the loop again */ 
			if (g->players[i].bet == 0){
				finished = 0;
			}
		}
		if (g->bet != 0){
			opened = 1;
		}
		/* Remove players who folded */
		for (i = 0; i < g->playerCount; i++){
			if (g->players[i].bet == -1){
				removePlayer(g, i);
				i--;
			}
		}
	}
}

void gameRound(Game * g){
	/* Runs a full round of the game after intialization,
	including one betting round, one discard round, a
	second betting round and a showdown. Offers an
	option for a second round if players remain at the end */
	printf("Here are all of the players:\n");
	int i;
	for (i = 0; i < g->playerCount; ++i){
		printf("Name: %s\n", g->players[i].name);
		printf("Money: %.2f\n", g->players[i].money);
	}
	printf("\nLet's start the betting.\n");
	betRound(g, 1);
	if (g->playerCount == 1){
		printf("Game over, %s wins\n", g->players[0].name);
		return;
	}
	for (i = 0; i < g->playerCount; i++){
		takeTurn(&g->players[i], g);
	}
	printf("Second betting round:\n");
	betRound(g, 2);
	if (g->playerCount == 1){
		printf("Game over, %s wins\n", g->players[0].name);
		return;
	}
	showdown(g);
	if (g->playerCount > 1){
		printf("Would you like to play another round? (y/n)\n");
		char decision;
		scanf(" %c", &decision);
		if (decision == 'y'){
			newRound(g);
			gameRound(g);
		}
	}
}

void gameLoop(){
	/* The outer game loop. Handles intros and initialization. Offers the 
	option to start a new game after the previous one has finished. */
	char name[40];
	printf("Hello and welcome to Valerie's poker table!\n");
	printf("Enter your name and let's get started: (No spaces)\n");
	scanf("%s", name);
	while (strcmp(name, "Watson") == 0) {
		printf("Oops, that's one of the computer names. Pick a different one:\n");
		scanf(" %s", name);
	}
	printf("Welcome %s!\n", name);
	printf("\n\n");
	printf("Let's start the game...\n");
	char * names[4] = {name, "Deep Blue", "Watson", "HAL 9000"};
	float money[4] = {10, 10, 10, 10};
	Game * g = beginGame(names, money);
	gameRound(g);
	destroyGame(g);
	char decision;
	printf("Would you like to start a new game? (y/n)\n");
	scanf(" %c", &decision);
	if (decision == 'y'){
		gameLoop();
	}
}