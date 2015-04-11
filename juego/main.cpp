#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "BarraDeVida.hpp"
#include "Capa.hpp"
#include "Escenario.hpp"
#include "Parser.hpp"
#include "Personaje.hpp"
#include "Logger.hpp"
#include "ConversorDeCoordenadas.hpp"

using namespace std;


#define QUIETO 0
#define CAMINAR_DERECHA 1
#define CAMINAR_IZQUIERDA 2
#define SALTAR 3
Logger *logger = Logger::instance();


//----------------------------------------------------------------

int InicializarSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->log_error("Error al iniciar SDL");
        return 1;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        logger->log_error("Error con inicializacion de imagenes");
        SDL_Quit();
        return 1;
    }
    logger->log_debug("SDL cargada correctamente");
    return 0;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == NULL){
        logger->log_error("LoadTexture");
    }
    return texture;
}
//----------------------------------------------------------------
//----------------------------------------------------------------

//---------------------------------------------------
//-----------------------MAIN------------------------
//---------------------------------------------------


class Juego
{
public:
    int argc;
    char** argv;
    ConversorDeCoordenadas* conv;

    bool salir = false;
    SDL_Renderer * renderer = NULL;

    Escenario *escenario;
    BarraDeVida barraDeVida1, barraDeVida2;

    unsigned int ANCHO_FISICO, ALTO_FISICO;
    unsigned int AnchoLogico, AltoLogico;

    int mover;
    int moverSZ;
    SDL_Window * window = NULL;

    Parser *conf;

    Personaje *personajeJuego;
    
    Juego(int argc_, char* argv_[]){
        argc = argc_;
        argv = argv_;
    };

    int jugar(){

        if (InicializarSDL() != 0) return 1;
        renderer = SDL_CreateRenderer(NULL, -1, 0);
        configurar();

        game_loop();

        // LIBERAR RECURSOS
        terminar_juego();
        terminar_sdl();
        return 0;
    };

    void cargar_configuracion(){
        this->conf = new Parser();
        conf->set_values(argv[1]);
        // Se settean configuraciones (con el json)
        // Esto tiene que cambiarse cuando se aprieta la letra R

		this->escenario = new Escenario(conf->escenario_ancho, conf->escenario_alto);

        //Pantalla
        ANCHO_FISICO = conf->ventana_anchopx; //800
        ALTO_FISICO = conf->ventana_altopx; //416
        //Mundo
        AnchoLogico = conf->escenario_ancho;
        AltoLogico = conf->escenario_alto;
        // Martin
        // fin de las configuraciones

    };

    void configurar(){
        cargar_configuracion();

        window = SDL_CreateWindow("Mortal Kombat 3 Ultimate",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   ANCHO_FISICO, ALTO_FISICO,
                                   SDL_WINDOW_MAXIMIZED);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        cargar_capas();

        //Izquierda
        barraDeVida1.Inicializar(0, ANCHO_FISICO/2, ALTO_FISICO, renderer, true);
       //Derecha
        barraDeVida2.Inicializar(ANCHO_FISICO/2, ANCHO_FISICO, ALTO_FISICO, renderer, false);
        CoordenadaLogica* coord_personaje = new CoordenadaLogica(1,1);
        Personaje* personaje = new Personaje(coord_personaje,"Subzero",renderer, conf->personaje_ancho, conf->personaje_alto);
        this->personajeJuego = personaje;
    }

    void cargar_capas(){

        for (unsigned int i = 0; i < conf->capas_vector.size(); i++){
            conf->capas_vector[i]->ren = renderer;

            // escenario->AgregarCapa(conf->capas_vector[i]); Por algÃºn motivo esto no anda

            escenario->AgregarCapa( // esto no deberÃ­a estar asÃ­, tendria que andar la lÃ­nea de arriba, pero estuve luchando y no la hago andar (maxi)
                new Capa (conf->capas_vector[i]->ubicacion,
                conf->capas_vector[i]->anchoLogico,
                conf->capas_vector[i]->x_logico,
                conf->capas_vector[i]->ren
                )
            );
        }
    }

