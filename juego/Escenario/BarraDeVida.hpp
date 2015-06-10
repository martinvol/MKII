#ifndef BARRASDEVIDA_H_INCLUDED
#define BARRASDEVIDA_H_INCLUDED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../LTexture.hpp"
#include "../Logger/Logger.hpp"
// Música
#include <SDL2/SDL_mixer.h>

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
    TTF_Font* fuente_nombre;
    SDL_Color color;
    SDL_Surface* superficie_nombre;
    SDL_Texture* textura_nombre;

    Mix_Chunk *pinia_sonido;

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

	void destruirTexturasNombre();

public:
	bool ganoRound;
	int x_ini, x_fin, y_ini, y_fin;
	SDL_Rect medallaRect;

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
