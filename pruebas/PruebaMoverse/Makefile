CFLAGS=-g -Wall -std=c++11 -pedantic
JSONFLAFS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
FONTFLAGS = -lSDL2_ttf
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes

prueba_personaje:
	$(CC) "prueba_Personaje.cpp" "Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAFS) -o juego_ejecutable
	./juego_ejecutable
