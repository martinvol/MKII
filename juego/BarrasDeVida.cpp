#include "BarrasDeVida.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

void BarraDeVida::Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam){
    this->izquierda = izquierdaParam;
    this->muerto = false;
    this->vidaNumerica = 1000;
    this->staminaNumerica = 100;

    //this->anchoRectanguloInterior = x_fin - x_ini;
    this->renderer = rendererParam;

    //La mitad del 25% del ancho pasado es vacio.
    //int espacio = int( (0.25 * (x_final-x_inicial))/2 );
    int espacio = int((x_final-x_inicial)/8);

    this->x_ini = x_inicial + espacio;
    this->x_fin = x_final - espacio;

    //Comienza en el 10% de la pantalla
    this->y_ini = int(altoPantalla * 0.1f);
    //Usa 10% del alto.
    this->y_fin = y_ini+ int(altoPantalla * 0.1f);

    this->vida.x = x_ini;
    this->vida.y = y_ini;
    this->vida.w = (x_fin - x_ini);
    this->vida.h = (y_fin - y_ini);

    //Danio es como vida, pero empieza en 0.
    this->danio.x = x_ini;
    this->danio.y = y_ini;
    this->danio.w = 0;
    this->danio.h = (y_fin - y_ini);

    //Borde va por afuera
    this->borde.x = x_ini-1;
    this->borde.y = y_ini-1;
    this->borde.w = this->vida.w+2;
    this->borde.h = this->vida.h +2;

    this->vacio.x = x_ini;
    this->vacio.y = y_ini;
    this->vacio.w = (x_fin - x_ini);
    this->vacio.h = (y_fin - y_ini);

    this->anchoRectanguloInterior = this->vida.w;

    //La barra derecha carga el danio al reves
    if (izquierda == false){
        this->danio.x = x_fin;
        this->danio.y = y_ini;
        this->danio.w = 0;
        this->danio.h = (y_fin - y_ini);
    }
}

void BarraDeVida::Dibujarse(){
    ActualizarAnchos();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    //Dibujo el borde blanco
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 200 );
    SDL_RenderDrawRect(renderer, &(this->borde));

    if (muerto){
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        //Dibujo la barra de negro y salgo
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 200 );
        SDL_RenderFillRect(renderer, &(this->vacio));
        return;
    }
        //Barra de vida (Azul)
        SDL_SetRenderDrawColor( renderer, 0, 0, 255, 200 );
        SDL_RenderFillRect( renderer, &(this->vida) );

        //Barra de danio (Rojo)
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 200 );
        SDL_RenderFillRect( renderer, &(this->danio) );

}

void BarraDeVida::Lastimar(int porcentaje){

   if (vidaNumerica <= 0){
        muerto = true;
        ActualizarAnchos();
        Dibujarse();
   }else
        vidaNumerica -= porcentaje;
}

void BarraDeVida::ActualizarAnchos(){
    int anchoDeVidaEsperado = int((vidaNumerica* anchoRectanguloInterior)/1000);
    int diferencia = (vida.w -anchoDeVidaEsperado);
    //asi no permite vidas negativas con anchos de danio
    //mayores a los de anchoRectanguloInterior.
    if (vidaNumerica <0){
        muerto = true;
        return;
    }

    if (anchoDeVidaEsperado <= vida.w){

        if (izquierda == true){
            //desplazo el azul a la derecha
            //y lo achico para no salir del borde
            vida.x +=2;
            vida.w -=2;
            // crece el danio
            danio.w +=2;
        }else{
            //se corre el danio para izq
            //y crece
            danio.x -=2;
            danio.w +=2;
            // se achica la vida.
            vida.w -=2;
        }
    }
}
