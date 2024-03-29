CFLAGS= -g -Wall -std=c++11 -pedantic 
JSONFLAGS = -ljsoncpp
SDLFLAGS= -w -lSDL2 -lSDL2_mixer -lSDL2
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
	
testGrilla: clean GrillaDeJugadores.o LTexture.o Accion.o Quieto.o coordenadas.o Rectangulo.o Logger.o
	$(CC) "pruebas/testGrilla.cpp" *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(FONTFLAGS) -o testScreen
	./testScreen
	make clean	

Menu.o: juego/Menu/Menu.cpp juego/Menu/ControladorMenu.cpp
	$(CC) "juego/Menu/Menu.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	$(CC) "juego/Menu/ControladorMenu.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

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

DirectorPractica.o: juego/Director/DirectorPractica.cpp juego/Director/DirectorPractica.hpp Director.o
	$(CC) "juego/Director/DirectorPractica.cpp" $(CFLAGS) -c

Logger.o: juego/Logger/Logger.cpp
	$(CC) "juego/Logger/Logger.cpp" $(CFLAGS) -c

LTexture.o: juego/LTexture.cpp
	$(CC) "juego/LTexture.cpp" $(CFLAGS) -c

Accion.o: juego/Personaje/Acciones/Accion.cpp
	$(CC) "juego/Personaje/Acciones/Accion.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PiniaAgachado.o: juego/Personaje/Acciones/PiniaAgachado.cpp
	$(CC) "juego/Personaje/Acciones/PiniaAgachado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

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

Traba.o: juego/Personaje/Acciones/Traba.cpp
	$(CC) "juego/Personaje/Acciones/Traba.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

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
	
PatadaAltaAgachado.o: juego/Personaje/Acciones/PatadaAltaAgachado.cpp
	$(CC) "juego/Personaje/Acciones/PatadaAltaAgachado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PatadaBaja.o: juego/Personaje/Acciones/PatadaBaja.cpp
	$(CC) "juego/Personaje/Acciones/PatadaBaja.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
PiniaEnAire.o: juego/Personaje/Acciones/PiniaEnAire.cpp
	$(CC) "juego/Personaje/Acciones/PiniaEnAire.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

RecibirGolpeBajo.o: juego/Personaje/Acciones/RecibirGolpeBajo.cpp
	$(CC) "juego/Personaje/Acciones/RecibirGolpeBajo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

RecibirGolpeAgachado.o: juego/Personaje/Acciones/RecibirGolpeAgachado.cpp
	$(CC) "juego/Personaje/Acciones/RecibirGolpeAgachado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Levantarse.o: juego/Personaje/Acciones/Levantarse.cpp
	$(CC) "juego/Personaje/Acciones/Levantarse.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

LevantarseDelGancho.o: juego/Personaje/Acciones/LevantarseDelGancho.cpp
	$(CC) "juego/Personaje/Acciones/LevantarseDelGancho.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

RecibirTraba.o: juego/Personaje/Acciones/RecibirTraba.cpp
	$(CC) "juego/Personaje/Acciones/RecibirTraba.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

RecibirGolpeAlto.o: juego/Personaje/Acciones/RecibirGolpeAlto.cpp
	$(CC) "juego/Personaje/Acciones/RecibirGolpeAlto.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PatadaSaltoDiagonal.o: juego/Personaje/Acciones/PatadaSaltoDiagonal.cpp
	$(CC) "juego/Personaje/Acciones/PatadaSaltoDiagonal.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c	

PatadaSaltoVertical.o: juego/Personaje/Acciones/PatadaSaltoVertical.cpp
	$(CC) "juego/Personaje/Acciones/PatadaSaltoVertical.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
SiendoArrojado.o: juego/Personaje/Acciones/SiendoArrojado.cpp
	$(CC) "juego/Personaje/Acciones/SiendoArrojado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
ArrojarProyectil.o: juego/Personaje/Acciones/ArrojarProyectil.cpp
	$(CC) "juego/Personaje/Acciones/ArrojarProyectil.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Gancho.o: juego/Personaje/Acciones/Gancho.cpp
	$(CC) "juego/Personaje/Acciones/Gancho.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

RoundKick.o: juego/Personaje/Acciones/RoundKick.cpp
	$(CC) "juego/Personaje/Acciones/RoundKick.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c	

Ganar.o:
	$(CC) "juego/Personaje/Acciones/Ganar.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Dizzy.o:
	$(CC) "juego/Personaje/Acciones/Dizzy.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Desaparecer.o:
	$(CC) "juego/Personaje/Acciones/Desaparecer.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Morir.o:
	$(CC) "juego/Personaje/Acciones/Morir.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Fatality.o:
	$(CC) "juego/Personaje/Acciones/Fatality.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Perform.o:
	$(CC) "juego/Personaje/Acciones/Perform.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Estado.o: juego/Personaje/Estado.cpp
	$(CC) "juego/Personaje/Estado.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Arrojable.o: juego/Personaje/Acciones/Arrojable.cpp
	$(CC) "juego/Personaje/Acciones/Arrojable.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Latigo.o: juego/Personaje/Acciones/Latigo.cpp
	$(CC) "juego/Personaje/Acciones/Latigo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Personaje.o: juego/Personaje/Personaje.cpp
	$(CC) "juego/Personaje/Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) $(FONTFLAGS) -c
	
