CFLAGS=-g -Wall -std=c++11 -pedantic
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes

testScreen: clean
	$(CC) "pruebas/Lesson3.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testScreen
	./testScreen
	make clean
	
testDraw: clean
	$(CC) "pruebas/PruebaDibujar/main.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testDraw
	./testDraw
	make clean
	
testFloor: clean
	$(CC) "pruebas/Lesson5.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testFloor
	./testFloor
	make clean
	
juego: clean
	$(CC) "Demo 01/main.cpp" "Demo 01/cargadorDeEscenario.cpp" "Demo 01/funcionesDibujar.cpp" $(SDLFLAGS) $(IMGFLAGS) -o juego
	./juego
	make clean

	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete
	rm -f juego
	rm -f testScreen
	rm -f testDraw
	rm -f testFloor

	
	
