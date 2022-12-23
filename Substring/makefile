substring: main.o substring.o
	g++ main.o substring.o -o substring

main.o: main.cpp substring.h
	g++ -g -Wall -c main.cpp

substring.o: substring.cpp substring.h
	g++ -g -Wall -c substring.cpp

clean:
	rm -f *.o substring
