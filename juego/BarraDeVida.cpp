#include "BarraDeVida.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

void BarraDeVida::Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam){
    this->izquierda = izquierdaParam;
    this->muerto = false;
    this->terminnoDeCansarse = true;
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
    this->y_fin = y_ini+ int(altoPantalla * 0.07f);

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


    int anchoStamina = int((x_final-x_inicial)/4);
    int altoStamina = int((y_fin - y_ini)/2);
    staminaVerde = {x_ini, y_fin+1, anchoStamina, altoStamina};
    staminaRoja = {x_ini, y_fin+1, 0, altoStamina};

    staminaBorde= {x_ini-1, y_fin, anchoStamina+2, altoStamina+2};


    //La barra derecha carga el danio al reves
    if (izquierda == false){
        this->danio.x = x_fin;
        this->danio.y = y_ini;
        this->danio.w = 0;
        this->danio.h = (y_fin - y_ini);
        staminaVerde = {x_fin-anchoStamina, y_fin+1, anchoStamina, altoStamina};
        staminaRoja = {x_fin, y_fin+1, 0, altoStamina};
        staminaBorde= {x_fin-anchoStamina-1, y_fin, anchoStamina+2, altoStamina+2};
    }
    this->anchoRectanguloStaminaInterior = this->staminaVerde.w;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::Dibujarse(){
    ActualizarAnchos();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    //Dibujo el borde blanco para vida y stamina
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 200 );
    SDL_RenderDrawRect(renderer, &(this->borde));
    SDL_RenderDrawRect(renderer, &(this->staminaBorde));

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

        //Barra de stamina (verde)
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 230 );
        SDL_RenderFillRect( renderer, &(this->staminaVerde) );

        //Barra de stamina (rojo)
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 230 );
        SDL_RenderFillRect( renderer, &(this->staminaRoja) );

}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::Lastimar(int porcentaje){

   if (vidaNumerica <= 0){
        muerto = true;
        ActualizarAnchos();
        Dibujarse();
   }else
        vidaNumerica -= porcentaje;
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::Aliviar(int porcentaje){
    if (staminaNumerica < 100){
        staminaNumerica += porcentaje;
        terminnoDeCansarse = true;
    }
    //Analogo al cansancio.
    //un alivio de 50 con 90 de stamina --> 150 stamina
    //con esto fijo el maximo a 100.
    if (staminaNumerica > 100)
        staminaNumerica= 100;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::ActualizarAnchoDeVida( int anchoDeVidaEsperado){
    //ACTUALIZO LA VIDA.
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

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::ActualizarAnchoDeStamina(int anchoDeStaminaEsperado){
//ACTUALIZO STAMINAS
    if ((anchoDeStaminaEsperado <= staminaVerde.w)&& (!terminnoDeCansarse)){
        if (izquierda == true){
            //desplazo el verde a la derecha
            //y lo achico para no salir del borde
            staminaVerde.x +=1;
            staminaVerde.w -=1;
            // crece el danio
            staminaRoja.w +=1;
        }else{
            //se corre el rojo para izq
            //y crece
            staminaRoja.x -=1;
            staminaRoja.w +=1;
            // se achica la stamina.
            staminaVerde.w -=1;
        }
    }

    if(anchoDeStaminaEsperado > staminaVerde.w){
        if (izquierda == true){
            staminaVerde.x -=1;
            staminaVerde.w +=1;

            staminaRoja.w -=1;
        }else{
            staminaRoja.x +=1;
            staminaRoja.w -=1;

            staminaVerde.w +=1;
        }
    }
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::Cansar(int porcentaje){
    if (staminaNumerica > 0)
        staminaNumerica -= porcentaje;
    //ej. queda con 10 de stamina, y un ataque le saca 20
    //Estamina negativa = cansancio se sale de la barra.
    //Con esto la fijo siempre en cero
    if (staminaNumerica < 0)
        staminaNumerica=0;
    terminnoDeCansarse = false;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::ActualizarAnchos(){
    int anchoDeStaminaEsperado = int((staminaNumerica* anchoRectanguloStaminaInterior)/100);
    int diferenciaStamina = (staminaVerde.w - anchoDeStaminaEsperado);

    //para que se vacie la barra.
    if (diferenciaStamina <=2){
        terminnoDeCansarse = true;
    }

    int anchoDeVidaEsperado = int((vidaNumerica* anchoRectanguloInterior)/1000);
    //int diferencia = (vida.w -anchoDeVidaEsperado);
    //asi no permite vidas negativas con anchos de danio
    //mayores a los de anchoRectanguloInterior.
    if (vidaNumerica <0){
        muerto = true;
        return;
    }

    ActualizarAnchoDeVida(anchoDeVidaEsperado);
    ActualizarAnchoDeStamina(anchoDeStaminaEsperado);
}
