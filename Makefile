CFLAGS=-g -Wall -std=c++11 -pedantic -O3
JSONFLAGS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
FONTFLAGS = -lSDL2_ttf
JSONTEST = "pruebas/integracion.json"
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes

testScreen: clean LTexture.o
	$(CC) "pruebas/PixelTest.cpp" *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o testScreen
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

Capa.o: juego/Escenario/Capa.cpp
	$(CC) "juego/Escenario/Capa.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Escenario.o: juego/Escenario/Escenario.cpp
	$(CC) "juego/Escenario/Escenario.cpp"  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Parser.o: juego/Parser/Parser.cpp
	$(CC) "juego/Parser/Parser.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

BarraDeVida.o: juego/Escenario/BarraDeVida.cpp
	$(CC) "juego/Escenario/BarraDeVida.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c


coordenadas.o: juego/Coordenadas/CoordenadaLogica.cpp juego/Coordenadas/CoordenadaFisica.cpp juego/Coordenadas/ConversorDeCoordenadas.cpp
	$(CC) "juego/Coordenadas/CoordenadaLogica.cpp" $(CFLAGS) -c
	$(CC) "juego/Coordenadas/CoordenadaFisica.cpp" $(CFLAGS) -c
	$(CC) "juego/Coordenadas/ConversorDeCoordenadas.cpp" $(CFLAGS) -c

Director.o: juego/Director/Director.cpp
	$(CC) "juego/Director/Director.cpp" $(CFLAGS) -c

Logger.o: juego/Logger/Logger.cpp
	$(CC) "juego/Logger/Logger.cpp" $(CFLAGS) -c

LTexture.o:
	$(CC) "juego/LTexture.cpp" $(CFLAGS) -c

Accion.o: juego/Personaje/Acciones/Accion.cpp
	$(CC) "juego/Personaje/Acciones/Accion.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Quieto.o: juego/Personaje/Acciones/Quieto.cpp
	$(CC) "juego/Personaje/Acciones/Quieto.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Girar.o: juego/Personaje/Acciones/Girar.cpp
	$(CC) "juego/Personaje/Acciones/Girar.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

CubrirAlto.o: juego/Personaje/Acciones/CubrirAlto.cpp
	$(CC) "juego/Personaje/Acciones/CubrirAlto.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
CubrirBajo.o: juego/Personaje/Acciones/CubrirBajo.cpp
	$(CC) "juego/Personaje/Acciones/CubrirBajo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
SaltarVertical.o: juego/Personaje/Acciones/SaltarVertical.cpp
	$(CC) "juego/Personaje/Acciones/SaltarVertical.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
SaltarDiagonal.o: juego/Personaje/Acciones/SaltarDiagonal.cpp
	$(CC) "juego/Personaje/Acciones/SaltarDiagonal.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Caminar.o: juego/Personaje/Acciones/Caminar.cpp
	$(CC) "juego/Personaje/Acciones/Caminar.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Agacharse.o: juego/Personaje/Acciones/Agacharse.cpp
	$(CC) "juego/Personaje/Acciones/Agacharse.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
		
PiniaAlta.o: juego/Personaje/Acciones/PiniaAlta.cpp
	$(CC) "juego/Personaje/Acciones/PiniaAlta.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PiniaBaja.o: juego/Personaje/Acciones/PiniaBaja.cpp
	$(CC) "juego/Personaje/Acciones/PiniaBaja.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PatadaAlta.o: juego/Personaje/Acciones/PatadaAlta.cpp
	$(CC) "juego/Personaje/Acciones/PatadaAlta.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PatadaBajaAgachado.o: juego/Personaje/Acciones/PatadaBajaAgachado.cpp
	$(CC) "juego/Personaje/Acciones/PatadaBajaAgachado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PatadaBaja.o: juego/Personaje/Acciones/PatadaBaja.cpp
	$(CC) "juego/Personaje/Acciones/PatadaBaja.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Estado.o: juego/Personaje/Estado.cpp
	$(CC) "juego/Personaje/Estado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Arrojable.o: juego/Personaje/Acciones/Arrojable.cpp
	$(CC) "juego/Personaje/Acciones/Arrojable.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c	

