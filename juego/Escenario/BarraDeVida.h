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
    void ActualizarAnchoDeVida(int anchoDeVidaEsperado);
    void ActualizarAnchoDeStamina(int anchoDeStaminaEsperado);

    bool muerto;
    bool izquierda;
    bool terminnoDeCansarse;
    SDL_Renderer *renderer;


    int x_ini, x_fin, y_ini, y_fin;
    int anchoRectanguloInterior;
    int anchoRectanguloStaminaInterior;
    int crecimientoDeBarra;

    SDL_Rect borde;
    SDL_Rect vida;
    SDL_Rect danio;
    SDL_Rect vacio;
    SDL_Rect staminaBorde;

    SDL_Rect staminaVerde, staminaRoja;

public:
    void Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam);
    void Dibujarse();
    void Lastimar(int porcentaje);
    void Cansar(int porcentaje);
    void Aliviar(int porcentaje);


};




#endif // BARRASDEVIDA_H_INCLUDED
