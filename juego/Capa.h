#ifndef CAPA_H_INCLUDED
#define CAPA_H_INCLUDED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
using namespace std;

class Capa{
private:
    string ubicacion;
    double anchoLogico;
    SDL_Renderer *ren;
    double x_logico;
public:
    Capa (string ubicacionParam, double anchoLogicoParam, double x_logicoParam, SDL_Renderer *rendererParam);
    ~Capa ();
    void Dibujarse(int x, int y);
    void Dibujarse(int x, int y, int alto, int ancho);
    SDL_Texture* CargarTextura();

    SDL_Texture* textura;
};

//----------------------------------------------------------------
//----------------------------------------------------------------
//--------------------CONTENEDORES DE COORDENADAS-----------------
//------------------------Dani despues dice como -----------------
//--------------------------se transforman------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
class CoordenadasFisicas{
public:
    int x_fisico;
    int y_fisico;
};
//----------------------------------------------------------------
class CoordenadasLogicas{
public:
    double x_logico;
    double y_logico;
};
#endif // CAPA_H_INCLUDED
