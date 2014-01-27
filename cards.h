# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#ifndef CARDS_H
#define CARDS_H
/* Structs */
typedef enum suit{Hearts, Diamonds, Spades, Clubs} Suit;
typedef enum rank{Joker, Deuce, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace} Rank;
typedef enum {HighCard, OnePair, TwoPair, ThreeofKind, Straight, 
	Flush, FullHouse, FourofKind, StraightFlush, RoyalFlush} Class;

typedef struct {
	Suit suit;
	Rank rank;
} Card;

typedef struct {
	Card * originalCards;
	Card * cards;
	int front;
	int rear;
	int cardCount;
	int cardCapacity;
} Deck;

typedef struct{
	int score;
	Card cards[5];
	int value;
	Class class;
	Rank ranks[5];
} Hand;

/* Deck functions */
Deck * buildDeck(); //Builds a standard deck of 52 cards
void shrinkDeck(Deck *); // Reduces size of deck
Card * deal(Deck *); //Pop function, removes top card from deck and returns it
int addToDeck(Card *, Deck *); //Enqueue function,  adds a card to the bottom of a deck
void discardCard(Card *, Deck *); //Simultaneously adds discarded card to deck and deals new card to replace it.
void destroyDeck(Deck *); /* Frees deck pointers */
/* Generic swap function */
void swap(void *, void *, size_t);
/*Card functions */
int compareCards(const void *, const void *); //Compare function for cards in qsort
int equalRank(int, Card *); //Checks for num cards of equal rank starting at cards
void sortCards(Card * cards, int); //Sorts num cards in order of rank
void shuffleCards(Card *, int); //Shuffles given number of cards starting at cards
void printCards(Card *, int); //Prints given number of cards starting at cards
void printDeck(Deck *); //Prints a deck of cards
/* Hand functions */
void printHand(Hand *); //Prints a hand along with information about hand type
void assignRanks(Hand *); //Assigns ranks to hand for scoring
int handScore(Hand *); //Sorts, scores and assigns ranks for a hand
#endif