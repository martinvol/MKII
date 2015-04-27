#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Escenario/BarraDeVida.hpp"
#include "Escenario/Capa.hpp"
#include "Escenario/Escenario.hpp"
#include "Parser/Parser.hpp"
#include "Personaje/Personaje.hpp"
#include "Logger/Logger.hpp"
#include "Coordenadas/ConversorDeCoordenadas.hpp"
#include "Escenario/Timer.hpp"
#include "Director/Director.hpp"

using namespace std;

#define MOVER_PIXELES 60*(parser->personaje_ancho/parser->ventana_ancho)
#define MOVER_PIXELES_VERTICAL 75*(parser->personaje_alto/parser->escenario_ancho)
#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000
#define IMG_DEFAULT "resources/miscelaneo/06.png"
#define CONST_MAXI_DELAY 50
#define FACTOR_SCROLL MOVER_PIXELES

#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000
#define ALTURA_MAX_SALTO parser->personaje_alto + parser->escenario_ypiso
#define CONST_MAXI_DELAY 50


// Esto no debería ser global, o si?
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
//---------------------------------------------------
//---------------------------------------------------

class Juego{
public:
    // Para no tener que crearlas y destruirlas en cada loop del juego
    //seria un desperdicio.
    bool Arriba_PRESIONADO, Abajo_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    bool saltoDiagonalIZQ = false;
    bool saltoDiagonalDER = false;
    bool golpeandoPJ = false;
    bool cansandoPJ = false;
    
    Sint16 presionado=0;
    SDL_Rect r;
    double t = 5.0;
    bool pausa = false;
    bool cambiarModo = false;

    int argc;
    char** argv;
    SDL_Texture *under;

    bool usandoJoystick = false;
    SDL_Joystick *Player1;
    int x_Joystick, y_Joystick;


    bool salir = false;
    SDL_Renderer * renderer = NULL;

    Parser* parser;
    
    float x_logico_personaje;
    float borde_izquierdo_logico_pantalla;

	Escenario* escenario;
	Ventana* ventana;
    Timer* timer;

    ConversorDeCoordenadas* conversor;

	Estado* estado, *estado2;
    Personaje* personajeJuego, *personajeJuego2;
    BarraDeVida* barraDeVida1, *barraDeVida2;

	Director* director;

//----------------------------------------------------------------

    Juego(int argc_, char* argv_[]){
        argc = argc_;
        argv = argv_;
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
        Logger::destroy_instance();
        return 0;
    };
//----------------------------------------------------------------

    void cargar_configuracion(Parser* parser){
        if (argc == 1 )
            parser->cargarDefault();
        else
            parser->set_values(argv[1]);
        // Se settean Parseriguraciones (con el json)
        // Esto tiene que cambiarse cuando se aprieta la letra R
        // fin de las configuraciones
		// Se crean todos los objetos que dependen de ellas:
		
		// Tomamos la ventana en el medio del escenario.
        borde_izquierdo_logico_pantalla = (parser->escenario_ancho/2.) - (parser->ventana_ancho/2.);

		this->r = {0, 0, parser->ventana_anchopx, parser->ventana_altopx};
		
		this->escenario = new Escenario(parser->escenario_ancho, parser->escenario_alto);
		this->conversor = new ConversorDeCoordenadas(parser->ventana_altopx, parser->ventana_anchopx,
                             parser->escenario_alto, parser->ventana_ancho, borde_izquierdo_logico_pantalla);
		this->ventana = new Ventana("Mortal Kombat 3 Ultimate", parser->ventana_anchopx, parser->ventana_altopx, parser->margen);
        
        renderer = SDL_CreateRenderer(ventana->window, -1, SDL_RENDERER_SOFTWARE);
        under = loadTexture("resources/background/p_under.png", renderer);
        cargar_capas();
        
        
    // PERSONAJE 1
		// Cargamos al personaje en el medio de la ventana, pero del lado izquierdo.
		// Este x_logico es del extremo izquierdo del personaje.
        x_logico_personaje = (parser->escenario_ancho/2.) - (parser->personaje_ancho);
        
        estado = new Estado((string)(this->parser->sprites_map["personaje1"]),
							renderer, parser->personaje_alto, parser->escenario_alto,
							parser->personaje_ancho, parser->escenario_ancho);
        this->personajeJuego = new Personaje(new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso),
										"Subzero", renderer, parser->personaje_ancho,
										parser->personaje_alto, estado,
										parser->personaje_mirar_derecha);
		printf("Personaje creado con estado: %s", (personajeJuego->nroAccionActual == QUIETO) ? "QUIETO\n" : "NO QUIETO\n");      