    void game_loop(){

        bool golpeandoPJ = false;
        bool cansandoPJ = false;
        bool scrollearDerecha = false;
        bool scrollearIzquierda = false;
        bool saltando = false;
        bool saltoDiagonalIZQ = false;
        bool saltoDiagonalDER = false;
        mover = 5;
        moverSZ= 1;
        int posicionPJ_Piso = 125;
        double t = 1.0;
        SDL_Event evento;
        SDL_Texture *under = loadTexture("resources/background/p_under.png", renderer);
        SDL_Rect r = {0, 0, ALTO_FISICO, ANCHO_FISICO};

        while (!salir){
            SDL_PollEvent( &evento );
            switch(evento.type){
                case SDL_QUIT:
                    salir = true;
                    break;
                case SDL_KEYDOWN:
                    if (evento.key.keysym.sym == SDLK_UP)  {
                        saltando = true;
                        this->personajeJuego->definir_imagen(QUIETO);
                        scrollearDerecha = false;
                        scrollearIzquierda = false;
                    }
                    if ((evento.key.keysym.sym == SDLK_x)){
                        saltoDiagonalDER = true;
                        this->personajeJuego->definir_imagen(QUIETO);
                        scrollearDerecha = false;
                        scrollearIzquierda = false;
                    }

                    if ((evento.key.keysym.sym == SDLK_z)){ //Ver ControladorDeEventos para multiples teclas
                        saltoDiagonalIZQ = true;
                        this->personajeJuego->definir_imagen(QUIETO);
                        scrollearDerecha = false;
                        scrollearIzquierda = false;
                    }
                    if (evento.key.keysym.sym == SDLK_RIGHT)  {
                        scrollearDerecha = true;
                        scrollearIzquierda = false;
                        this->personajeJuego->definir_imagen(CAMINAR_DERECHA);

                    }
                    if ((evento.key.keysym.sym == SDLK_LEFT) && (mover <0) )  {
                        scrollearIzquierda = true;
                        scrollearDerecha = false;
                        this->personajeJuego->definir_imagen(CAMINAR_IZQUIERDA);
                    }
                    if(evento.key.keysym.sym == SDLK_a)  {
                        barraDeVida1.Aliviar(20);
                        barraDeVida2.Aliviar(20);
                    }
                    if(evento.key.keysym.sym == SDLK_c)  {
                        if (cansandoPJ == false){
                            barraDeVida1.Cansar(50);
                            barraDeVida2.Cansar(50);
                            cansandoPJ = true;
                        }
                    }
                    if((evento.key.keysym.sym == SDLK_d))  {
                        if (golpeandoPJ == false){
                            barraDeVida1.Lastimar(90);
                            barraDeVida2.Lastimar(750);
                            golpeandoPJ = true;
                        }
                        break;
                    }
                    if (evento.key.keysym.sym == SDLK_ESCAPE) salir = true;
                    if (evento.key.keysym.sym == SDLK_r){
                        reiniciarJuego();
                    }
                    break;
                case SDL_KEYUP:
                    this->personajeJuego->definir_imagen(QUIETO);
                    scrollearDerecha = false;
                    scrollearIzquierda = false;
                    if((evento.key.keysym.sym == SDLK_d))  {
                        golpeandoPJ = false;
                    }
                    if((evento.key.keysym.sym == SDLK_c))  {
                        cansandoPJ = false;
                    }

                    break;
                default:
                    this->personajeJuego->definir_imagen(QUIETO);

           }

           if (scrollearIzquierda && mover<0) mover+= 5;
           else if (scrollearDerecha && abs(mover)<700) mover-= 5;

           //Limpio y dibujo
           SDL_RenderClear(renderer);

           SDL_RenderCopy(renderer, under, NULL, &r);
           (escenario->capas[0])->Dibujarse(0,0);
           (escenario->capas[1])->Dibujarse(0 + mover/4,0);
           (escenario->capas[2])->Dibujarse(0 + mover/2,0);
           (escenario->capas[3])->Dibujarse((int)escenario->capas[3]->x_logico + mover,0);
           (escenario->capas[4])->Dibujarse((int)escenario->capas[4]->x_logico + mover,0);
           (escenario->capas[5])->Dibujarse((int)escenario->capas[5]->x_logico + mover,0);
           (escenario->capas[6])->Dibujarse((int)escenario->capas[6]->x_logico + mover,0);
           (escenario->capas[7])->Dibujarse((int)escenario->capas[7]->x_logico + mover,0);
           (escenario->capas[8])->Dibujarse((int)escenario->capas[8]->x_logico + mover,208);
           (escenario->capas[9])->Dibujarse((int)escenario->capas[9]->x_logico + mover,0);
          // (escenario->capas[10])->Dibujarse(- mover/2,125);
           barraDeVida1.Dibujarse();
           barraDeVida2.Dibujarse();

           if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
                if(posicionPJ_Piso > 125){
                    posicionPJ_Piso = 125;
                    saltando =saltoDiagonalIZQ = saltoDiagonalDER= false;
                    t = 1.0;
                }else{
                    //Vo = 10px/t ; g =  5.5px/t*t
                    t+=0.05;
                    posicionPJ_Piso -= 10*t; //Vo *t
                    posicionPJ_Piso += 5.5*t*t; // -g *t * t
                    if(saltoDiagonalIZQ && mover<0)
                        mover +=5;
                    if(saltoDiagonalDER && abs(mover)<700)
                        mover -=5;
                }
           }

           this->personajeJuego->Dibujarse(-mover/2,posicionPJ_Piso);


           SDL_RenderPresent(renderer);
           SDL_Delay(1000/40.);
        }
    SDL_DestroyTexture(under);
    };

    void reiniciarJuego(){
        logger->log_debug("Tengo que cambiar las configuraciones");
        terminar_juego();
        cargar_configuracion();
        cargar_capas();
        SDL_SetWindowSize(window, ANCHO_FISICO, ALTO_FISICO); // Dani se encarga de poner esto en su objeto

    };
    void terminar_juego(){
        escenario->Borrar();
        for (unsigned int i = 0; i < conf->capas_vector.size(); i++) delete conf->capas_vector[i];
    };
    void terminar_sdl(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    };

};



int main(int argc, char* argv[]){
    logger->set_debug(true);
    logger->set_warning(true);
    logger->set_error(true);

    logger->log_debug("Empieza el juego");

    Juego juego(argc, argv);
    return juego.jugar();
}
