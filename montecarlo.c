#include "cards.h"
#include "montecarlo.h"

Deck * constructMCDeck(Hand * hand){
	/* Constructs a deck of 47 cards using all but the cards in the hand */
	int i;
	Deck * d = buildDeck();
	for (i = 0; i < 5; i++){
		Card handCard = hand->cards[i];
		int pos = (handCard.suit * 13) + handCard.rank - 1 - i;
		swap((char *) d->cards,(char *) d->cards + (pos * sizeof(Card)), sizeof(Card));
		deal(d);
		shrinkDeck(d);
	}
	return d;
}

int runSimulation(int action, Hand hand, Deck deck){
	/* Runs a single simulation of the move indicated by int
	on the hand with the MC deck. Returns the score of the
	resulting hand */
	int i = 0;
	int j = 0;
	shuffleCards(deck.cards, 47);
	while (action != 0){
		/* Moves are looked at as binary representations of ints
		from 0 to 31. 00000 = 0, keep all cards. 00001 = 1,
		keep first card. 11111 = 31, discard all cards, etc. */
		int move = action & 1;
		action = action >> 1;
		if (move){
			hand.cards[i] = deck.cards[j]; 
			j++;
		}
		i++;
	}
	return handScore(&hand);
}

int monteCarloSimulation(Hand * hand, float results[32]){
	/* Runs DEFAULT_PRECISION similations on each hand and
	returns the int indicating the best move */
	Deck * deck = constructMCDeck(hand);
	/* Keep all cards */
	results[0] = handScore(hand);
	int max = 0;
	int i;
	/* All other possibilites.*/
	for (i = 1; i < 32; i++){
		int newresult = 0;
		int j;
		for (j = 0; j < DEFAULT_PRECISION; j++){
			newresult = runSimulation(i, *hand, *deck) + newresult;
		}
		results[i] = (float) newresult/(float) DEFAULT_PRECISION;
		if (results[i] > results[max]){
			max = i;
		}
	}
	destroyDeck(deck);
	return max;
}

void monteCarloAdvisor(Hand * hand){
	/* Advisor to print out information to human player */
	float results[32];
	int suggestedMove = monteCarloSimulation(hand, results);
	printf("The Monte Carlo advisor suggests:\n");
	if (suggestedMove){
		printf("Discarding ");
		if (suggestedMove & 1){
			printf("a");
			if (suggestedMove & 30){
				printf(",");
			}
		}
		if (suggestedMove & 2){
			printf("b");
			if (suggestedMove & 28){
				printf(",");
			}
		}
		if (suggestedMove & 4){
			printf("c");
			if (suggestedMove & 24){
				printf(",");
			}
		}
		if (suggestedMove & 8){
			printf("d");
			if (suggestedMove & 16){
				printf(",");
			}
		}
		if (suggestedMove & 16){
			printf("e");
		}
	}
	else {
		printf("Keeping all cards");
	}
	printf(" for an expected final hand score of %.2f. Your current hand score is %.0f.\n", results[suggestedMove], results[0]);
}