GrillaDeJugadores.o: 
	$(CC) "juego/Menu/GrillaDeJugadores.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

TextBoxes.o:
	$(CC) "juego/Menu/TextBox.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	$(CC) "juego/Menu/ControladorTextBox.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

PanelBotones.o: juego/Personaje/PanelBotones.cpp juego/Personaje/PanelBotones.hpp
	$(CC) "juego/Personaje/PanelBotones.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) $(FONTFLAGS) -c

AI.o:
	$(CC) "juego/AI/AI.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
Estado: Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Agacharse.o RecibirTraba.o Traba.o RecibirGolpeAgachado.o LevantarseDelGancho.o Levantarse.o PiniaAlta.o PiniaBaja.o PiniaAgachado.o PatadaAlta.o  PatadaBajaAgachado.o PatadaAltaAgachado.o PatadaBaja.o Girar.o CubrirAlto.o CubrirBajo.o Accion.o PiniaEnAire.o PatadaSaltoDiagonal.o Ganar.o
	$(CC)  Estado.o Quieto.o SaltarVertical.o SaltarDiagonal.o Caminar.o Agacharse.o Accion.o PiniaAlta.o PiniaBaja.o Girar.o PatadaSaltoDiagonal.o PiniaEnAire.o Ganar.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Estado"

Timer.o: juego/Escenario/Timer.cpp
	$(CC) "juego/Escenario/Timer.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -c
	
Accion: Accion.o
	$(CC)  Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Accion"


Quieto: Accion.o Quieto.o 
	$(CC)  Accion.o Quieto.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Quieto"

Girar: Accion.o Girar.o
	$(CC)  Accion.o Girar.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Girar"

CubrirAlto: Accion.o CubrirAlto.o
	$(CC)  Accion.o CubrirAlto.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "CubrirAlto"

RecibirTraba: Accion.o RecibirTraba.o
	$(CC)  Accion.o RecibirTraba.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "RecibirTraba"

CubrirBajo: Accion.o CubrirBajo.o
	$(CC)  Accion.o CubrirBajo.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "CubrirBajo"

Agacharse: Accion.o Agacharse.o
	$(CC)  Accion.o Agacharse.o  $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Agacharse"

SaltarVertical: Accion.o SaltarVertical.o
	$(CC)  Accion.o  SaltarVertical.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "SaltarVertical"

Caminar: Accion.o Caminar.o
	$(CC)  Accion.o  Caminar.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Caminar"

PiniaAgachado: Accion.o PiniaAgachado.o
	$(CC)  Accion.o  PiniaAgachado.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PiniaAgachado"
	
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

PatadaAltaAgachado: Accion.o PatadaAltaAgachado.o
	$(CC)  Accion.o  PatadaAltaAgachado.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PatadaAltaAgachado"

PatadaBaja: Accion.o PatadaBaja.o
	$(CC)  Accion.o  PatadaBaja.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "PatadaBaja"

Personaje: Personaje.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o Personaje.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Personaje"

Traba: Traba.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o Traba.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Traba"

RecibirGolpeAgachado: RecibirGolpeAgachado.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o RecibirGolpeAgachado.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "RecibirGolpeAgachado"

Levantarse: Levantarse.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o Levantarse.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Levantarse"

LevantarseDelGancho: LevantarseDelGancho.o Accion.o	Estado.o
	$(CC)   Accion.o Estado.o LevantarseDelGancho.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "LevantarseDelGancho"

Jugador.o: juego/Personaje/Jugador.cpp
	$(CC) "juego/Personaje/Jugador.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Ventana.o: juego/Escenario/Ventana.cpp
	$(CC) "juego/Escenario/Ventana.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Rectangulo.o: juego/Personaje/Acciones/Rectangulo.cpp
	$(CC) "juego/Personaje/Acciones/Rectangulo.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

compilar_juego: Arrojable.o LTexture.o GrillaDeJugadores.o TextBoxes.o Rectangulo.o Menu.o main.o Capa.o Escenario.o Parser.o RecibirTraba.o BarraDeVida.o Logger.o Accion.o Personaje.o coordenadas.o Estado.o Quieto.o Caminar.o SaltarVertical.o SaltarDiagonal.o Agacharse.o PiniaAlta.o PiniaBaja.o Traba.o PatadaAlta.o PatadaBajaAgachado.o PatadaAltaAgachado.o  PatadaBaja.o Girar.o CubrirAlto.o CubrirBajo.o Director.o Jugador.o Ventana.o Timer.o PiniaEnAire.o PatadaSaltoDiagonal.o PatadaSaltoVertical.o Gancho.o RoundKick.o LevantarseDelGancho.o Levantarse.o RecibirGolpeAgachado.o RecibirGolpeBajo.o RecibirGolpeAlto.o SiendoArrojado.o PiniaAgachado.o ArrojarProyectil.o AI.o DirectorPractica.o PanelBotones.o Ganar.o Morir.o Dizzy.o Desaparecer.o Fatality.o Perform.o Latigo.o
	$(CC) *.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) $(FONTFLAGS) -lm -o juego_ejecutable

juego: compilar_juego
	./juego_ejecutable ${jsonpath}
	# make clean
	
juegoVolpe: juego
	make juego jsonpath=resources/volpe.json

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
