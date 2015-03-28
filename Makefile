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

testVida: clean
	$(CC) "pruebas/barrasDeVida.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testVida
	./testVida
	make clean
	
juego: clean
	$(CC) "juego/main.cpp" "juego/cargadorDeEscenario.cpp" "juego/funcionesDibujar.cpp" $(SDLFLAGS) $(IMGFLAGS) -o juego_ejecutable
	./juego_ejecutable
	make clean

	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete
	rm -f juego_ejecutable
	rm -f testScreen
	rm -f testDraw
	rm -f testFloor

	
	
