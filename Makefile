CFLAGS=-g -Wall -std=c++11 -pedantic
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes

testScreen:
	$(CC) "pruebas/Lesson3.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testScreen
	./testScreen
	
testDraw:
	$(CC) "pruebas/PruebaDibujar/main.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testDraw
	./testDraw
	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	rm -f testScreen
	rm -f testDraw

	
	
