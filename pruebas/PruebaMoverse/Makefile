CFLAGS=-g -Wall -std=c++11 -pedantic
JSONFLAGS = -ljsoncpp
SDLFLAGS= -w -lSDL2
IMGFLAGS = -lSDL2_image
CC=g++

prueba_Personaje: Personaje.o Accion.o
	$(CC) "prueba_Personaje.cpp" Accion.o Personaje.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "prueba_Personaje"
	./prueba_Personaje
	make clean

Accion.o: Accion.cpp
	$(CC) "Accion.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c

Personaje.o: Personaje.cpp
	$(CC) "Personaje.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
pruebita.o: pruebita.cpp
	$(CC) "pruebita.cpp" $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) $(JSONFLAGS) -c
	
pruebita: pruebita.o
	$(CC)  pruebita.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "pruebita"
	
Personaje: Personaje.o Accion.o
	$(CC)  Personaje.o Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Personaje"

Accion: Accion.o
	$(CC)  Accion.o $(CFLAGS) $(SDLFLAGS) $(IMGFLAGS) -o "Accion"

clean:
	find . -name "*.o" -type f -delete
	find . -name "a.out" -type f -delete
	rm -f prueba_Personaje Accion pruebita Personaje core


