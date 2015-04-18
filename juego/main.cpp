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
#include "timer.hpp"

using namespace std;

#define MOVER_PIXELES 60*(conf->personaje_ancho/conf->ventana_ancho)
#define MOVER_PIXELES_VERTICAL 75*(conf->personaje_alto/conf->escenario_ancho)
#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000
#define ALTURA_MAX_SALTO conf->personaje_alto + conf->escenario_ypiso
#define IMG_DEFAULT "resources/miscelaneo/06.gif" ///
#define CONST_MAXI_DELAY 50
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
    bool alturaMaxima = false;
    Sint16 presionado=0;
    SDL_Rect r;
    int posicionPJ_Piso = 0;
    double t = 5.0;
    bool pausa = false;
    bool cambiarModo = false;

    int argc;
    char** argv;
    ConversorDeCoordenadas* conv;
    SDL_Texture *under;

    bool usandoJoystick = false;
    SDL_Joystick *Player1;
    int x_Joystick, y_Joystick;


    bool salir = false;
    SDL_Renderer * renderer = NULL;

    Escenario *escenario;
    BarraDeVida barraDeVida1, barraDeVida2;

    unsigned int ANCHO_FISICO, ALTO_FISICO;
    unsigned int AnchoLogico, AltoLogico;

    float x_logico_personaje;
    float borde_izquierdo_logico_pantalla;

    SDL_Window * window = NULL;
    Conf *conf;
    Personaje *personajeJuego;
    Estado* estado;
    Timer* timer;

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
        if (argc == 1 )
            conf->cargarDefault();
        else
            conf->set_values(argv[1]);
        // Se settean configuraciones (con el json)
        // Esto tiene que cambiarse cuando se aprieta la letra R

        //Pantalla
        ANCHO_FISICO = conf->ventana_anchopx; //800
        ALTO_FISICO = conf->ventana_altopx; //416
        posicionPJ_Piso = conf->escenario_ypiso;
        r = {0, 0, ALTO_FISICO, ANCHO_FISICO};
        //Mundo
        AnchoLogico = conf->escenario_ancho;
        AltoLogico = conf->escenario_alto;
        // Martin
        // fin de las configuraciones

        

        // Cargamos al personaje en el medio del mapa
        x_logico_personaje = (conf->escenario_ancho/2) - (conf->personaje_ancho/2);


        borde_izquierdo_logico_pantalla = (conf->escenario_ancho/2.) - (conf->ventana_ancho/2.);
        this->timer = new Timer(100, IMG_DEFAULT, conv, renderer);

		this->conv = new ConversorDeCoordenadas(conf->ventana_altopx, conf->ventana_anchopx,
												conf->escenario_alto, conf->escenario_ancho, borde_izquierdo_logico_pantalla);
	
        // printf("%f %f\n", x_logico_personaje, borde_izquierdo_logico_pantalla);


    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void configurar(){
        cargar_configuracion();

        window = SDL_CreateWindow("Mortal Kombat 3 Ultimate",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   conf->ventana_anchopx, conf->ventana_altopx,
                                   SDL_WINDOW_MAXIMIZED);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        under = loadTexture("resources/background/p_under.png", renderer);
        cargar_capas();

        //Izquierda
        barraDeVida1.Inicializar(0, conf->ventana_anchopx/2, conf->ventana_altopx, renderer, true) ;
       //Derecha
        barraDeVida2.Inicializar(conf->ventana_anchopx/2, conf->ventana_anchopx, conf->ventana_altopx, renderer, false);
        estado = new Estado((string)(this->conf->sprites_map["personaje1"]), renderer, conf->personaje_alto, conf->escenario_alto, conf->personaje_ancho, conf->escenario_ancho);
        Personaje* personaje = new Personaje(new CoordenadaLogica(x_logico_personaje,conf->escenario_ypiso),"Subzero",renderer, conf->personaje_alto, conf->personaje_ancho, estado, conf);
        this->personajeJuego = personaje;

        Player1 = SDL_JoystickOpen(0);
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");

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
                conf->capas_vector[i]->ren,
                this->conv,
                conf->escenario_ancho
                )
            );
        }

    }
