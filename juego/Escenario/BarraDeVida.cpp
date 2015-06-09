#include "BarraDeVida.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PATH_FONT_BARRA "resources/miscelaneo/nk57-monospace-ex-bd.ttf"
#define FONT_SIZE_BARRA 20

using namespace std;

BarraDeVida::BarraDeVida(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam){
	this->Inicializar(x_inicial, x_final, altoPantalla, rendererParam, izquierdaParam);
	medallaVictoria = new LTexture(this->renderer);
	medallaVictoria->loadFromFile("resources/miscelaneo/05.gif", 0, 0, 0,false);
	Logger* logger;
	logger =  Logger::instance();
	if(medallaVictoria == NULL){
			logger->log_debug("IMG_LoadTexture error: resources/1.png");			
	}
	this->nombre=" ";
	TTF_Init();
	this->fuente_nombre = TTF_OpenFont(PATH_FONT_BARRA, FONT_SIZE_BARRA);
	this->color = { 0, 0, 0, 0xFF };
	this->superficie_nombre = NULL;
	this->textura_nombre = NULL;
}

BarraDeVida::~BarraDeVida(){
	Mix_FreeChunk(pinia_sonido);
	delete medallaVictoria;
	if (superficie_nombre) destruirTexturasNombre();
	TTF_CloseFont(fuente_nombre);
}

void BarraDeVida::GanoRound(){
	this->ganoRound = true;
}

void BarraDeVida::setNombre(string nombre){
	this->nombre = nombre;
	if (superficie_nombre) destruirTexturasNombre();
	superficie_nombre = TTF_RenderText_Solid(fuente_nombre, nombre.c_str(), color);
	textura_nombre = SDL_CreateTextureFromSurface(renderer, superficie_nombre);
}

void BarraDeVida::destruirTexturasNombre(){
	SDL_FreeSurface(superficie_nombre);
	SDL_DestroyTexture(textura_nombre);
	superficie_nombre = NULL;
	textura_nombre = NULL;
}

void BarraDeVida::Inicializar(int x_inicial, int x_final, int altoPantalla, SDL_Renderer *rendererParam, bool izquierdaParam){
	this->ganoRound = false;
    this->izquierda = izquierdaParam;
    this->muerto = false;
    this->terminnoDeCansarse = true;
    this->vidaNumerica = 1000;
    this->staminaNumerica = 100;
    this->crecimientoDeBarra = int(altoPantalla * 0.01);

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
    this->borde.h = this->vida.h+2;

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
	
	//Rectangulo de medalla
	this->medallaRect.x = x_ini;
	this->medallaRect.y = staminaBorde.y + staminaBorde.h+1;
	this->medallaRect.w = staminaVerde.h;
    this->medallaRect.h = staminaVerde.h;
	

    //La barra derecha carga el danio al reves
    if (izquierda == false){
        this->danio.x = x_fin;
        this->danio.y = y_ini;
        this->danio.w = 0;
        this->danio.h = (y_fin - y_ini);
        staminaVerde = {x_fin-anchoStamina, y_fin+1, anchoStamina, altoStamina};
        staminaRoja = {x_fin, y_fin+1, 0, altoStamina};
        staminaBorde= {x_fin-anchoStamina-1, y_fin, anchoStamina+2, altoStamina+2};
        //tope derecho menos su ancho
        this->medallaRect.x = x_fin - medallaRect.w;
    }
    this->anchoRectanguloStaminaInterior = this->staminaVerde.w;

   pinia_sonido = Mix_LoadWAV("resources/music/punch.wav");
   

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
	
	if (ganoRound)
		SDL_RenderCopy(this->renderer, this->medallaVictoria->mTexture, NULL, &medallaRect);
	
    if (muerto){
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        //Dibujo la barra de negro y salgo
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 200 );
        SDL_RenderFillRect(renderer, &(this->vacio));

        SDL_RenderCopy(renderer, textura_nombre, NULL, &(this->vacio));
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
        
        SDL_Rect destino = {this->vida.x, this->vida.y, this->vida.w + this->danio.w, this->vida.h};
        SDL_RenderCopy(renderer, textura_nombre, NULL, &destino);
        
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
void BarraDeVida::Lastimar(int porcentaje){

   if (vidaNumerica <= 0){
        muerto = true;
        ActualizarAnchos();
        Dibujarse();
   }else{
        vidaNumerica -= porcentaje;
    }
    Mix_PlayChannel(-1, pinia_sonido, 0);

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
    if (anchoDeVidaEsperado < vida.w){
        if (izquierda == true){
            //desplazo el azul a la derecha
            //y lo achico para no salir del borde
            vida.x +=crecimientoDeBarra;
            vida.w -=crecimientoDeBarra;
            // crece el danio
            danio.w +=crecimientoDeBarra;
        }else{
            //se corre el danio para izq
            //y crece
            danio.x -=crecimientoDeBarra;
            danio.w +=crecimientoDeBarra;
            // se achica la vida.
            vida.w -=crecimientoDeBarra;
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
            staminaVerde.x +=2;
            staminaVerde.w -=2;
            // crece el danio
            staminaRoja.w +=2;
        }else{
            //se corre el rojo para izq
            //y crece
            staminaRoja.x -=2;
            staminaRoja.w +=2;
            // se achica la stamina.
            staminaVerde.w -=2;
        }
    }

    if(anchoDeStaminaEsperado > staminaVerde.w){
        if (izquierda == true){
            staminaVerde.x -=2;
            staminaVerde.w +=2;

            staminaRoja.w -=2;
        }else{
            staminaRoja.x +=2;
            staminaRoja.w -=2;

            staminaVerde.w +=2;
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
    if (diferenciaStamina <=3){
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

bool BarraDeVida::seMurio(){
    return muerto;
}

void BarraDeVida::Resetear(){
	this->muerto = false;
	this->vidaNumerica = 1000;
	
	this->vida.x = x_ini;    
    this->vida.w = (x_fin - x_ini);
    
    this->danio.x = x_ini;    
    this->danio.w = 0;
	
	
	if (izquierda == false){
        this->danio.x = x_fin;        
        this->danio.w = 0;        
	}
}

int BarraDeVida::VidaActual(){
		return this->vidaNumerica;
}
