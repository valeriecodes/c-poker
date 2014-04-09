
In order the run the game, run "make" to compile, and then "./game" to run the executable. There will be text prompts to guide you through the game. In a betting round, betting less than the call amount or entering -1 will result in a fold. Entering 0 ("checking") after a bet has been placed in a round is an illegal move and will result in problems with the player's bet tally.

All functions are described in header files and commented in source code.

Source files are organized as follows:
Card, Deck and Hand data structs and functions related to them are in the cards.c file.
Monte Carlo simulation functions are in the montecarlo.c file.
Player and Game data structs and fuctions related to them, including all the functions for printing the game to the command line, are in the players.c file.
The game.c file just consists of a main to seed rand and to call gameLoop(), which handles the game process.

The deck queue is implemented circularly in an array format. That is, after element 0 is popped, the front pointer is moved to element one, and the new last element can be put at element 0. I have used the function deckShrink() to reduce the size of the deck when a card is drawn that will not be replaced (i.e. after the inital deal). 
Precision of Monte Carlo can be dialed up or down by changing the DEFAULT_PRECISION value in montecarlo.h. I have it set to 10000, which took <5 seconds per hand on my computer, 100000 took ~25 seconds.

Sources:
The C Programming Language (Kernighan & Ritchie)
Class Piazza page
Lecture notes
Monte Carlo paper on Dropbox
Talked to Steve about Monte Carlo implementation
http://en.wikipedia.org/wiki/Enumerated_type
http://en.wikipedia.org/wiki/Five-card_draw
http://en.wikipedia.org/wiki/List_of_poker_hands
http://en.wikipedia.org/wiki/Betting_in_poker
http://www.cplusplus.com/reference/
http://www.cs.bu.edu/teaching/c/queue/array/types.html
http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