//----------------------------------------------------------------
//---------------------------------------------------------------
//----------------------------------------------------------------
    void game_loop(){

        float timerFps;

        //uno solo...por ahora (?)
        if (SDL_NumJoysticks() < 1){
            logger->log_warning("No hay JOYSTICK conectado");
        }else{
            usandoJoystick = true;
        }

        SDL_Event evento;
        while (!salir){

            timerFps = SDL_GetTicks();
            
            Controlador(&evento);       //Controlador
            if (!pausa){
                ActualizarModelo();     //Modelo
            }
            DibujarTodo();              //Vista

            SDL_FlushEvent(SDL_KEYDOWN);


            timerFps = SDL_GetTicks() - timerFps;

            if(timerFps < int(1000/24)){
                SDL_Delay(CONST_MAXI_DELAY);
            }
        }

    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void reiniciarJuego(){
        logger->log_debug("Tengo que cambiar las configuraciones");
        terminar_juego();
        cargar_configuracion();
        estado = new Estado((string)(this->conf->sprites_map["personaje1"]), renderer, conf->personaje_alto, conf->escenario_alto, conf->personaje_ancho, conf->escenario_ancho);
        this->personajeJuego = new Personaje(new CoordenadaLogica(x_logico_personaje,conf->escenario_ypiso),"Subzero",renderer, conf->personaje_alto, conf->personaje_ancho, estado, conf);
        cargar_capas();
        SDL_SetWindowSize(window, conf->ventana_anchopx, conf->ventana_altopx); // Dani se encarga de poner esto en su objeto
        barraDeVida1.Inicializar(0, conf->ventana_anchopx/2, conf->ventana_altopx, renderer, true);
        barraDeVida2.Inicializar(conf->ventana_anchopx/2, conf->ventana_anchopx, conf->ventana_altopx, renderer, false);
        this->timer->reset();
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        escenario->Borrar();
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        delete this->personajeJuego; // Elimina el estado también
        delete this->timer;
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

        SDL_Rect fillRect = {0, 0, conf->ventana_anchopx, conf->ventana_altopx};
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect(renderer, &fillRect );


        SDL_RenderCopy(renderer, under, NULL, &r);
        // Que es esto?

        for (int i = 0; i < escenario->capas.size(); i++){
        // Itero hacia en orden inverso
        // Así respeto los Z index
        //for (int i = 0; i<escenario->capas.size(); i++){
            // Estas son coordenadas lógicas, que por adentro capas las cambia a físicas
            // esa cuenta cancha la deería hacer por afuera, pero comofunciona, por ahora la dejo


            (escenario->capas[i])->DibujarseAnchoReal2(borde_izquierdo_logico_pantalla, 0, conv);

            /*(escenario->capas[i])->DibujarseAnchoReal(
                escenario->capas[i]->x_logico - borde_izquierdo_logico_pantalla
                + (AnchoLogico - escenario->capas[i]->anchoLogico)*(borde_izquierdo_logico_pantalla )/(AnchoLogico-(((float)ANCHO_FISICO)/conv->factor_ancho))
                // mover*((float)escenario->capas[i]->anchoLogico/(float)conv->x_logico)
                , 0, conv);

            //(escenario->capas[i])->DibujarseAnchoReal(escenario->capas[i]->x_logico + mover, 0, conv);
*/

            if (i==conf->personaje_zindex){
                //~ CoordenadaFisica* coord = conv->aFisica(personajeJuego->obtenerCoordenadaIzqSup());
                this->personajeJuego->Dibujarse(
                    //~ coord->x_fisico,
                    //~ coord->y_fisico,
                    (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    (conf->escenario_alto - posicionPJ_Piso - (conf->personaje_alto))*(conf->ventana_altopx/conf->escenario_alto),
                    (conf->ventana_altopx/conf->escenario_alto)*conf->personaje_alto,
                    (conf->ventana_anchopx/conf->ventana_ancho)*conf->personaje_ancho);
				//~ delete coord;
            }
        }

        if (escenario->capas.size()==0 || conf->personaje_zindex >= (escenario->capas.size())){
            // Si no hay capas;
            //~ CoordenadaFisica* coord = conv->aFisica(personajeJuego->obtenerCoordenadaIzqSup());
			this->personajeJuego->Dibujarse(
					//~ coord->x_fisico,
					//~ coord->y_fisico,
                    (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    (conf->escenario_alto - posicionPJ_Piso - (conf->personaje_alto))*(conf->ventana_altopx/conf->escenario_alto),
                    (conf->ventana_altopx/conf->escenario_alto)*conf->personaje_alto,
                    (conf->ventana_anchopx/conf->ventana_ancho)*conf->personaje_ancho);
			//~ delete coord;
        }
		
        barraDeVida1.Dibujarse();
        barraDeVida2.Dibujarse();

        //this->timer->Dibujarse();
		
        // CoordenadaFisica* c = conv->aFisica(new CoordenadaLogica(conf->personaje_ancho, conf->personaje_alto));
        if (pausa){
            SDL_Rect pantalla = {0,0,conf->ventana_anchopx,conf->ventana_altopx};
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 150 );
            SDL_RenderFillRect( renderer, &pantalla );
        }
        SDL_RenderPresent(renderer);
};

enum Estados{
      Quieto_State,
      SaltoDiagonal_State,
      SaltoVertical_State,
      Caminando_State
   } estadoPersonaje1;


void Controlador(SDL_Event *evento){
       while(SDL_PollEvent( evento )) {

        //Ahora anda este tambien.
        /*if (evento->type == SDL_JOYBUTTONDOWN){
            ;
        }*/
    if(usandoJoystick){
        x_Joystick = SDL_JoystickGetAxis(Player1, 0);
        y_Joystick = SDL_JoystickGetAxis(Player1, 1);
        if( x_Joystick < -JOYSTICK_DEAD_ZONE ){
            //  x = -1;
            Izq_PRESIONADO = true;

        }else if( x_Joystick > JOYSTICK_DEAD_ZONE ){
            //  x =  1;
            Der_PRESIONADO = true;
        }else{
            //  x = 0;
            Izq_PRESIONADO =false;
            Der_PRESIONADO = false;
            scrollearDerecha = false;
            scrollearIzquierda = false;
        }

        //Vertical
        if( y_Joystick < -JOYSTICK_DEAD_ZONE ){
            //  y = -1;
            Arriba_PRESIONADO = true;
            }else if( y_Joystick > JOYSTICK_DEAD_ZONE ){
                //y =  1;
                //Abajo_PRESIONADO = true;
                ;
            }else{
                //yDir = 0;
                Arriba_PRESIONADO = false;
            }
    }
        //-----------------------------------------
        //----------EVENTOS NO-JOYSTICK------------
        //-----------------------------------------
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
                if (evento->key.keysym.sym == SDLK_p)  {
                    cambiarModo = true;
                }
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
                if((evento->key.keysym.sym == SDLK_p) && (cambiarModo))  {
                    cambiarModo = false;
                    pausa = !pausa;
                }
                if((evento->key.keysym.sym == SDLK_d))  {
                    golpeandoPJ = false;
                }
                if((evento->key.keysym.sym == SDLK_c))  {
                    cansandoPJ = false;
                }
                break;
            default:
                ;
           }
        }
    };


    void ActualizarModelo(){
      /*Quieto_State,      SaltoDiagonal_State,      SaltoVertical_State,
      Caminando_State */
    //Arriba_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
            if(posicionPJ_Piso < conf->escenario_ypiso){
                posicionPJ_Piso = conf->escenario_ypiso;
                saltando = saltoDiagonalIZQ = saltoDiagonalDER = false;
                //Despues de caer vuelve a quieto.
                alturaMaxima = false;
                estadoPersonaje1 = Quieto_State;
                this->personajeJuego->definir_imagen(QUIETO);
                //~ cout<<"QUIERO IR A QUIETOOOO"<<endl;
                t = 5.0;
            }else{
            //alturaMaxima
                //Vo = 10px/t ; g = 6px/t*t
               //t+=0.01;
                //||*/ (posicionPJ_Piso <(conf->escenario_ypiso- posicionPJ_Piso))
                //Nota: posicionPJ_Piso = 0 no es en la parte superior de la pantalla :-/
                if (posicionPJ_Piso  >= ALTURA_MAX_SALTO) {
                    alturaMaxima = true;
                    t = 5.0;
                }
                if (alturaMaxima){
                    posicionPJ_Piso -= MOVER_PIXELES_VERTICAL*t;//*t;
                    //alturaMaxima = false;
                }else{
                    posicionPJ_Piso += MOVER_PIXELES_VERTICAL*t;//*t;
                }
                //posicionPJ_Piso -= MOVER_PIXELES*10*t; //Vo *t
                //posicionPJ_Piso += MOVER_PIXELES*6*t*t; // -g *t * t
                //this->personajeJuego->definir_imagen(SALTODIAGONAL);
                if(saltoDiagonalIZQ){
                    if (x_logico_personaje - 3*MOVER_PIXELES >= 0) x_logico_personaje -= 3*MOVER_PIXELES;

                    if (((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho) < (ANCHO_FISICO*(100-conf->margen)/200))
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - 3*MOVER_PIXELES;

                }else if(saltoDiagonalDER){
                    if (x_logico_personaje <= (conf->escenario_ancho - conf->personaje_ancho)) x_logico_personaje += 3*MOVER_PIXELES;

                    if (((borde_izquierdo_logico_pantalla + 3*MOVER_PIXELES + conf->ventana_ancho) < conf->escenario_ancho)
                    &&((x_logico_personaje + (conf->personaje_ancho) - borde_izquierdo_logico_pantalla)> (conf->ventana_ancho- conf->ventana_ancho*(100-conf->margen)/200)))
                        borde_izquierdo_logico_pantalla += 3*MOVER_PIXELES;
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
                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_DER);

                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    break;
                }
                //Quieto -->salto diag izq
                if(Arriba_PRESIONADO && Izq_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;
                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_IZQ);

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
                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_DER);
                //Camino --> Salto diagonal izq
                }else if(Izq_PRESIONADO && Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;

                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_IZQ);
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
                if (saltoDiagonalDER)
                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_DER);
                else
                    this->personajeJuego->definir_imagen(SALTARDIAGONAL_IZQ);

                break;
            default:
                this->personajeJuego->definir_imagen(QUIETO);
        }
        if (scrollearIzquierda){
                if (x_logico_personaje >= 0 && ((x_logico_personaje - MOVER_PIXELES)>=0)) x_logico_personaje -= MOVER_PIXELES;
                if ((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho < ANCHO_FISICO*(100-conf->margen)/200)
                {
                    //x_logico_personaje = x_logico_personaje + MOVER_PIXELES;
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                    if (borde_izquierdo_logico_pantalla<0 && (borde_izquierdo_logico_pantalla - MOVER_PIXELES)<0){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;
                        this->personajeJuego->definir_imagen( QUIETO);
                    }
                }
                // mover+= 5;


            } else if (scrollearDerecha){
                 if (x_logico_personaje <= conf->escenario_ancho - conf->personaje_ancho) x_logico_personaje += MOVER_PIXELES;
                 if ((x_logico_personaje + (conf->personaje_ancho) - borde_izquierdo_logico_pantalla)> (conf->ventana_ancho -conf->ventana_ancho*(100-conf->margen)/200))
                {
                    //x_logico_personaje = x_logico_personaje - MOVER_PIXELES;
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;

                    if (borde_izquierdo_logico_pantalla + (conf->ventana_ancho) >= conf->escenario_ancho){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                        this->personajeJuego->definir_imagen(QUIETO);
                    }
                }
            }
            
            this->timer->avanzarTimer(SDL_GetTicks()); 
    };

};//FIN CLASE JUEGO
//----------------------------------------------------------------
//----------------------------------------------------------------
int main(int argc, char* argv[]){
    logger->set_debug(true);
    logger->set_warning(true);
    logger->set_error(true);

    logger->log_debug("-------------------------------------");
    logger->log_debug("Empieza el juego");

    Juego juego(argc, argv);
    return juego.jugar();
}
