# include "cards.h"

Deck * buildDeck(){
	/* Builds a deck of 52 cards */
	int i, j;
	Deck * d = (Deck *) malloc(sizeof(Deck));
	d->originalCards = (Card *) malloc(52 * sizeof(Card));
	d->cards = d->originalCards;
	for (i = 0; i < 4; i++){
		for (j = 1; j <= 13; j++){
			int pos = (i * 13) + j - 1;
			d->cards[pos].suit = i;
			d->cards[pos].rank = j;
		}
	}
	d->front = 0;
	d->cardCapacity = 52;
	d->cardCount = 52;
	d->rear = d->cardCapacity;
	return d;
}

void shrinkDeck(Deck * deck){
	/* Reduce deck size and adjust pointers */
	deck->cards++;
	deck->cardCapacity--;
	deck->front--;
	deck->rear = deck->cardCapacity;
}

Card * deal(Deck * d){
	/* Pop from queue */
	if (d->front == d->cardCapacity){
		d->front = 0;
		d->rear = d->cardCapacity;
	}
	Card temp  = d->cards[d->front];
	Card * result = &temp;
	d->cards[d->front].rank = 0;
	d->cardCount--;
	d->front++;
	return result;
}

int addToDeck(Card * toAdd, Deck * drawDeck){
	/* Enqueue */
	if (drawDeck->rear == drawDeck->cardCapacity){
		drawDeck->rear = 0;
	}
	if (drawDeck->cards[drawDeck->rear].rank != 0){
		return 1;
	}
	else {
		drawDeck->cards[drawDeck->rear] = *toAdd;
		drawDeck->rear++;
		drawDeck->cardCount++;
	}
	return 0;
}

void discardCard(Card * toDiscard, Deck * drawDeck){
	/* Enqueues toDiscard and deals a new card in its place */
	Card * temp = malloc(sizeof(Card));
	*temp = *toDiscard;
	*toDiscard = *deal(drawDeck);
	addToDeck(temp, drawDeck);
	free(temp);
}

void destroyDeck(Deck * d){
	/* Frees deck pointers */
	free(d->originalCards);
	free(d);
	d = NULL;
}

void swap(void * a, void * b, size_t sz){
	/* Generic swap function */
	char * temp = (char *) malloc(sz);
	memcpy(temp, a, sz);
	memcpy(a, b, sz);
	memcpy(b, temp, sz);
	free(temp);
}

int compareCards(const void * val1, const void * val2){
	/* Compares two cards, uses suit arbitrarily 
	to ensure consistent ordering */
	Card * card1 = (Card *) val1;
	Card * card2 = (Card *) val2;
	int result = card1->rank - card2->rank;
	if (result == 0) {
		result = card1->suit - card2->suit;
	}
	return result;
}

int equalRank(int num, Card* cards){
	/* Returns true (1) if num cards starting at pointer cards 
	are equal in rank, returns false (0) otherwise */
	int i;
	for (i = 1; i < num; i++){
		if (cards[i-1].rank != cards[i].rank){
			return 0;
		}
	}
	return 1;
}

void sortCards(Card * cards, int num){
	/* Sorts cards using qsort with compareCards function*/
	qsort(cards, num, sizeof(Card), compareCards);
}

void shuffleCards(Card * cards, int num){
	/* Shuffles num cards starting at cards pointer */
	int i = 0;
	for (i = 0; i < num; i++){
		int remainingIndices = num - i;
		int switchIndex = i + (rand() % remainingIndices);
		swap((char *) cards + (i * sizeof(Card)),(char *) cards + (switchIndex * sizeof(Card)), sizeof(Card));
 	}
}

