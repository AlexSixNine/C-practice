stamp: main.o stamp.o
	g++ main.o stamp.o -o stamp -lcrypto

main.o: main.cpp stamp.h
	g++ -g -Wall -c main.cpp -lcrypto

stamp.o: stamp.cpp stamp.h
	g++ -g -Wall -c stamp.cpp -lcrypto

clean:
	rm -f *.o stamp
