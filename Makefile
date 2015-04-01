CFLAGS=-g -Wall -std=c++11 -pedantic
JSONFLAGS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
FONTFLAGS = -lSDL2_ttf
JSONTEST = "pruebas/json/test.json"
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
	
testTiempo: clean
	$(CC) "pruebas/testTiempo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(FONTFLAGS) -o testTiempo
	./testTiempo
	make clean	

main.o: juego/main.cpp
	$(CC) "juego/main.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Capa.o: juego/Capa.cpp
	$(CC) "juego/Capa.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Escenario.o: juego/Escenario.cpp
	$(CC) "juego/Escenario.cpp"  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

parser.o: juego/parser.cpp
	$(CC) "juego/parser.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

BarrasDeVida.o: juego/BarrasDeVida.cpp
	$(CC) "juego/BarrasDeVida.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

coordenadas.o: source/CoordenadaLogica.cpp source/CoordenadaFisica.cpp source/ConversorDeCoordenadas.cpp
	$(CC) "source/CoordenadaLogica.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	$(CC) "source/CoordenadaFisica.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	$(CC) "source/ConversorDeCoordenadas.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

logger.o: juego/logger.c
	$(CC) "juego/logger.c" $(CFLAGS) -c

juego: main.o Capa.o Escenario.o parser.o BarrasDeVida.o logger.o
	$(CC) main.o Capa.o Escenario.o parser.o  BarrasDeVida.o logger.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -lm -o juego_ejecutable
	./juego_ejecutable ${jsonpath}
	# make clean
	
juegoJsonTest: main.o Capa.o Escenario.o parser.o BarrasDeVida.o
	$(CC) *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -lm -o juego_ejecutable
	./juego_ejecutable $(JSONTEST)

prueba_personaje:
	$(CC) "pruebas/PruebaMoverse/prueba_Personaje.cpp" "pruebas/PruebaMoverse/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -o juego_ejecutable
	
prueba_integracionI: clean Capa.o Escenario.o parser.o coordenadas.o
	$(CC) "pruebas/IntegracionI.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	$(CC) *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -lm -o integracionI 	
	./integracionI
	make clean
	
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete
	rm -f juego_ejecutable testScreen testDraw testFloor testVida testTiempo integracionI

commit: clean
	git add -u
	git add .
	git commit
	git push
