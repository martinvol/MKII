#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "BarraDeVida.h"
#include "Capa.h"
#include "Escenario.h"
#include "parser.h"
#include "Personaje.hpp"
#include "logger.h"
#include "ConversorDeCoordenadas.h"

using namespace std;

#define QUIETO 0
#define CAMINAR_DERECHA 1
#define CAMINAR_IZQUIERDA 2
#define SALTAR 3
#define SALTODIAGONAL 4
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
//----------------------------------------------------------------
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == NULL){
        logger->log_error("LoadTexture");
    }
    return texture;
}
//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//-----------------------MAIN------------------------
//---------------------------------------------------
class Juego{
public:
    // Para no tener que crearlas y destruirlas en cada loop del juego
    //seria un desperdicio.
    bool Arriba_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    bool saltoDiagonalIZQ = false;
    bool saltoDiagonalDER = false;
    bool golpeandoPJ = false;
    bool cansandoPJ = false;
    bool scrollearDerecha = false;
    bool scrollearIzquierda = false;
    bool saltando = false;
    Sint16 presionado=0;
    SDL_Rect r;
    int posicionPJ_Piso = 125;
    double t = 1.0;



    int argc;
    char** argv;
    ConversorDeCoordenadas* conv;
    SDL_Texture *under;
    SDL_Joystick *Player1;

    bool salir = false;
    SDL_Renderer * renderer = NULL;

    Escenario *escenario;
    BarraDeVida barraDeVida1, barraDeVida2;

    unsigned int ANCHO_FISICO, ALTO_FISICO;
    unsigned int AnchoLogico, AltoLogico;

    int mover = 5;
    int moverSZ =1;

    float x_logico_personaje;
    float borde_izquierdo_logico_pantalla;

    SDL_Window * window = NULL;
    Conf *conf;
    Personaje *personajeJuego;

    Juego(int argc_, char* argv_[]){
        argc = argc_;
        argv = argv_;
        this->escenario = new Escenario();
    };
//----------------------------------------------------------------
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
//----------------------------------------------------------------

