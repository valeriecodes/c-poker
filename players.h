# include "cards.h"
# include <math.h>

typedef struct{
	char * name;
	float money;
	Hand * hand;
	int isHuman;
	float bet;
} Player;

typedef struct {
	Deck * draw;
	Player * players;
	int playerCount;
	float bet;
	float pot;
} Game;

/* Player functions */
void destroyPlayer(Player *); /* Frees pointer for player's hand */
void printPlayers(Player *, int); /*Prints players along with their hand and money 
								amounts (used for debugging)*/
void makeMove(Player *, Deck *, void *); /* Have specified player make a move 
								indicated either by and integer or a char string */
int compareHands(const void *, const void *); /* Compare function for qsort, 
								compares players by value of hands */
void sortPlayers(Player *, int); /* Sorts players by value of hands */
/*Game functions */
Game * beginGame(char * [], float []); /* Constructs game struct, creates deck 
										and player pointers */
int removePlayer(Game *, int); /* Removes a player from the game */
void destroyGame(Game *); /* Frees all game-related pointers */
void printGame(Game *); /* Prints deck, players and hands (used for debugging) */
void showdown(Game *); /* Selects a winner or winners from players and 
						distributes winnings */
void newRound(Game *); /*Clears bet, cards, and pot, starting a new round with the same 
						players and money amounts from the previous round */
void takeTurn(Player *, Game *); /* Runs Monte Carlo and makes appointed move or asks for input*/
void makeBet(Player *, Game *, int); /* Makes or asks for player input in 
						betting decision and updates appropriate variables*/
void betRound(Game *, int); /* Runs a round of betting for all players */
void gameRound(Game *); /* Runs a full round of game, including first betting round, 
						discard round, second betting round and showdown */
void gameLoop(); /* Game loop, including intro and prompts */