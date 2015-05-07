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

#define MOVER_PIXELES 80*(parser->personaje_ancho/parser->ventana_ancho)
#define MOVER_PIXELES_VERTICAL 75*(parser->personaje_alto/parser->escenario_ancho)
#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000
#define IMG_DEFAULT "resources/miscelaneo/06.png"
#define CONST_MAXI_DELAY 50
#define FACTOR_SCROLL 20

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
    bool Arriba_PRESIONADO = false, Abajo_PRESIONADO = false, Izq_PRESIONADO = false, Der_PRESIONADO = false;
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
        
        this->estado = new Estado((string)(this->parser->sprites_map["personaje1"]),
							renderer, parser->personaje_alto, parser->escenario_alto,
							parser->personaje_ancho, parser->escenario_ancho, parser->ventana_ancho);
        this->personajeJuego = new Personaje(new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso),
										"Subzero", renderer, parser->personaje_ancho,
										parser->personaje_alto, estado,
										parser->personaje_mirar_derecha, this->conversor);

        this->estado2 = new Estado((string)(this->parser->sprites_map["personaje1"]),
                            renderer, parser->personaje_alto, parser->escenario_alto,
                            parser->personaje_ancho, parser->escenario_ancho, parser->ventana_ancho);
        this->personajeJuego2 = new Personaje(new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso),
                                        "Segundo", renderer, parser->personaje_ancho,
                                        parser->personaje_alto, estado2,
                                        parser->personaje_mirar_derecha, this->conversor);
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
/*<<<<<<< HEAD
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
        
       //Derecha
        barraDeVida2.Inicializar(conf->ventana_anchopx/2, conf->ventana_anchopx, conf->ventana_altopx, renderer, false);
        estado = new Estado((string)(this->conf->sprites_map["personaje1"]), renderer, conf->personaje_alto, conf->escenario_alto, conf->personaje_ancho, conf->escenario_ancho);
        Personaje* personaje = new Personaje(new CoordenadaLogica(x_logico_personaje,conf->escenario_ypiso),&barraDeVida1,"Subzero",renderer, conf->personaje_alto, conf->personaje_ancho, estado, conf);
        this->personajeJuego = personaje;
=======*/
        this->parser = new Parser();
        cargar_configuracion(this->parser);
		
		//~ // Para dos personajes:
		director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, this->personajeJuego2, barraDeVida1, barraDeVida2, FACTOR_SCROLL, this->timer);
		//~ // Con un personaje:
		//director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, barraDeVida1, FACTOR_SCROLL, this->timer);
//>>>>>>> remotes/origin/ClonarMaster

        Player1 = SDL_JoystickOpen(0);
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");
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
        logger->log_debug("Tengo que cambiar las Parseriguraciones");
        terminar_juego();
        
		Arriba_PRESIONADO = Izq_PRESIONADO = Der_PRESIONADO = Abajo_PRESIONADO  = false;
        configurar();

/*<<<<<<< HEAD
        estado = new Estado((string)(this->conf->sprites_map["personaje1"]), renderer, conf->personaje_alto, conf->escenario_alto, conf->personaje_ancho, conf->escenario_ancho);
        this->personajeJuego = new Personaje(new CoordenadaLogica(x_logico_personaje,conf->escenario_ypiso),&barraDeVida1,"Subzero",renderer, conf->personaje_alto, conf->personaje_ancho, estado, conf);
        cargar_capas();
        SDL_SetWindowSize(window, conf->ventana_anchopx, conf->ventana_altopx); // Dani se encarga de poner esto en su objeto
        this->personajeJuego->barraDeVida->Inicializar(0, conf->ventana_anchopx/2, conf->ventana_altopx, renderer, true);
        barraDeVida2.Inicializar(conf->ventana_anchopx/2, conf->ventana_anchopx, conf->ventana_altopx, renderer, false);
        this->timer->reset(SDL_GetTicks());
=======
>>>>>>> remotes/origin/ClonarMaster*/
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        delete this->parser;	// Elimina sus propias capas.
        logger->log_debug("termine de borrar todo");
        delete this->director; 	// Elimina, conversor, jugadores (personajes y barras de vida), timer, escenario, ventana
        logger->log_debug("termine de borrar todo");
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

        for (int i = 0; i < escenario->capas.size(); i++){
        // Itero hacia en orden inverso
        // Así respeto los Z index

			// Estas son coordenadas lógicas, que por adentro capas las cambia a físicas
			// esa cuenta cancha la debería hacer por afuera, pero comofunciona, por ahora la dejo
			(escenario->capas[i])->DibujarseAnchoReal(this->ventana->obtenerBordeLogicoIzquierdo(this->conversor), 0);

			// Si el z_index del personaje está entre medio de las capas:
            if (i==parser->personaje_zindex){
                this->personajeJuego->Dibujarse();
                this->personajeJuego2->Dibujarse();
			}
        }

//<<<<<<< HEAD
		//~ tthis->personajeJuego->barraDeVida.Dibujarse();
//        barraDeVida2.Dibujarse();
//=======
		// Si no hay capaz o el z_index del personaje supera al indice de la ultima capa, lo debo imprimir ahora:
        if (escenario->capas.size()==0 || parser->personaje_zindex >= (escenario->capas.size())){
			this->personajeJuego->Dibujarse();
            this->personajeJuego2->Dibujarse();
		}
		
        this->barraDeVida1->Dibujarse();
        this->barraDeVida2->Dibujarse();
//>>>>>>> remotes/origin/ClonarMaster

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
			}

			//Vertical
			if( y_Joystick < -JOYSTICK_DEAD_ZONE ){
				//  y = -1;
				Arriba_PRESIONADO = true;
			}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){
				//y =  1;
				Abajo_PRESIONADO = true;
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
					Arriba_PRESIONADO = true;
				}
				if (evento->key.keysym.sym == SDLK_RIGHT && !pausa)  {
					Der_PRESIONADO = true;
				}
				if ((evento->key.keysym.sym == SDLK_LEFT && !pausa))  {
					Izq_PRESIONADO = true;
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
                if(evento->key.keysym.sym == SDLK_p)  {
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
		} // FIN eventos no joystick
	
	} // Termina de registrar eventos.

}

void ActualizarModelo(){
		
	if (Der_PRESIONADO){
		if (Arriba_PRESIONADO){
			this->director->seMuevePersonaje(jugador1, ArribaDerecha);
		}
		else if (Abajo_PRESIONADO){
			this->director->seMuevePersonaje(jugador1, AbajoDerecha);
		}
		else {
			this->director->seMuevePersonaje(jugador1, Derecha);
		}
	} else if (Izq_PRESIONADO) {
		if (Arriba_PRESIONADO){
			this->director->seMuevePersonaje(jugador1, ArribaIzquierda);
		}
		else if (Abajo_PRESIONADO){
			this->director->seMuevePersonaje(jugador1, AbajoIzquierda);
		}
		else {
			this->director->seMuevePersonaje(jugador1, Izquierda);
		}
	} else if (Arriba_PRESIONADO){
		// Sólo va a ser saltar vertical porque sino hubiera entrado arriba y no sería un else.
		this->director->seMuevePersonaje(jugador1, Arriba);
	} else if (Abajo_PRESIONADO){
		// Sólo va a ser agacharse en el lugar porque sino hubiera entrado arriba y no sería un else.
		this->director->seMuevePersonaje(jugador1, Abajo);
	} else {
		this->director->seMuevePersonaje(jugador1, Nada);
	}
	
	this->director->actualizar();

}        

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