void printCards(Card * cards, int num){
	/* Prints num cards starting at cards pointer */
	char* suitNames [] = {"Hearts", "Diamonds", "Spades", "Clubs"};
	char* rankNames [] = {NULL, "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
	int i = 0;
	while (i < num){
		printf("%s of %s\n", rankNames[cards[i].rank], suitNames[cards[i].suit]);
		i++;
	}
}

void printDeck(Deck * deck) {
	/* Prints a deck in order */
	printCards(&deck->cards[deck->front], deck->cardCapacity - deck->front);
	if(deck->rear <= deck->front){
		printCards(deck->cards, deck->rear);
	}
}

/* HAND FUNCTIONS */
void printHand(Hand * hand){
	/* Prints a hand */
	char* rankNames [] = {NULL, "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
	char* class[10] = {"High card", "One pair", "Two pair", 
		"Three of a kind", "Straight", "Flush", "Full house", 
		"Four of a kind", "Straight flush", "Royal flush"};
	char alph[5] = {'a','b','c','d','e'};
	int i;
	for (i = 0; i < 5; i++){
		printf("%c - ", alph[i]);
		printCards(&hand->cards[i], 1);
	}
	printf("Hand type: %s - %s\n", class[hand->class], rankNames[hand->ranks[0]]);
}

void assignRanks(Hand * hand){
	/* Assign ranks to hand for scoring */
	if (hand->class == 7 || hand->class == 6){
		/* Four of a kind or full house */
		/* Top rank is most common card */
		hand->ranks[0] = hand->cards[2].rank;
    	/* Check if most common card is lowest or highest */
    	if (hand->cards[2].rank != hand->cards[0].rank){
    		/* If highest, second rank is lowest card */
    		hand->ranks[1] = hand->cards[0].rank;
    	}
    	else {
    		/* If lowest, second rank is highest card */
    		hand->ranks[1] = hand->cards[4].rank;
    	}
    	/* No other ranks needed */
    	hand->ranks[2] = 0;
    	hand->ranks[3] = 0;
    	hand->ranks[4] = 0;
	}
	else if (hand->class == 2 || hand->class == 3){
		/* Two pair or three of a kind */
		if (hand->class == 3){
			/* If three of a kind, card at index two must be
			ordinal rank */
			hand->ranks[0] = hand->cards[2].rank;
			/* If item at index one has a different rank, the three
			of a kind rank is the largest card in the hand */
			if (hand->cards[2].rank != hand->cards[1].rank){
				hand->ranks[1] = hand->cards[1].rank;
				hand->ranks[2] = hand->cards[0].rank;
			}
			/* If item at index three has a different rank, the three
			of a kind rank is the smallest card in the hand */
			else if (hand->cards[2].rank != hand->cards[3].rank){
				hand->ranks[1] = hand->cards[4].rank;
				hand->ranks[2] = hand->cards[3].rank;
			}
			/* Three of a kind rank is the middle card */
			else {
				hand->ranks[1] = hand->cards[4].rank;
				hand->ranks[2] = hand->cards[0].rank;
			}
		}
		else if (hand->class == 2) {
			/* Two pair */
			/* Is the top card part of a pair? */
			if (hand->cards[4].rank == hand->cards[3].rank){
				/* If so, set it to top rank */
				hand->ranks[0] = hand->cards[4].rank;
				/* Look for second pair */
				if (hand->cards[2].rank == hand->cards[1].rank){
					/* If items one and two are a pair, set ranks */
					hand->ranks[1] = hand->cards[2].rank;
					hand->ranks[2] = hand->cards[0].rank;
				}
				else {
					/* Else items zero and one are a pair */
					hand->ranks[1] = hand->cards[1].rank;
					hand->ranks[2] = hand->cards[2].rank;
				}
			}
			else {
				/* Items three and two are a pair, items
				zero and one are a pair */
				hand->ranks[0] = hand->cards[3].rank;
				hand->ranks[1] = hand->cards[1].rank;
				hand->ranks[2] = hand->cards[4].rank;
			}
		}
		hand->ranks[3] = 0;
		hand->ranks[4] = 0;
	}
	else if (hand->class == 1){
		int nopair = 1;
		int i = 0;
		while (nopair) {
			if (hand->cards[i].rank == hand->cards[i + 1].rank){
				nopair = 0;
				hand->ranks[0] = hand->cards[i].rank;
			}
			i++;
		}
		/* pair is at 3, 4*/
		if (i == 4){
			hand->ranks[1] = hand->cards[2].rank;
			hand->ranks[2] = hand->cards[1].rank;
			hand->ranks[3] = hand->cards[0].rank;
		}
		/* pair is at 2, 3 */
		if (i == 3){
			hand->ranks[1] = hand->cards[4].rank;
			hand->ranks[2] = hand->cards[1].rank;
			hand->ranks[3] = hand->cards[0].rank;
		}
		/* pair is at 1, 2 */
		if (i == 2){
			hand->ranks[1] = hand->cards[4].rank;
			hand->ranks[2] = hand->cards[3].rank;
			hand->ranks[3] = hand->cards[0].rank;
		}
		/* pair is at 0, 1 */
		if (i == 1){
			hand->ranks[1] = hand->cards[4].rank;
			hand->ranks[2] = hand->cards[3].rank;
			hand->ranks[3] = hand->cards[2].rank;
		}
		hand->ranks[4] = 0;
	}
	else {
		if ((hand->class == 8 || hand->class == 4) && 
			(hand->cards[3].rank == 4 && hand->cards[4].rank == 13)){
			/* Straight or straight flush with ace low */
			hand->ranks[4] = 0;
			hand->ranks[0] = hand->cards[3].rank;
			hand->ranks[1] = hand->cards[2].rank;
			hand->ranks[2] = hand->cards[1].rank;
			hand->ranks[3] = hand->cards[0].rank;
		}
		else {
			/* Straights, flushes, high cards */
			hand->ranks[0] = hand->cards[4].rank;
			hand->ranks[1] = hand->cards[3].rank;
			hand->ranks[2] = hand->cards[2].rank;
			hand->ranks[3] = hand->cards[1].rank;
			hand->ranks[4] = hand->cards[0].rank;
		}
	}
}

int handScore(Hand * hand){
	sortCards(hand->cards, 5);
    int isFlush = 1;
    int i;
    for (i = 1; i < 5; ++i){
    	if (hand->cards[i].suit != hand->cards[0].suit){
    		isFlush = 0;
    	}
    }
    /* Four of a kind */
    if (equalRank(4, &hand->cards[0]) || equalRank(4, &hand->cards[1])){
    	hand->class = 7;
    }
    /* Full house */
    else if ((equalRank(3, &hand->cards[0]) && equalRank(2, &hand->cards[3])) || (equalRank(2, &hand->cards[0]) && equalRank(3, &hand->cards[2]))){
    	hand->class = 6;
    }
    /* Three of a kind */
    else if (equalRank(3, &hand->cards[0]) || equalRank(3, &hand->cards[1]) || equalRank(3, &hand->cards[2])){
    	hand->class = 3;
    }
    /* Two pair */
    else if ((equalRank(2, &hand->cards[0]) && (equalRank(2, &hand->cards[2]) || equalRank(2, &hand->cards[3]))) || 
    		(equalRank(2, &hand->cards[1]) && equalRank(2, &hand->cards[3]))) {
    	hand->class = 2;
    }
    else { /* At most one pair */
    	int isStraight = 0;
    	hand->class = 0;
    	/*Check for pairs */
    	for (i = 0; i < 4; i++){
    		if (equalRank(2, &hand->cards[i])) {
    			hand->class = 1;
    		}
    	}
    	/* Check for straights, including ace-low straights */
    	if (hand->class == 0 && (hand->cards[4].rank - hand->cards[0].rank == 4 
    			|| (hand->cards[4].rank == 13 && ((hand->cards[3].rank - hand->cards[0].rank == 3) && hand->cards[0].rank == 1)))) {
    		isStraight = 1;
    	}
    	/* Royal flush */
    	if ((isFlush && isStraight) && hand->cards[3].rank == 12){
    		hand->class = 9;
    	}
    	/* Straight flush */
    	else if (isFlush && isStraight){
    		hand->class = 8;
    	}
    	/* Flush */
    	else if (isFlush){
    		hand->class = 5;
    	}
    	/* Straight */
    	else if (isStraight){
    		hand->class = 4;
    	}
    }
    assignRanks(hand);
	hand->score = (13 * hand->class) + hand->ranks[0];
	return hand->score;
}