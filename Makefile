CFLAGS=-g -Wall -std=c++11 -pedantic
JSONFLAFS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
FONTFLAGS = -lSDL2_ttf
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
	$(CC) "pruebas/barrasDeVida.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(FONTFLAGS) -o testVida
	./testVida
	make clean
	

main.o: juego/main.cpp
	$(CC) "juego/main.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -c

Capa.o: juego/Capa.cpp
	$(CC) "juego/Capa.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -c

Escenario.o: juego/Escenario.cpp
	$(CC) "juego/Escenario.cpp"  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -c

parser.o: juego/parser.cpp
	$(CC) "juego/parser.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -c

juego: main.o Capa.o Escenario.o parser.o
	$(CC) main.o Capa.o Escenario.o parser.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -lm -o juego_ejecutable
	./juego_ejecutable ${jsonpath}
	# make clean

prueba_personaje:
	$(CC) "pruebas/PruebaMoverse/prueba_Personaje.cpp" "pruebas/PruebaMoverse/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -o juego_ejecutable
	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete
	rm -f juego_ejecutable
	rm -f testScreen
	rm -f testDraw
	rm -f testFloor
	rm -f testVida

commit: clean
	git add -u
	git commit
	git push
