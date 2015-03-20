CFLAGS=-g -Wall -std=c++11 -pedantic
SDLFLAGS= -w -lSDL2
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes

testScreen:
	$(CC) "pruebas/test_grafico.c" $(SDLFLAGS) -o testScreen
	./testScreen
	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	rm -f testScreen

	
	
