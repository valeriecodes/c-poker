#include "cards.h"
#define DEFAULT_PRECISION 10000 /*Can be dialed up or down, depending on 
								accuracy desired */

Deck * constructMCDeck(Hand *); /* Constructs 47-card deck with all cards not in
								 the hand */
int runSimulation(int, Hand, Deck); /* Runs a single simulation of move indicated 
									by int and returns score of resulting hand */
int monteCarloSimulation(Hand *, float[]); /* Runs simulations for all possible 
									moves and returns int indicating best move */
void monteCarloAdvisor(Hand *); /*Advises human player based on output of Monte
								 Carlo simulation. */