    void cargar_configuracion(){
        this->conf = new Conf();
        conf->set_values(argv[1]);
        // Se settean configuraciones (con el json)
        // Esto tiene que cambiarse cuando se aprieta la letra R

        //Pantalla
        ANCHO_FISICO = conf->ventana_anchopx; //800
        ALTO_FISICO = conf->ventana_altopx; //416
        r = {0, 0, ALTO_FISICO, ANCHO_FISICO};
        //Mundo
        AnchoLogico = conf->escenario_ancho;
        AltoLogico = conf->escenario_alto;
        // Martin
        // fin de las configuraciones

        this->conv = new ConversorDeCoordenadas(ALTO_FISICO, ANCHO_FISICO,
                                          AltoLogico, AnchoLogico);

        // Cargamos al personaje en el medio del mapa
        x_logico_personaje = (conf->escenario_ancho/2) - (conf->personaje_ancho/2);
        borde_izquierdo_logico_pantalla = (conf->escenario_ancho/2) - (this->conv->factor_ancho*ANCHO_FISICO/2);

        // printf("%f %f\n", x_logico_personaje, borde_izquierdo_logico_pantalla);


    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void configurar(){
        cargar_configuracion();

        window = SDL_CreateWindow("Mortal Kombat 3 Ultimate",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   ANCHO_FISICO, ALTO_FISICO,
                                   SDL_WINDOW_MAXIMIZED);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        //under = loadTexture("resources/background/p_under.png", renderer);
        cargar_capas();

        //Izquierda
        barraDeVida1.Inicializar(0, ANCHO_FISICO/2, ALTO_FISICO, renderer, true);
       //Derecha
        barraDeVida2.Inicializar(ANCHO_FISICO/2, ANCHO_FISICO, ALTO_FISICO, renderer, false);
        Personaje* personaje = new Personaje(1,1,"Subzero",renderer);
        this->personajeJuego = personaje;
        SDL_JoystickEventState(SDL_ENABLE);
        Player1 = SDL_JoystickOpen(0);
        Arriba_PRESIONADO = Izq_PRESIONADO = Der_PRESIONADO = erre_PRESIONADO = false;
        estadoPersonaje1 = Quieto_State;
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void cargar_capas(){

        for (unsigned int i = 0; i < conf->capas_vector.size(); i++){
            conf->capas_vector[i]->ren = renderer;

            escenario->AgregarCapa( // esto no deberÃ­a estar asÃ­, tendria que andar la lÃ­nea de arriba, pero estuve luchando y no la hago andar (maxi)
                new Capa (conf->capas_vector[i]->ubicacion,
                conf->capas_vector[i]->anchoLogico,
                conf->capas_vector[i]->x_logico,
                conf->capas_vector[i]->ren
                )
            );
        }

    }
//----------------------------------------------------------------
//---------------------------------------------------------------
//----------------------------------------------------------------
    void game_loop(){
        //uno solo...por ahora (?)
        if (SDL_NumJoysticks() < 1){
            cout <<"NO HAY JOYSTICK CONECTADO"<<endl;
        }

        SDL_Event evento;
        while (!salir){
            Controlador(&evento);   //Controlador
            //Modelo
            ActualizarModelo();
            DibujarTodo();          //Vista
            //Ver el delay...
            SDL_Delay(1000/40.);
        }
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void reiniciarJuego(){
        logger->log_debug("Tengo que cambiar las configuraciones");
        terminar_juego();
        cargar_configuracion();
        cargar_capas();
        SDL_SetWindowSize(window, ANCHO_FISICO, ALTO_FISICO); // Dani se encarga de poner esto en su objeto

    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        escenario->Borrar();
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        for (unsigned int i = 0; i < conf->capas_vector.size(); i++) delete conf->capas_vector[i];
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_sdl(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    };


//----------------------------------------------------------------
//----------------------------------------------------------------
void DibujarTodo(){
        //Limpio y dibujo
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, under, NULL, &r);
        //this->escenario->Dibujarse();

        for (int i = escenario->capas.size(); i --> 0; ){
        //for (int i = 0; i < escenario->capas.size(); i ++) {
        // Itero hacia en orden inverso
        // Así respeto los Z index
        //for (int i = 0; i<escenario->capas.size(); i++){
            // Estas son coordenadas lógicas, que por adentro capas las cambia a físicas
            // esa cuenta cancha la deería hacer por afuera, pero comofunciona, por ahora la dejo

            (escenario->capas[i])->Dibujarse(
                escenario->capas[i]->x_logico - borde_izquierdo_logico_pantalla
                // mover*((float)escenario->capas[i]->anchoLogico/(float)conv->x_logico)
                , 0);


            if (i==conf->personaje_zindex){
                this->personajeJuego->Dibujarse(
                    (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    posicionPJ_Piso, conv->factor_alto*conf->personaje_alto, conv->factor_ancho*conf->personaje_ancho);
            }
        }

       /* printf("X logico personaje %f \n", x_logico_personaje);
        printf("Donde dibujo el personaje %f \n", (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho);
*/

        barraDeVida1.Dibujarse();
        barraDeVida2.Dibujarse();

        if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
            if(posicionPJ_Piso > 125){
                posicionPJ_Piso = 125;
                saltando =saltoDiagonalIZQ = saltoDiagonalDER= false;
                //Despues de caer vuelve a quieto.
                estadoPersonaje1 = Quieto_State;
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

        /*if(saltando == true){
            if(posicionPJ_Piso > 125){
                posicionPJ_Piso = 125;
                saltando = false;
                t = 1.0;
            }else{
                //Vo = 10px/t ; g =  5.5px/t*t
                t+=0.05;
                posicionPJ_Piso -= 10*t; //Vo *t
                posicionPJ_Piso += 5.5*t*t; // -g *t * t
            }
        }*/
        // CoordenadaFisica* c = conv->aFisica(new CoordenadaLogica(conf->personaje_ancho, conf->personaje_alto));
        SDL_RenderPresent(renderer);
};

enum Estados{
      Quieto_State,
      SaltoDiagonal_State,
      SaltoVertical_State,
      Caminando_State
   } estadoPersonaje1;


    void Controlador(SDL_Event *evento){
        SDL_PollEvent( evento );
        //SDL_JoystickID myID = SDL_JoystickInstanceID(Player1);
        presionado = SDL_JoystickGetAxis(Player1,0);
        // + ---> DERECHA
        // - ---> IZQUIERDA
        //cout<<presionado<<endl;

        switch(evento->type){
            case SDL_QUIT:
                salir = true;
                break;
            case SDL_KEYDOWN:

                //-----------------------------------------
                //-----------------------------------------
                //---------------BASICOS-------------------
                if (evento->key.keysym.sym == SDLK_UP)  {
                    Arriba_PRESIONADO = true;
                    /*saltando = true;
                    this->personajeJuego->definir_imagen(SALTAR);
                    scrollearDerecha = false;
                    scrollearIzquierda = false;*/
                }
                if (evento->key.keysym.sym == SDLK_RIGHT)  {
                    Der_PRESIONADO = true;
                    /*scrollearDerecha = true;
                    scrollearIzquierda = false;
                    this->personajeJuego->definir_imagen(CAMINAR_DERECHA);*/
                }
                if ((evento->key.keysym.sym == SDLK_LEFT))  {
                    Izq_PRESIONADO = true;
                    /*scrollearIzquierda = true;
                    scrollearDerecha = false;
                    this->personajeJuego->definir_imagen(CAMINAR_IZQUIERDA);*/
                }
                //-----------------------------------------
                //-----------------------------------------

                if(evento->key.keysym.sym == SDLK_a)  {
                    barraDeVida1.Aliviar(20);
                    barraDeVida2.Aliviar(20);
                }
                if(evento->key.keysym.sym == SDLK_c)  {
                    if (cansandoPJ == false){
                        barraDeVida1.Cansar(50);
                        barraDeVida2.Cansar(50);
                        cansandoPJ = true;
                    }
                }
                if((evento->key.keysym.sym == SDLK_d))  {
                    if (golpeandoPJ == false){
                        barraDeVida1.Lastimar(90);
                        barraDeVida2.Lastimar(750);
                        golpeandoPJ = true;
                    }
                    break;
                }
                if (evento->key.keysym.sym == SDLK_ESCAPE) salir = true;
                if (evento->key.keysym.sym == SDLK_r){
                    reiniciarJuego();
                }
                break;
            case SDL_KEYUP:
                //this->personajeJuego->definir_imagen(QUIETO);
                scrollearDerecha = false;
                scrollearIzquierda = false;
                //-----------------------------------------
                //-----------------------------------------
                //---------------BASICOS-------------------
                if((evento->key.keysym.sym == SDLK_UP))  {
                    Arriba_PRESIONADO = false;
                }
                if((evento->key.keysym.sym == SDLK_LEFT))  {
                    Izq_PRESIONADO = false;
                }
                if((evento->key.keysym.sym == SDLK_RIGHT))  {
                    Der_PRESIONADO = false;
                }
                //-----------------------------------------
                //-----------------------------------------

                if((evento->key.keysym.sym == SDLK_d))  {
                    golpeandoPJ = false;
                }
                if((evento->key.keysym.sym == SDLK_c))  {
                    cansandoPJ = false;
                }
                break;
            default:
                ;//this->personajeJuego->definir_imagen(QUIETO);

           }



    };


    void ActualizarModelo(){
      /*Quieto_State,      SaltoDiagonal_State,      SaltoVertical_State,
      Caminando_State */
    //Arriba_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
            if(posicionPJ_Piso > 125){
                //posicionPJ_Piso = conf->escenario_ypiso;
		posicionPJ_Piso = 125;
                saltando =saltoDiagonalIZQ = saltoDiagonalDER= false;
                //Despues de caer vuelve a quieto.
                estadoPersonaje1 = Quieto_State;
                t = 1.0;
            }else{
                //Vo = 10px/t ; g =  5.5px/t*t
                t+=0.05;
                posicionPJ_Piso -= 10*t; //Vo *t
                posicionPJ_Piso += 5.5*t*t; // -g *t * t
                if( (saltoDiagonalIZQ) /*&& (mover<0)*/ ){
                    mover +=5;
                    x_logico_personaje = x_logico_personaje - 5;

                }else if(saltoDiagonalDER  /*&& abs(mover)<700*/){
                    mover -=5;
                    x_logico_personaje = x_logico_personaje + 5;
                }

            }
        }

        switch(estadoPersonaje1){
    //QUIETO
            case Quieto_State:
                //Quieto --> salto Diag Der
                if(Arriba_PRESIONADO && Der_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalDER = true;
                    this->personajeJuego->definir_imagen(SALTODIAGONAL);

                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    break;
                }
                //Quieto -->salto diag izq
                if(Arriba_PRESIONADO && Izq_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;
                    this->personajeJuego->definir_imagen(SALTODIAGONAL);

                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                    break;
                }
                //Quieto --> camina izq.
                if(Izq_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->definir_imagen(CAMINAR_IZQUIERDA);
                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                    break;
                }
                // Quieto --> camina der
                if (Der_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->definir_imagen(CAMINAR_DERECHA);
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    break;
                }
                // Quieto --> salta vertical
                if (Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoVertical_State;
                    saltando = true;
                    this->personajeJuego->definir_imagen(SALTAR);

                    scrollearDerecha = false;
                    scrollearIzquierda = false;
                    break;
                }
                // Quieto -->Quieto
                this->personajeJuego->definir_imagen(QUIETO);
                break;
    //CAMINANDO
            case Caminando_State:
                //Camino --> Salto diagonal der
                if (Der_PRESIONADO && Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalDER = true;
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    this->personajeJuego->definir_imagen(SALTODIAGONAL);
                //Camino --> Salto diagonal izq
                }else if(Izq_PRESIONADO && Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;
                    scrollearIzquierda = true;
                    scrollearDerecha = false;

                    this->personajeJuego->definir_imagen(SALTODIAGONAL);
                //Camino --> sigo caminando
                }else if (Der_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->definir_imagen(CAMINAR_DERECHA);
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                //Camino --> sigo caminando
                }else if (Izq_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->definir_imagen(CAMINAR_IZQUIERDA);
                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                //Camino y no hay accion--> quieto
                }else{
                    estadoPersonaje1 = Quieto_State;
                    this->personajeJuego->definir_imagen(QUIETO);
                }
                break;
    //SALTANDO_VERTICAL
            case SaltoVertical_State:
                estadoPersonaje1 = SaltoVertical_State;
                this->personajeJuego->definir_imagen(SALTAR);
                scrollearDerecha = false;
                scrollearIzquierda = false;
                break;
    //SALTANDO_DIAGONAL
            case SaltoDiagonal_State:
                estadoPersonaje1 = SaltoDiagonal_State;
                this->personajeJuego->definir_imagen(SALTODIAGONAL);
                if (saltoDiagonalIZQ){
                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                }else{
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                }
                break;
            default:
                this->personajeJuego->definir_imagen(SALTAR);
        }


        //Scrolleo desde el modelo, y despues de procesar movimientos, sino se achata.
        if (scrollearIzquierda){
                x_logico_personaje = x_logico_personaje - 5.;
                if ((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho < ANCHO_FISICO*(100-conf->margen)/200)

                {
                    x_logico_personaje = x_logico_personaje + 5.;
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - 5.;
                    if (borde_izquierdo_logico_pantalla<0){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + 5.;
                        puts("Se acabó la pantalla");
                    }
                    puts("me muevo hacia la Izquierda");
                }
                // mover+= 5;

        } else if (scrollearDerecha){
                x_logico_personaje = x_logico_personaje + 5.;
                if ((x_logico_personaje + (conf->personaje_ancho/2) - borde_izquierdo_logico_pantalla)*conv->factor_ancho > (ANCHO_FISICO -ANCHO_FISICO*(100-conf->margen)/200))
                {
                    x_logico_personaje = x_logico_personaje - 5.;
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + 5.;

                    if (borde_izquierdo_logico_pantalla + (((float)ANCHO_FISICO)/conv->factor_ancho) > conf->escenario_ancho){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - 5.;
                        puts("Se acabó la pantalla");
                    }
                    puts("me muevo hacia la derecha");
                }
            }

    };

};//FIN CLASE JUEGO
//----------------------------------------------------------------
//----------------------------------------------------------------
int main(int argc, char* argv[]){
    logger->set_debug(true);
    logger->set_warning(true);
    logger->set_error(true);

    logger->log_debug("Empieza el juego");

    Juego juego(argc, argv);
    return juego.jugar();
}
