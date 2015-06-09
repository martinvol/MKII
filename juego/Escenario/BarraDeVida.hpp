#ifndef BARRASDEVIDA_H_INCLUDED
#define BARRASDEVIDA_H_INCLUDED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../LTexture.hpp"
#include "../Logger/Logger.hpp"
// Música
#include <SDL2/SDL_mixer.h>

class BarraDeVida{
public:
    // 0 - 1000
    int vidaNumerica, staminaNumerica;
private:
	bool ganoRound;

    void ActualizarAnchos();
    void ActualizarAnchoDeVida(int anchoDeVidaEsperado);
    void ActualizarAnchoDeStamina(int anchoDeStaminaEsperado);

    bool muerto;
    bool izquierda;
    bool terminnoDeCansarse;
    SDL_Renderer *renderer;

    Mix_Chunk *pinia_sonido;

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
    
    string nombre;
    
    LTexture* medallaVictoria;
    SDL_Rect medallaRect;

	void dibujarNombre(string nombre, SDL_Rect* destino);

public:
    BarraDeVida(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam);
    ~BarraDeVida();
    void Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam);
	void setNombre(string nombre);
    void Dibujarse();
    void Lastimar(int porcentaje);
    void Cansar(int porcentaje);
    void Aliviar(int porcentaje);
    void GanoRound();
	void Resetear();
    bool seMurio();
    int VidaActual();

};

#endif // BARRASDEVIDA_H_INCLUDED
