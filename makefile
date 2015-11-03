main: main.o
	g++ -o main main.o -lpthread -lm

main.o: main.cpp
	g++ -c main.cpp -lpthread -lm