Personaje.o: juego/Personaje/Personaje.cpp
	$(CC) "juego/Personaje/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Estado: Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Agacharse.o PiniaAlta.o PiniaBaja.o PatadaAlta.o  PatadaBajaAgachado.o PatadaBaja.o Girar.o CubrirAlto.o CubrirBajo.o Accion.o 
	$(CC)  Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Agacharse.o Accion.o PiniaAlta.o PiniaBaja.o Girar.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Estado"

Timer.o: 
	$(CC) "juego/Escenario/Timer.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -c
	
Accion: Accion.o
	$(CC)  Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Accion"


Quieto: Accion.o Quieto.o
	$(CC)  Accion.o Quieto.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Quieto"

Girar: Accion.o Girar.o
	$(CC)  Accion.o Girar.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Girar"

CubrirAlto: Accion.o CubrirAlto.o
	$(CC)  Accion.o CubrirAlto.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "CubrirAlto"

CubrirBajo: Accion.o CubrirBajo.o
	$(CC)  Accion.o CubrirBajo.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "CubrirBajo"

Agacharse: Accion.o Agacharse.o
	$(CC)  Accion.o Agacharse.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Agacharse"

SaltarVertical: Accion.o SaltarVertical.o
	$(CC)  Accion.o  SaltarVertical.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "SaltarVertical"

Caminar: Accion.o Caminar.o
	$(CC)  Accion.o  Caminar.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Caminar"
	
SaltarDiagonal: Accion.o SaltarDiagonal.o
	$(CC)  Accion.o  SaltarDiagonal.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "SaltarDiagonal"

PiniaAlta: Accion.o PiniaAlta.o
	$(CC)  Accion.o  PiniaAlta.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PiniaAlta"

PiniaBaja: Accion.o PiniaBaja.o
	$(CC)  Accion.o  PiniaBaja.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PiniaBaja"

PatadaAlta: Accion.o PatadaAlta.o
	$(CC)  Accion.o  PatadaAlta.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PatadaAlta"

PatadaBajaAgachado: Accion.o PatadaBajaAgachado.o
	$(CC)  Accion.o  PatadaBajaAgachado.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PatadaBajaAgachado"

PatadaBaja: Accion.o PatadaBaja.o
	$(CC)  Accion.o  PatadaBaja.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PatadaBaja"

Personaje: Personaje.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o Personaje.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Personaje"


Jugador.o: juego/Personaje/Jugador.cpp
	$(CC) "juego/Personaje/Jugador.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c


Ventana.o: juego/Escenario/Ventana.cpp
	$(CC) "juego/Escenario/Ventana.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Rectangulo.o: juego/Personaje/Acciones/Rectangulo.cpp
	$(CC) "juego/Personaje/Acciones/Rectangulo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

compilar_juego: Arrojable.o LTexture.o  Rectangulo.o main.o Capa.o Escenario.o Parser.o BarraDeVida.o Logger.o Accion.o Personaje.o coordenadas.o Estado.o Quieto.o Caminar.o SaltarVertical.o SaltarDiagonal.o Agacharse.o PiniaAlta.o PiniaBaja.o PatadaAlta.o PatadaBajaAgachado.o PatadaBaja.o Girar.o CubrirAlto.o CubrirBajo.o Director.o Jugador.o Ventana.o Timer.o
	$(CC) *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -lm -o juego_ejecutable

juego: compilar_juego
	./juego_ejecutable ${jsonpath}
	make clean
	
juegoJsonTest: compilar_juego
	./juego_ejecutable $(JSONTEST)
	make clean

prueba_personaje:
	$(CC) "pruebas/PruebaMoverse/prueba_Personaje.cpp" "pruebas/PruebaMoverse/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -o juego_ejecutable
	
prueba_integracionI: clean Capa.o Escenario.o Parser.o coordenadas.o Logger.o
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
