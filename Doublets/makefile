doublets: main.o doublets.o
	g++ main.o doublets.o -o doublets

main.o: main.cpp doublets.h
	g++ -g -Wall -c -O3 main.cpp

doublets.o: doublets.cpp doublets.h
	g++ -g -Wall -c -O3 doublets.cpp

clean:
	rm -f *.o doublets
