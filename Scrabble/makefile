scrabble: main.o scrabble.o
	g++ main.o scrabble.o -o scrabble

main.o: main.cpp scrabble.h
	g++ -g -Wall -c main.cpp

scrabble.o: scrabble.cpp scrabble.h
	g++ -g -Wall -c scrabble.cpp

clean:
	rm -f *.o scrabble