        //Izquierda
        barraDeVida1 = new BarraDeVida(0, parser->ventana_anchopx/2, parser->ventana_altopx, renderer, true);

	//~ // PERSONAJE 2
		//~ // Cargamos al personaje en el medio de la ventana, pero del lado derecho.
		//~ // Este x_logico es del extremo izquierdo del personaje.
        //~ x_logico_personaje2 = (parser->escenario_ancho/2.);
		//~ estado = new Estado((string)(this->parser->sprites_map["personaje2"]),
							//~ renderer, parser->personaje2_alto, parser->escenario_alto,
							//~ parser->personaje2_ancho, parser->escenario_ancho);
        //~ this->personajeJuego = new Personaje(new CoordenadaLogica(x_logico_personaje2,parser->escenario_ypiso),
										//~ "Kabal", renderer, parser->personaje2_ancho,
										//~ parser->personaje2_alto, estado,
										//~ parser->personaje2_mirar_derecha); 
       //~ //Derecha
        barraDeVida2 = new BarraDeVida(parser->ventana_anchopx/2, parser->ventana_anchopx, parser->ventana_altopx, renderer, false);
        
        this->timer = new Timer(100, IMG_DEFAULT, conversor, renderer);
        this->timer->reset(SDL_GetTicks());

}

//----------------------------------------------------------------
//----------------------------------------------------------------
    void configurar(){
        this->parser = new Parser();
        cargar_configuracion(this->parser);
		
		//~ // Para dos personajes:
		//~ director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, this->personajeJuego2, barraDeVida1, barraDeVida2, FACTOR_SCROLL, this->timer);
		//~ // Con un personaje:
		director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, barraDeVida1, FACTOR_SCROLL, this->timer);

        Player1 = SDL_JoystickOpen(0);
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");
		//~ // De antes:
        //~ Arriba_PRESIONADO = Izq_PRESIONADO = Der_PRESIONADO = erre_PRESIONADO =Abajo_PRESIONADO  = false;
        //~ estadoPersonaje1 = Quieto_State;
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void cargar_capas(){
        for (unsigned int i = 0; i < parser->capas_vector.size(); i++){
            //~ // Debería funcionar esto:
            //~ parser->capas_vector[i]->ren = renderer;
			//~ escenario->AgregarCapa(parser->capas_vector[i]);
            //~ // Pero no lo conseguimos (Maxi luchó bastante)
            escenario->AgregarCapa( 
                new Capa (parser->capas_vector[i]->ubicacion,
                parser->capas_vector[i]->anchoLogico,
                parser->capas_vector[i]->x_logico,
                renderer,
                this->parser->escenario_ancho,
                this->parser->ventana_ancho
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
                // Antes:
                //~ ActualizarModelo();     //Modelo
                // Ahora:
                this->director->actualizar();	//Modelo  
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
        logger->log_debug("Tengo que cambiar las Parseriguraciones");
        terminar_juego();
        configurar();
	
	//~ // De antes:
	//~ //Habia que reiniciar los estados.
        //~ estadoPersonaje1 = Quieto_State;
		//~ Arriba_PRESIONADO = Izq_PRESIONADO = Der_PRESIONADO = erre_PRESIONADO =Abajo_PRESIONADO  = false;
        //~ saltando = saltoDiagonalIZQ = saltoDiagonalDER = false;

    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        delete this->parser;	// Elimina sus propias capas.
        delete this->director; 	// Elimina, conversor, jugadores (personajes y barras de vida), timer, escenario, ventana
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_sdl(){
        SDL_DestroyRenderer(renderer);
        IMG_Quit();
        SDL_Quit();
    };


//----------------------------------------------------------------
//----------------------------------------------------------------
void DibujarTodo(){
        //Limpio y dibujo
        SDL_RenderClear(renderer);

        SDL_Rect fillRect = {0, 0, parser->ventana_anchopx, parser->ventana_altopx};
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect(renderer, &fillRect );


        SDL_RenderCopy(renderer, under, NULL, &r);
        // Que es esto?

        for (int i = 0; i < escenario->capas.size(); i++){
        // Itero hacia en orden inverso
        // Así respeto los Z index

			// Estas son coordenadas lógicas, que por adentro capas las cambia a físicas
			// esa cuenta cancha la debería hacer por afuera, pero comofunciona, por ahora la dejo
			(escenario->capas[i])->DibujarseAnchoReal(borde_izquierdo_logico_pantalla, 0);

			// Si el z_index del personaje está entre medio de las capas:
            if (i==parser->personaje_zindex){
                this->personajeJuego->Dibujarse(conversor);
			}
        }

		// Si no hay capaz o el z_index del personaje supera al indice de la ultima capa, lo debo imprimir ahora:
        if (escenario->capas.size()==0 || parser->personaje_zindex >= (escenario->capas.size())){
			this->personajeJuego->Dibujarse(conversor);
		}
		
        this->barraDeVida1->Dibujarse();
        this->barraDeVida2->Dibujarse();

        this->timer->Dibujarse();
		
        if (pausa){
            SDL_Rect pantalla = {0,0,parser->ventana_anchopx,parser->ventana_altopx};
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 150 );
            SDL_RenderFillRect( renderer, &pantalla );
        }
        SDL_RenderPresent(renderer);
};

/* El de ahora debería ser avisándole al director */
void Controlador(SDL_Event *evento){
	bool nada1 = true;
	bool nada2 = true;
	while(SDL_PollEvent( evento )) {
		if(usandoJoystick){
			
			x_Joystick = SDL_JoystickGetAxis(Player1, 0);
			y_Joystick = SDL_JoystickGetAxis(Player1, 1);
			
			if( x_Joystick < -JOYSTICK_DEAD_ZONE ){
				//  x = -1;
				this->director->seMuevePersonaje(jugador1, Izquierda);
				nada1 = false;

			}else if( x_Joystick > JOYSTICK_DEAD_ZONE ){
				//  x =  1;
				this->director->seMuevePersonaje(jugador1, Derecha);
				nada1 = false;
			}else{
				//  x = 0;
			}

			//Vertical
			if( y_Joystick < -JOYSTICK_DEAD_ZONE ){
				//  y = -1;
				this->director->seMuevePersonaje(jugador1, Arriba);
				nada1 = false;
			}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){
				//y =  1;
				//Abajo_PRESIONADO = true;
				;
			}else{
				//yDir = 0;
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
				if (evento->key.keysym.sym == SDLK_UP && !pausa)  {
					this->director->seMuevePersonaje(jugador1, Arriba);
					nada1 = false;
				}
				if (evento->key.keysym.sym == SDLK_RIGHT && !pausa)  {
					this->director->seMuevePersonaje(jugador1, Derecha);
					nada1 = false;
				}
				if ((evento->key.keysym.sym == SDLK_LEFT && !pausa))  {
					this->director->seMuevePersonaje(jugador1, Izquierda);
					nada1 = false;
				}
				//-----------------------------------------
				//-----------------------------------------
				if (evento->key.keysym.sym == SDLK_p)  {
					pausa = !pausa;
				}
				if(evento->key.keysym.sym == SDLK_a && !pausa)  {
					barraDeVida1->Aliviar(20);
					barraDeVida2->Aliviar(20);
				}
				if(evento->key.keysym.sym == SDLK_c && !pausa)  {
					if (cansandoPJ == false){
						barraDeVida1->Cansar(50);
						barraDeVida2->Cansar(50);
						cansandoPJ = true;
					}
				}
				if((evento->key.keysym.sym == SDLK_d && !pausa))  {
					if (golpeandoPJ == false){
						barraDeVida1->Lastimar(90);
						barraDeVida2->Lastimar(750);
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
	if (nada1 && !pausa) this->director->seMuevePersonaje(jugador1, Nada);
}

/* El de antes: 

enum Estados{
      Quieto_State,
      SaltoDiagonal_State,
      SaltoVertical_State,
      Caminando_State,
      Agachado_State
   } estadoPersonaje1;

void Controlador(SDL_Event *evento){
	while(SDL_PollEvent( evento )) {
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
                    //saltando = true;
                    //this->personajeJuego->activarAccion(SALTAR);
                    //scrollearDerecha = false;
                    //scrollearIzquierda = false;
                }
                if (evento->key.keysym.sym == SDLK_DOWN)  {
                    Abajo_PRESIONADO = true;
                }
                if (evento->key.keysym.sym == SDLK_RIGHT)  {
                    Der_PRESIONADO = true;
                    //scrollearDerecha = true;
                    //scrollearIzquierda = false;
                    //this->personajeJuego->activarAccion(CAMINAR_DERECHA);
                }
                if ((evento->key.keysym.sym == SDLK_LEFT))  {
                    Izq_PRESIONADO = true;
                    //scrollearIzquierda = true;
                    //scrollearDerecha = false;
                    //this->personajeJuego->activarAccion(CAMINAR_IZQUIERDA);
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
                    //delete this->personajeJuego->estado;
                    //this->personajeJuego->activarAccion(QUIETO);
                    reiniciarJuego();
                    return;
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
                if((evento->key.keysym.sym == SDLK_DOWN))  {
                    Abajo_PRESIONADO = false;
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
                    //Extraer esto si es posible. El metodo depende del estado del
                    //estado de la variable pausa.
                    this->timer->pausarTimer(SDL_GetTicks());
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
*/

/*  El actualizar modelo de antes, que ahora se supone que se encarga el director:
void ActualizarModelo(){
      //Quieto_State,      SaltoDiagonal_State,      SaltoVertical_State,
      //Caminando_State 
    //Arriba_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
            if(posicionPJ_Piso < parser->escenario_ypiso){
                posicionPJ_Piso = parser->escenario_ypiso;
                saltando = saltoDiagonalIZQ = saltoDiagonalDER = false;
                //Despues de caer vuelve a quieto.
                alturaMaxima = false;
                estadoPersonaje1 = Quieto_State;
                this->personajeJuego->activarAccion(QUIETO);
                //~ cout<<"QUIERO IR A QUIETOOOO"<<endl;
                t = 5.0;
            }else{
            //alturaMaxima
                //Vo = 10px/t ; g = 6px/t*t
               //t+=0.01;
                // (posicionPJ_Piso <(parser->escenario_ypiso- posicionPJ_Piso))
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
                //this->personajeJuego->activarAccion(SALTODIAGONAL);
                if(saltoDiagonalIZQ){
                    if (x_logico_personaje - 3*MOVER_PIXELES >= 0) x_logico_personaje -= 3*MOVER_PIXELES;

                    if (((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho) < (ANCHO_FISICO*(100-parser->margen)/200))
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - 3*MOVER_PIXELES;

                }else if(saltoDiagonalDER){
                    if (x_logico_personaje <= (parser->escenario_ancho - parser->personaje_ancho)) {
						x_logico_personaje += 3*MOVER_PIXELES;
					}

                    if (((borde_izquierdo_logico_pantalla + 3*MOVER_PIXELES + parser->ventana_ancho) < parser->escenario_ancho)
                    &&((x_logico_personaje + (parser->personaje_ancho) - borde_izquierdo_logico_pantalla)> (parser->ventana_ancho- parser->ventana_ancho*(100-parser->margen)/200))){
						//~ puts("hola");
                        borde_izquierdo_logico_pantalla += 3*MOVER_PIXELES;
					}
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
                    this->personajeJuego->activarAccion(SALTARDIAGONAL_DER);

                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    break;
                }
                //Quieto -->salto diag izq
                if(Arriba_PRESIONADO && Izq_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;
                    this->personajeJuego->activarAccion(SALTARDIAGONAL_IZQ);

                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                    break;
                }
                //Quieto --> camina izq.
                if(Izq_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->activarAccion(CAMINAR_IZQUIERDA);

                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                    break;
                }
                // Quieto --> camina der
                if (Der_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->activarAccion(CAMINAR_DERECHA);
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                    break;
                }
                // Quieto --> salta vertical
                if (Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoVertical_State;
                    saltando = true;
                    this->personajeJuego->activarAccion(SALTAR);

                    scrollearDerecha = false;
                    scrollearIzquierda = false;
                    break;
                }
                if (Abajo_PRESIONADO){
                    estadoPersonaje1 = Agachado_State;
                    this->personajeJuego->activarAccion(AGACHARSE);
                    break;
                }
                // Quieto -->Quieto
                this->personajeJuego->activarAccion(QUIETO);
                break;
    //CAMINANDO
            case Caminando_State:
                //Camino --> Salto diagonal der
                if (Der_PRESIONADO && Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalDER = true;
                    this->personajeJuego->activarAccion(SALTARDIAGONAL_DER);
                //Camino --> Salto diagonal izq
                }else if(Izq_PRESIONADO && Arriba_PRESIONADO){
                    estadoPersonaje1 = SaltoDiagonal_State;
                    saltoDiagonalIZQ = true;

                    this->personajeJuego->activarAccion(SALTARDIAGONAL_IZQ);
                //Camino --> sigo caminando
                }else if (Der_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->activarAccion(CAMINAR_DERECHA);
                    scrollearDerecha = true;
                    scrollearIzquierda = false;
                //Camino --> sigo caminando
                }else if (Izq_PRESIONADO){
                    estadoPersonaje1 = Caminando_State;
                    this->personajeJuego->activarAccion(CAMINAR_IZQUIERDA);
                    scrollearIzquierda = true;
                    scrollearDerecha = false;
                //Camino y no hay accion--> quieto
                }else{
                    estadoPersonaje1 = Quieto_State;
                    this->personajeJuego->activarAccion(QUIETO);
                }
                break;

    //AGACHADO
            case Agachado_State:
                if(!Abajo_PRESIONADO){
                    estadoPersonaje1 = Quieto_State;
                    this->personajeJuego->activarAccion(QUIETO);
                }else{
                    this->personajeJuego->activarAccion(AGACHARSE);
                    cout<<"agachado"<<endl;
                }
                break;
    //SALTANDO_VERTICAL
            case SaltoVertical_State:
                estadoPersonaje1 = SaltoVertical_State;
                this->personajeJuego->activarAccion(SALTAR);
                scrollearDerecha = false;
                scrollearIzquierda = false;
                break;
    //SALTANDO_DIAGONAL
            case SaltoDiagonal_State:
                estadoPersonaje1 = SaltoDiagonal_State;
                if (saltoDiagonalDER)
                    this->personajeJuego->activarAccion(SALTARDIAGONAL_DER);
                else
                    this->personajeJuego->activarAccion(SALTARDIAGONAL_IZQ);

                break;
            default:
                this->personajeJuego->activarAccion(QUIETO);
        }
        if (scrollearIzquierda){
                if (x_logico_personaje >= 0 && ((x_logico_personaje - MOVER_PIXELES)>=0)) x_logico_personaje -= MOVER_PIXELES;
                if ((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho < ANCHO_FISICO*(100-parser->margen)/200)
                {
                    //x_logico_personaje = x_logico_personaje + MOVER_PIXELES;
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                    if (borde_izquierdo_logico_pantalla<0 && (borde_izquierdo_logico_pantalla - MOVER_PIXELES)<0){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;
                        //this->personajeJuego->activarAccion( QUIETO);
                    }
                }
                // mover+= 5;


            } else if (scrollearDerecha){
				//cout<<"Posicion logica del personaje: "<<x_logico_personaje<<endl;
				//cout<<"Ancho escenario: "<<parser->escenario_ancho<<endl;
				//cout<<"Personaje ancho: "<<parser->personaje_ancho<<endl;

                 if (x_logico_personaje <= parser->escenario_ancho - parser->personaje_ancho)
					x_logico_personaje += MOVER_PIXELES;

                 if ((x_logico_personaje + (parser->personaje_ancho) - borde_izquierdo_logico_pantalla)> (parser->ventana_ancho -parser->ventana_ancho*(100-parser->margen)/200))
                {
                    borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;
                    if (borde_izquierdo_logico_pantalla + (parser->ventana_ancho) >= parser->escenario_ancho){
                        borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                    }
                }
            }

            while (x_logico_personaje > (parser->escenario_ancho - parser->personaje_ancho)){
                x_logico_personaje -= MOVER_PIXELES;
            }
            this->timer->avanzarTimer(SDL_GetTicks());
   
		};          
*/
        

};  //FIN CLASE JUEGO

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
