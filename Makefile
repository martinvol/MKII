CFLAGS=-g -Wall -std=c++11 -pedantic
JSONFLAGS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
FONTFLAGS = -lSDL2_ttf
JSONTEST = "pruebas/integracion.json"
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

BarraDeVida.o: juego/BarraDeVida.cpp
	$(CC) "juego/BarraDeVida.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Coordenadas.o: juego/CoordenadaLogica.cpp juego/CoordenadaFisica.cpp juego/ConversorDeCoordenadas.cpp
	$(CC) "juego/CoordenadaLogica.cpp" $(CFLAGS) -c
	$(CC) "juego/CoordenadaFisica.cpp" $(CFLAGS) -c
	$(CC) "juego/ConversorDeCoordenadas.cpp" $(CFLAGS) -c

Director.o: juego/Director.cpp
	$(CC) "juego/Director.cpp" $(CFLAGS) -c

logger.o: juego/logger.c
	$(CC) "juego/logger.c" $(CFLAGS) -c

Accion.o: juego/Accion.cpp
	$(CC) "juego/Accion.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Quieto.o: juego/Quieto.cpp
	$(CC) "juego/Quieto.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
SaltarVertical.o: juego/SaltarVertical.cpp
	$(CC) "juego/SaltarVertical.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
SaltarDiagonal.o: juego/SaltarDiagonal.cpp
	$(CC) "juego/SaltarDiagonal.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Caminar.o: juego/Caminar.cpp
	$(CC) "juego/Caminar.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
		
Estado.o: juego/Estado.cpp
	$(CC) "juego/Estado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Personaje.o: juego/Personaje.cpp
	$(CC) "juego/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Estado: Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Accion.o
	$(CC)  Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Estado"

Timer.o: 
	$(CC) "juego/timer.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -c
	
Accion: Accion.o
	$(CC)  Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Accion"

Quieto: Accion.o Quieto.o
	$(CC)  Accion.o Quieto.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Quieto"

SaltarVertical: Accion.o SaltarVertical.o
	$(CC)  Accion.o  SaltarVertical.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "SaltarVertical"

Caminar: Accion.o Caminar.o
	$(CC)  Accion.o  Caminar.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Caminar"
	
SaltarDiagonal: Accion.o SaltarDiagonal.o
	$(CC)  Accion.o  SaltarDiagonal.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "SaltarDiagonal"


Personaje: Personaje.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o Personaje.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Personaje"



Jugador.o: juego/Jugador.cpp
	$(CC) "juego/Jugador.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c


compilar_juego: main.o Capa.o Escenario.o parser.o BarraDeVida.o logger.o Accion.o Personaje.o Coordenadas.o Estado.o Quieto.o Timer.o Caminar.o SaltarVertical.o SaltarDiagonal.o
	$(CC) *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -lm -o juego_ejecutable

juego: compilar_juego
	./juego_ejecutable ${jsonpath}
	# make clean
	
juegoJsonTest: compilar_juego
	./juego_ejecutable $(JSONTEST)
	make clean

prueba_personaje:
	$(CC) "pruebas/PruebaMoverse/prueba_Personaje.cpp" "pruebas/PruebaMoverse/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -o juego_ejecutable
	
prueba_integracionI: clean Capa.o Escenario.o parser.o Coordenadas.o logger.o
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
