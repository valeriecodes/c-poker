game: game.o cards.o montecarlo.o players.o
	gcc -Wall -o game game.o players.o montecarlo.o cards.o -lm

cards.o: cards.c cards.h
	gcc -Wall -c cards.c -I.

montecarlo.o: montecarlo.c cards.h montecarlo.h
	gcc -Wall -c montecarlo.c -I.

players.o: players.c montecarlo.h players.h cards.h
	gcc -Wall -c players.c -I. -lm

game.o: game.c players.h
	gcc -Wall -c game.c -I.

clean:
	rm -f *.o game