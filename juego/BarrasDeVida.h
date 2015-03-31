#ifndef BARRASDEVIDA_H_INCLUDED
#define BARRASDEVIDA_H_INCLUDED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class BarraDeVida{
public:
    // 0 - 1000
    int vidaNumerica, staminaNumerica;
private:
    void ActualizarAnchos();

    bool muerto;
    bool izquierda;
    SDL_Renderer *renderer;


    int x_ini, x_fin, y_ini, y_fin;
    int anchoRectanguloInterior;
    SDL_Rect borde;
    SDL_Rect vida;
    SDL_Rect danio;
    SDL_Rect vacio;

    SDL_Rect staminaVerde, staminaRojo;


public:
    void Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam);
    void Dibujarse();
    void Lastimar(int porcentaje);

};




#endif // BARRASDEVIDA_H_INCLUDED
