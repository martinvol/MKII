#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
#include "Director/DirectorPractica.hpp"
#include "AI/AI.hpp"
#include "Menu/Menu.hpp"
#include "Menu/ControladorMenu.hpp"
#include "Menu/GrillaDeJugadores.hpp"
#include "Menu/TextBox.hpp"

// Música
#include <SDL2/SDL_mixer.h>


using namespace std;

#define MOVER_PIXELES 80*(parser->personaje_ancho/parser->ventana_ancho)
#define MOVER_PIXELES_VERTICAL 75*(parser->personaje_alto/parser->escenario_ancho)
#define FRAMERATE 40

#define IMG_DEFAULT "resources/miscelaneo/06.png"
#define CONST_MAXI_DELAY 18

#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000

#define USING_PATH_JSON true

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
    if (SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0){
		logger->log_error("No se pudo inizializar SDL_Joystick");
		return 1;
	}
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        logger->log_error("No se pudo inizializar SDL_mixer");
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return 1;
    }
    TTF_Init();
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
	
	float tiempoTotal; // EN SEGUNDOS.
	float tiempoUnidad;
	float tiempoActual;
	bool tiempo_agotado = false;
	bool Personaje_1_GanoRound = false;
	bool Personaje_2_GanoRound = false;
	bool Personaje_1_Gano_2_Round = false;
	bool Personaje_2_Gano_2_Round = false;
    bool golpeandoPJ = false;
    bool golpeandoPJalta = false;
    bool golpeandoPJbaja = false;
    bool cansandoPJ = false;
    bool arrojandoPk=false;
    SDL_Texture* splash;
    
    Sint16 presionado=0;
    SDL_Rect r;
    double t = 5.0;
    bool pausa = false;
    bool salir = false, salir_pelea = false;
    Menu* menu;
    ControladorMenu* controlador;
    modo modo_a_cambiar;
    bool cambiarModo = false;
    float timerFps;

    int argc;
    char** argv;

    bool usandoJoystick = false;
    SDL_Joystick *Player1, *Player2;
    SDL_JoystickID myID = -1;
    int x_Joystick, y_Joystick;

	modo modo_actual = MENU;
    
    SDL_Renderer * renderer = NULL;

    Parser* parser;
    
    float x_logico_personaje, x_logico_personaje2;
    float borde_izquierdo_logico_pantalla;

	Escenario* escenario = NULL;
	Ventana* ventana = NULL;
    Timer* timer = NULL;

    ConversorDeCoordenadas* conversor;
	Grilla* grilla = NULL;
	Estado* estado1 = NULL, *estado2 = NULL;
    Personaje* personajeJuego1 = NULL, *personajeJuego2 = NULL;
    string pathPersonaje1 = "", pathPersonaje2 = "";
    string nombrePersonaje1 = "", nombrePersonaje2 = "";
    bool USAR_AI = false;
    AI* ai = NULL;
    BarraDeVida* barraDeVida1 = NULL, *barraDeVida2 = NULL;

	Director* director = NULL;

    Mix_Music *musica_fondo;

//----------------------------------------------------------------

    Juego(int argc_, char* argv_[]){
        argc = argc_;
        argv = argv_;
    };

//----------------------------------------------------------------
    int jugar(){


        if (InicializarSDL() != 0) return 1;
        renderer = SDL_CreateRenderer(NULL, -1, 0);

        cargar_configuracion();
        
        game_loop();
        
        // LIBERAR RECURSOS
        terminar_juego();
        terminar_sdl();
        Logger::destroy_instance();
        return 0;
    };
//----------------------------------------------------------------

    void cargar_configuracion(){
		this->parser = new Parser();
		
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
        
        this->ventana = new Ventana("Mortal Kombat 3 Ultimate", parser->ventana_anchopx, parser->ventana_altopx, parser->margen);
        renderer = SDL_CreateRenderer(ventana->window, -1, SDL_RENDERER_SOFTWARE);
        
        // Separacion entre personajes de un tercio de la ventana.
        float SEPARACION = parser->ventana_ancho / 3.0;
        
    // PERSONAJE 1
		// Cargamos al personaje en el medio de la ventana, pero del lado izquierdo.
		// Este x_logico es del extremo izquierdo del personaje.
        x_logico_personaje = (parser->escenario_ancho/2.) - (parser->personaje_ancho) - SEPARACION/2;


	// PERSONAJE 2
		// Cargamos al personaje en el medio de la ventana, pero del lado derecho.
		// Este x_logico es del extremo izquierdo del personaje.
		x_logico_personaje2 = (parser->escenario_ancho/2.) + SEPARACION/2;

        if (SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0){
		logger->log_error("No se pudo inizializar SDL_Joystick");
		//return 1;
		}
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");        
        
        
        Player1 = SDL_JoystickOpen(0);
        Player2 = SDL_JoystickOpen(1);
        SDL_JoystickID myID = SDL_JoystickInstanceID(Player1);
        ///Por defecto es 0
        ///Si se desconecta es un -1        
        ///cout<<myID<<endl;				
        
		if (Player1 == NULL ){
			logger->log_error("Player 1 JOYSTICK desconectado");			
        }else{
            usandoJoystick = true;
            logger->log_debug("Player 1 JOYSTICK conectado");            
        }
        
        if (Player2 == NULL ){
			logger->log_error("Player 2 JOYSTICK desconectado");			
        }else{
            usandoJoystick = true;
            logger->log_debug("Player 2 JOYSTICK conectado");            
        }
    
    musica_fondo = Mix_LoadMUS("resources/music/mision.ogg");
    if(musica_fondo == NULL){
        printf("Falló SDL_mixer, Error: %s\n", Mix_GetError());
    }

    if(Mix_PlayMusic(musica_fondo, -1) == -1){
        logger->log_debug("Error al cargar la música");
        printf("Mix_PlayMusic: %s\n", Mix_GetError());

    }

    tiempoTotal = this->parser->tiempoTotal; 
    tiempoUnidad = tiempoTotal * 10;
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

        //uno solo...por ahora (?)
        //SDL_JoystickEventState (SDL_QUERY);
        Uint32 t1;

        splash = IMG_LoadTexture(renderer, "resources/background/splash.jpg");
        SDL_Rect source_rect;
        source_rect.x = 0;
        source_rect.y = 0;
        source_rect.w = parser->ventana_anchopx;
        source_rect.h = parser->ventana_altopx;
        //DibujarTodo();

        SDL_RenderClear(renderer);


        SDL_Event evento;

        for (int j=0; j<30; j++){
            // ESTO ESTA MUY MAL
            // PERO LO HIZO VOLPE PARA QUE EL JOYSTICK NO SE TRABE
            // SI ENTREGAMOS CON LA COMPUTADORA DE OTRO, HAY QUE SACARLO
            timerFps = SDL_GetTicks();
            //~ SDL_JoystickClose(Player1);
            //~ SDL_JoystickClose(Player2);
            //~ Player1 = SDL_JoystickOpen(0); 
            //~ Player2 = SDL_JoystickOpen(1); 
            // lo pongo acá en el splash y disimulo
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, splash, &source_rect, NULL);
            //SDL_Delay((1.*1000./CONST_MAXI_DELAY));
            SDL_RenderPresent(renderer);
            timerFps = SDL_GetTicks() - timerFps;
            
            if(timerFps < 1.*1000./CONST_MAXI_DELAY){
                SDL_Delay((1.*1000./CONST_MAXI_DELAY)-timerFps);
            }
            SDL_FlushEvent(SDL_KEYDOWN);

            Controlador(&evento);

        }
        
        SDL_RenderClear(renderer);
        SDL_DestroyTexture(splash);

		this->grilla = new Grilla(renderer, parser->ventana_anchopx, parser->ventana_altopx);
		
        menu = new Menu (renderer, ventana);
        SDL_RenderClear(renderer);
        menu->Dibujarse();
        SDL_RenderPresent(renderer);
        controlador = new ControladorMenu(menu);
        while (!salir){
            SDL_FlushEvent(SDL_KEYDOWN);
            ControladorBasico(&evento);
            if(usandoJoystick){     
                SDL_JoystickUpdate();
            }
            
			salir_pelea = false;
            modo_a_cambiar = controlador->procesarEvento(&evento);
            SDL_RenderClear(renderer);
            menu->Dibujarse();
            SDL_RenderPresent(renderer);
            if (modo_a_cambiar == Pelea) {
                modo_actual = Pelea;
                logger->log_debug("Debería pasar a: Pelea"); ///
                elegir_personajes(Pelea);
                if (this->grilla->eligio[0] && this->grilla->eligio[1]) {
					comenzar_escenario_de_pelea();
					crear_personajes();
					//Inicio el countdown.
					tiempoActual = SDL_GetTicks();
					pelear(&evento); 
				}     
                salir_de_modo();
            } else
            if (modo_a_cambiar == Practica) {
                modo_actual = Practica;
                logger->log_debug("Debería pasar a: Practica"); ///
                // Por ahora repito todo.
                elegir_personajes(Practica);
                if (this->grilla->eligio[0] && this->grilla->eligio[1]) {
					comenzar_escenario_de_pelea();
					crear_personajes_practica();
					pelear(&evento);
				}
                salir_de_modo();
            } else
            if (modo_a_cambiar == CPU) {
                modo_actual = CPU;
                USAR_AI = true;
                logger->log_debug("Debería pasar a: CPU"); ///
                // Por ahora repito todo.
                elegir_personajes(CPU);
                if (this->grilla->eligio[0] && this->grilla->eligio[1]) {
					comenzar_escenario_de_pelea();
					crear_personajes();
					//Inicio el countdown.
					tiempoActual = SDL_GetTicks();
					pelear(&evento);
				}
                salir_de_modo();
                USAR_AI = false;
            }
		}
}

void setearNombres(){
	nombrePersonaje1 = this->grilla->textbox1->obtenerTexto();
	nombrePersonaje2 = this->grilla->textbox2->obtenerTexto();
	cout << "Nombre1: " << nombrePersonaje1 << endl; ///
	cout << "Nombre2: " << nombrePersonaje2 << endl; ///
    if (barraDeVida1 && barraDeVida2){
        barraDeVida1->setNombre(nombrePersonaje1);
        barraDeVida2->setNombre(nombrePersonaje2);
    }
}

void salir_de_modo(){
    modo_actual  = MENU;
    modo_a_cambiar = MENU;
    delete menu;
    Menu* menu = new Menu (renderer, ventana);
    delete controlador;
    ControladorMenu* controlador = new ControladorMenu(menu);
    delete director;
    director = NULL;
    SDL_Delay(100);
}
		
void pelear(SDL_Event* evento){
    while (!salir_pelea){					

        timerFps = SDL_GetTicks();
        Controlador(evento);       //Controlador
        if (!pausa){
            
            // t1 = SDL_GetTicks();
            ActualizarModelo(personajeJuego1);     //Modelo 
            ActualizarModelo(personajeJuego2);
            this->director->actualizar();                


        }
        DibujarTodo();              //Vista
        SDL_FlushEvent(SDL_KEYDOWN);

        //SDL_Delay(150);
        timerFps = SDL_GetTicks() - timerFps;
        
        if(timerFps < 1.*1000./CONST_MAXI_DELAY){
            SDL_Delay((1.*1000./CONST_MAXI_DELAY)-timerFps);
        }

		if (director->seMurio(0)){
            logger->log_debug(string("Ganó el jugador: ") + parser->personaje2_nombre + string("!!!"));
            director->GanoRound(1); 
            modo_actual = Pelea;
            
            //Si ya tenia ganado un round, ahora gano el segundo
            if (Personaje_1_GanoRound){
				Personaje_1_Gano_2_Round = true;
				logger->log_debug(string("Ganó la PARTIDA el jugador: ") + parser->personaje2_nombre + string("!!!"));
				salir_pelea = true;
			}else{
				Personaje_1_GanoRound = true;	
			}			
			ArmarRound();
            
            
        } if (director->seMurio(1)){
            logger->log_debug(string("Ganó el jugador: ") + parser->personaje1_nombre + string("!!!"));
            director->GanoRound(0);
            modo_actual = Pelea;     
            
            //Si ya tenia ganado un round, ahora gano el segundo
            if (Personaje_2_GanoRound){
				Personaje_2_Gano_2_Round = true;
				logger->log_debug(string("Ganó LA PARTIDA el jugador: ") + parser->personaje1_nombre + string("!!!"));
				salir_pelea = true;
			}else{
				Personaje_2_GanoRound = true;	
			}            
			ArmarRound();   
        }
        
        //Si no se murio ninguno, pero se acabo el tiempo, gana el que pego mas.
        //falta...
        if (tiempo_agotado){	
			logger->log_debug("Tiempo de round agotado");
			if (director->ObtenerVida(0) > director->ObtenerVida(1)){
			//Repito la logica para quien gana.
				logger->log_debug(string("Ganó (por tener mayor cantidad de vida) el jugador: ") + parser->personaje1_nombre + string("!!!"));
				director->GanoRound(0);
				modo_actual = Pelea;     
            
				//Si ya tenia ganado un round, ahora gano el segundo
				if (Personaje_2_GanoRound){
					Personaje_2_Gano_2_Round = true;
					logger->log_debug(string("Ganó LA PARTIDA el jugador: ") + parser->personaje1_nombre + string("!!!"));
					salir_pelea = true;
				}else{
					Personaje_2_GanoRound = true;	
				}            
				
			}else if (director->ObtenerVida(0) < director->ObtenerVida(1)){
			//Repito la logica para quien gana.
				logger->log_debug(string("Ganó (por tener mayor cantidad de vida) el jugador: ") + parser->personaje2_nombre + string("!!!"));
				director->GanoRound(1); 
				modo_actual = Pelea;
            
				//Si ya tenia ganado un round, ahora gano el segundo
				if (Personaje_1_GanoRound){
					Personaje_1_Gano_2_Round = true;
					logger->log_debug(string("Ganó la PARTIDA el jugador: ") + parser->personaje2_nombre + string("!!!"));
					salir_pelea = true;
				}else{
					Personaje_1_GanoRound = true;	
				}	
				
			//Si ninguno se pego:
			//*	Como el truco, gana el que hizo primera.
			//*	Si los dos ganaron un round, va de nuevo (al desempate)
			//* Si ninguno gano un round, va de nuevo, como si nada.
			}else {
				if (Personaje_1_GanoRound && !Personaje_2_GanoRound){
					logger->log_debug(string("Ganó la PARTIDA (1 round + empate) el jugador: ") + parser->personaje2_nombre + string("!!!"));
					salir_pelea = true;
				}else if (Personaje_2_GanoRound && !Personaje_1_GanoRound){
					logger->log_debug(string("Ganó LA PARTIDA (1 round + empate) el jugador: ") + parser->personaje1_nombre + string("!!!"));
					salir_pelea = true;
				}else{
					logger->log_debug("Empate sin datos suficientes para declarar ganador. Se repite el round");
					ArmarRound();   
				}
				
			}
			//Reestablezco timer
			tiempo_agotado = false;
			tiempoActual = SDL_GetTicks();
			timer->reset(tiempoActual);
			ArmarRound();   						
		}
        
    }    
        
    Personaje_1_GanoRound = Personaje_2_GanoRound = false;
	Personaje_1_Gano_2_Round = Personaje_2_Gano_2_Round = false;
}

//--------------------------------------------
void ArmarRound(){	
	//Reinicio posicion PJ1
	delete this->personajeJuego1->coordenada;
	delete this->personajeJuego1->siguiente;
	this->personajeJuego1->y_inicial = parser->escenario_ypiso;
	this->personajeJuego1->coordenada = new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso);
	this->personajeJuego1->siguiente = new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso);
	
	//Reinicio posicion PJ2
	delete this->personajeJuego2->coordenada;
	delete this->personajeJuego2->siguiente;	
	this->personajeJuego2->y_inicial = parser->escenario_ypiso;
	this->personajeJuego2->coordenada = new CoordenadaLogica(x_logico_personaje2, parser->escenario_ypiso);
	this->personajeJuego2->siguiente = new CoordenadaLogica(x_logico_personaje2, parser->escenario_ypiso);
	
	//Reestablezco vidas
	this->barraDeVida1->Resetear();
	this->barraDeVida2->Resetear();
	
	//Reinicio timer
	tiempoActual = SDL_GetTicks();
	timer->reset(tiempoActual);
	
}
//-------------------------------------------    
//------CONTROLADOR DE PAUSA Y SALIR---------
//-------------------------------------------    
void ControladorBasico(SDL_Event* evento){
	if (usandoJoystick) SDL_JoystickUpdate();
	switch(evento->type){			
		case SDL_QUIT:
            if (modo_actual == MENU && !salir_pelea){
                puts("salir en true 1");
                salir = true;
            } else {
                salir_pelea = true;
            }
			break;
		case SDL_KEYDOWN:
			if (evento->key.keysym.sym == SDLK_p)  {
				if (modo_actual==Pelea || modo_actual == CPU || modo_actual ==Practica )
					pausa = !pausa;
				true;
			}
			if (evento->key.keysym.sym == SDLK_ESCAPE){
				if (modo_actual == MENU && !salir_pelea){
                    puts("salir en true 2");
                    salir = true;
                }else {
                    salir_pelea = true;
                }
			}
		case SDL_KEYUP:
			if(evento->key.keysym.sym == SDLK_p)  {                   
				if (this->timer != NULL)
					this->timer->pausarTimer(SDL_GetTicks());
            }
        default:
			;
	}
}


//--------------------------------------------
//-------------COMENZAR UNA PELEA-------------
//--------------------------------------------
void elegir_personajes(modo seleccionMenu){
	this->grilla->eligio[0] = this->grilla->eligio[1] = false;
	if (seleccionMenu != Pelea) this->pathPersonaje2 = this->grilla->randomChoicePlayer2();
	this->grilla->open(this->menu->obtenerIDventana());
	if (this->grilla->eligio[0]){
		this->pathPersonaje1 = this->grilla->seleccionarOpcion(0);
	}
	if (this->grilla->eligio[1]) this->pathPersonaje2 = this->grilla->seleccionarOpcion(1);
	cout << this->pathPersonaje1 << endl ; ///
	cout << this->pathPersonaje2 << endl; ///
	
}


void comenzar_escenario_de_pelea(){
	this->escenario = new Escenario(parser->escenario_ancho, parser->escenario_alto);
	this->conversor = new ConversorDeCoordenadas(parser->ventana_altopx, parser->ventana_anchopx,
						 parser->escenario_alto, parser->ventana_ancho, borde_izquierdo_logico_pantalla);
	cargar_capas();
    
}


void crear_personajes(){
// En realidad recibiría nombre, personaje elegido y si es AI los del 2 serían los de defecto o azar o lo que sea.
    /* Personaje 1 - izquierda */
	if (USING_PATH_JSON) {
		this->estado1 = new Estado((string)(this->parser->sprites_map["personaje1"]),
							renderer, parser->personaje_alto, parser->escenario_alto,
							parser->personaje_ancho, parser->escenario_ancho, parser->ventana_ancho);
		}
	else this->estado1 = new Estado(this->pathPersonaje1,
							renderer, parser->personaje_alto, parser->escenario_alto,
							parser->personaje_ancho, parser->escenario_ancho, parser->ventana_ancho);
    this->personajeJuego1 = new Personaje(new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso),
                                    "Subzero", renderer, parser->personaje_alto,
                                    parser->personaje_ancho, estado1,
                                    this->conversor, parser->velocidad_arma,
                                    1, true, parser->tomas);
    this->barraDeVida1 = new BarraDeVida(0, parser->ventana_anchopx/2, parser->ventana_altopx, renderer, true);

    this->personajeJuego1->dibujar_botones(this->parser, false);
    /* Personaje 2 - derecha */
    // SI SON IGUALES, A UN ESTADO LE PASO LAS CONSTANTES. 
    // NOTAR QUE HAY DOS CONSTRUCTORES; UNO TOMA ESTOS VALORES POR
    // DEFECTO IGUAL A CERO.
    
    if ((this->parser->sprites_map["personaje1"] == this->parser->sprites_map2["personaje2"]) ||
		this->pathPersonaje1 == this->pathPersonaje2) {
		
        if (USING_PATH_JSON) 
			this->estado2 = new Estado((string)(this->parser->sprites_map["personaje1"]),
                        renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho, 
                        parser->color_inicio, parser->color_fin, parser->color_offset);
            else this->estado2 = new Estado(this->pathPersonaje2, renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho, 
                        parser->color_inicio, parser->color_fin, parser->color_offset);             
    }else {
        if (USING_PATH_JSON)
        this->estado2 = new Estado((string)(this->parser->sprites_map2["personaje2"]),
                        renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho);
        else this->estado2 = new Estado(this->pathPersonaje2, renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho);
        
	}
    
    this->personajeJuego2 = new Personaje(new CoordenadaLogica(x_logico_personaje2, parser->escenario_ypiso),
                                    "Segundo", renderer, parser->personaje2_alto,
                                    parser->personaje2_ancho, estado2,
                                    this->conversor, parser->velocidad_arma,
                                    2, false, parser->tomas);
    
    barraDeVida2 = new BarraDeVida(parser->ventana_anchopx/2, parser->ventana_anchopx, parser->ventana_altopx, renderer, false);

    if (USAR_AI) this->ai = new AI(this->personajeJuego2, this->personajeJuego1);
            
    director = new Director(this->escenario, this->ventana, this->conversor, this->parser->escenario_ypiso, this->personajeJuego1, this->personajeJuego2, barraDeVida1, barraDeVida2, this->timer);
    puts("creo un director normal");

    this->timer = new Timer(100, IMG_DEFAULT, conversor, renderer);
    this->timer->reset(SDL_GetTicks());
    
    setearNombres();

}

void crear_personajes_practica(){
// En realidad recibiría nombre, personaje elegido y si es AI los del 2 serían los de defecto o azar o lo que sea.
    /* Personaje 1 - izquierda */
    this->estado1 = new Estado((string)(this->parser->sprites_map["personaje1"]),
                        renderer, parser->personaje_alto, parser->escenario_alto,
                        parser->personaje_ancho, parser->escenario_ancho, parser->ventana_ancho);
    this->personajeJuego1 = new Personaje(new CoordenadaLogica(x_logico_personaje, parser->escenario_ypiso),
                                    "Subzero", renderer, parser->personaje_alto,
                                    parser->personaje_ancho, estado1,
                                    this->conversor, parser->velocidad_arma,
                                    1, true, parser->tomas);

    this->personajeJuego1->dibujar_botones(this->parser, true);

    /* Personaje 2 - derecha */
    // SI SON IGUALES, A UN ESTADO LE PASO LAS CONSTANTES. 
    // NOTAR QUE HAY DOS CONSTRUCTORES; UNO TOMA ESTOS VALORES POR
    // DEFECTO IGUAL A CERO.

    
    if (this->parser->sprites_map["personaje1"] == this->parser->sprites_map2["personaje2"])
        this->estado2 = new Estado((string)(this->parser->sprites_map["personaje1"]),
                        renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho, 
                        parser->color_inicio, parser->color_fin, parser->color_offset);             
    else
        this->estado2 = new Estado((string)(this->parser->sprites_map2["personaje2"]),
                        renderer, parser->personaje2_alto, parser->escenario_alto,
                        parser->personaje2_ancho, parser->escenario_ancho, parser->ventana_ancho);
    
    this->personajeJuego2 = new Personaje(new CoordenadaLogica(x_logico_personaje2, parser->escenario_ypiso),
                                    "Segundo", renderer, parser->personaje2_alto,
                                    parser->personaje2_ancho, estado2,
                                    this->conversor, parser->velocidad_arma,
                                    2, false, parser->tomas);
    this->barraDeVida1 = NULL;
    
    barraDeVida2 = NULL;
    this->timer = NULL;
            
    director = new DirectorPractica(this->escenario, this->ventana, this->conversor, this->parser->escenario_ypiso, this->personajeJuego1, this->personajeJuego2, NULL, NULL, NULL);
    puts("creo un director práctica");

	nombrePersonaje1 = this->grilla->textbox1->obtenerTexto();
	nombrePersonaje2 = this->grilla->textbox2->obtenerTexto();
	cout << "Nombre1: " << nombrePersonaje1 << endl; ///
	cout << "Nombre2: " << nombrePersonaje2 << endl; ///
}


//----------------------------------------------------------------
//----------------------------------------------------------------
    void reiniciarJuego(){
        logger->log_debug("-------------------------------------");
        logger->log_debug("Vuelve a empezar el juego, cambiando las configuraciones");
        terminar_juego();        
		
		this->grilla = new Grilla(renderer, parser->ventana_anchopx, parser->ventana_altopx);
		
        cargar_configuracion();
        comenzar_escenario_de_pelea();
        
        if ((modo_actual == Pelea)
            // Por ahora
            || (modo_actual == CPU)){
            crear_personajes();

        } else {
			// || (modo_actual == Practica)
            crear_personajes_practica();
        }
	if(Personaje_1_GanoRound)
		director->GanoRound(1); 
	
	if(Personaje_2_GanoRound)
		director->GanoRound(0); 
    };

    void terminar_juego(){
        SDL_JoystickClose(Player1);
        SDL_JoystickClose(Player2);
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
        delete this->parser;	// Elimina sus propias capas.
        delete this->director;
        director = NULL;
        // Elimina, conversor, jugadores (personajes y barras de vida), timer, escenario
		delete this->ventana;
        if (this->ai != NULL) delete this->ai;
        Mix_FreeMusic(musica_fondo);
		delete this->grilla;
        SDL_DestroyRenderer(renderer);
        logger->log_debug("Borramos todos los objetos");
    };

    void terminar_sdl(){
        SDL_DestroyRenderer(renderer);
        IMG_Quit();
        SDL_Quit();
    };


void DibujarTodo(){
        //Limpio y dibujo
        
        SDL_RenderClear(renderer);
		
        SDL_Rect fillRect = {0, 0, parser->ventana_anchopx, parser->ventana_altopx};
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect(renderer, &fillRect );

        for (unsigned int i = 0; i < escenario->capas.size(); i++){
        // Itero hacia en orden inverso
        // Así respeto los Z index

            // Estas son coordenadas lógicas, que por adentro capas las cambia a físicas
            // esa cuenta cancha la debería hacer por afuera, pero comofunciona, por ahora la dejo
			(escenario->capas[i])->DibujarseAnchoReal(this->ventana->obtenerBordeLogicoIzquierdo(this->conversor), 0);

			// Si el z_index del personaje está entre medio de las capas:
            if (i==parser->personaje_zindex){
                this->personajeJuego1->Dibujarse();
            }
            if (i==parser->personaje2_zindex){
				this->personajeJuego2->Dibujarse();
            }
        }
		// Si no hay capaz o el z_index del personaje supera al indice de la ultima capa, lo debo imprimir ahora:
        if (escenario->capas.size()==0 || parser->personaje_zindex >= (escenario->capas.size())){
			this->personajeJuego1->Dibujarse();
		}
        if (escenario->capas.size()==0 || parser->personaje2_zindex >= (escenario->capas.size())){
            this->personajeJuego2->Dibujarse();
        }
		
        if (this->barraDeVida1){
            this->barraDeVida1->Dibujarse();           
        }

        if (this->barraDeVida2){
            this->barraDeVida2->Dibujarse();
        }

        if (this->timer){
            this->timer->Dibujarse();
        }
		
        if (pausa){
            //~ SDL_Rect pantalla = {0,0,parser->ventana_anchopx,parser->ventana_altopx};
            //~ SDL_SetRenderDrawColor( renderer, 0, 0, 0, 200 );
            //~ SDL_RenderFillRect( renderer, &pantalla );
        }
        SDL_RenderPresent(renderer);
};


/* El de ahora debería ser avisándole al director */
void Controlador(SDL_Event *evento){		
	//myID = SDL_JoystickInstanceID(Player1);
	///cout<< myID<<endl;			
		
	while(SDL_PollEvent( evento )) {
		if(usandoJoystick){		
			SDL_JoystickUpdate();
			//controlar_joystick();
			if (personajeJuego1 != NULL) personajeJuego1->ActualizarControlador(Player1, this->parser, evento);	
            if (modo_actual != Practica && modo_actual != CPU){
                if (personajeJuego2 != NULL) personajeJuego2->ActualizarControlador(Player2 , this->parser, evento);                    
            }				
			
		} 
		//-----------------------------------------
		//-----EVENTOS NO-JOYSTICK (aka DEBUG)-----
		//--------"--------"-------------------------		
		switch(evento->type){			
			case SDL_QUIT:
				if (modo_actual == MENU){
                    salir = true;
                } else {
                    salir_pelea = true;
                }
				break;
			case SDL_KEYDOWN:
				//-----------------------------------------
				//-----------------------------------------
				//---------------BASICOS-------------------
				if (evento->key.keysym.sym == SDLK_UP)  {
					personajeJuego2->Arriba = true;					
				}
				if (evento->key.keysym.sym == SDLK_RIGHT)  {
					personajeJuego2->Derecha = true;										
				}
				if (evento->key.keysym.sym == SDLK_DOWN)  {
					personajeJuego2->Abajo = true;										
				}
				if ((evento->key.keysym.sym == SDLK_LEFT))  {
					personajeJuego2->Izquierda = true;					
				}
				//-----------------------------------------
				//-----------------------------------------
				if (evento->key.keysym.sym == SDLK_p)  {
					pausa = !pausa;
					if (pausa)
						logger->log_debug("Pausa activada");
					else
						logger->log_debug("Pausa desactivada");
				}
				if(evento->key.keysym.sym == SDLK_c)  {
					if (cansandoPJ == false){
						cansandoPJ = true;
					}
				}
				if(evento->key.keysym.sym == SDLK_d)  {
					if (golpeandoPJ == false){
				        this->escenario->Temblar(SDL_GetTicks());
						golpeandoPJ = true;
					}
					break;
				}
				if(evento->key.keysym.sym == SDLK_s)  {
					golpeandoPJalta=true;
				}
				if(evento->key.keysym.sym == SDLK_f)  {
					golpeandoPJbaja=true;
				}
                if(evento->key.keysym.sym == SDLK_e)  {
                    arrojandoPk=true;
                }
				if (evento->key.keysym.sym == SDLK_ESCAPE){
                    if (modo_actual == MENU){
                        salir = true;
                    } else {
                        salir_pelea = true;
                    }
                }
				if (evento->key.keysym.sym == SDLK_r){
					reiniciarJuego();
				}
				///MILE
				///Cubrirse
				if (evento->key.keysym.sym == SDLK_g){
					personajeJuego2->CubrirAlto = true;	
				}
				///PatadaAlta
				if (evento->key.keysym.sym == SDLK_x){
					personajeJuego2->PatadaAlta = true;	
				}
				///PatadaBaja
				if (evento->key.keysym.sym == SDLK_l){
					personajeJuego2->PatadaBaja = true;	
				}
				break;
			case SDL_KEYUP:
                //-----------------------------------------
                //-----------------------------------------
                //---------------BASICOS-------------------
                if((evento->key.keysym.sym == SDLK_UP))  {
					personajeJuego2->Arriba = false;					                    
                }
                if((evento->key.keysym.sym == SDLK_DOWN))  {
					personajeJuego2->Abajo = false;					                    
                }
                if((evento->key.keysym.sym == SDLK_LEFT))  {
                    personajeJuego2->Izquierda = false;					
                }
                if((evento->key.keysym.sym == SDLK_RIGHT))  {
                    personajeJuego2->Derecha = false;					
                }
                //-----------------------------------------
                //-----------------------------------------
                if(evento->key.keysym.sym == SDLK_p)  {   
					if (timer != NULL)                
						this->timer->pausarTimer(SDL_GetTicks());
                }
                if((evento->key.keysym.sym == SDLK_d))  {
                    golpeandoPJ = false;
                }
                if((evento->key.keysym.sym == SDLK_c))  {
                    cansandoPJ = false;
                }
                if((evento->key.keysym.sym == SDLK_s))  {
                    golpeandoPJalta = false;
                }
                if((evento->key.keysym.sym == SDLK_f))  {
                    golpeandoPJbaja = false;
                }
                if(evento->key.keysym.sym == SDLK_e)  {
                    arrojandoPk=false;
                }
                ///MILE
                ///Cubrirse
                if(evento->key.keysym.sym == SDLK_g){
					personajeJuego2->CubrirAlto = false;
				}
				///PatadaAlta
				if (evento->key.keysym.sym == SDLK_x){
					personajeJuego2->PatadaAlta = false;	
				}
				///PatadaBaja
				if (evento->key.keysym.sym == SDLK_l){
					personajeJuego2->PatadaBaja = false;	
				}
                break;
			default:
				;
		} // FIN eventos no joystick
	
	} // Termina de registrar eventos.

}

void ActualizarModelo(Personaje* personaje){
	
	if ((this->ai != NULL ) && (personaje == this->ai->personajeAI)) this->ai->reaccionar();
	

    float tiempoAuxiliar = SDL_GetTicks();	
	bool avanzarTimer = false;
    if (this->timer){
    	if (tiempoAuxiliar > (tiempoActual + tiempoUnidad)){
    		tiempoActual = tiempoAuxiliar;
    		avanzarTimer = true;
    	}
    }
	
    if (this->timer != NULL && avanzarTimer){
    	this->timer->avanzarTimer(SDL_GetTicks()); // El Timer no iria en el director ? *Manu*
    }
    if (this->timer != NULL && this->timer->terminoElTiempo()){			
			tiempo_agotado = true;
	}
	
	if (personaje->nroAccionActual == PATADABAJAAGACHADO){
		personaje->activarAccion(PATADABAJAAGACHADO);
		return;
	}else if (personaje->nroAccionActual == PATADAALTAAGACHADO){
		personaje->activarAccion(PATADAALTAAGACHADO);
		return;
	}else if (personaje->nroAccionActual == GANCHO) {
		personaje->activarAccion(GANCHO);	
		return;
	} 	
	
//DERECHA		
	if (personaje->Derecha){
		//+ARRIBA = SALTO DIAGONAL DERECHA
		if (personaje->Arriba){
			if (personaje->PiniaAlta or personaje->PiniaBaja){				
				personaje->activarAccion(PINIASALTANDODIAGONAL);
				personaje->PiniaAlta = false;
				personaje->PiniaBaja = false;
			}
			else if (personaje->PatadaAlta or personaje->PatadaBaja){				
				personaje->activarAccion(PATADASALTANDODIAGONAL);
				personaje->PatadaAlta = false;
				personaje->PatadaBaja = false;
			}else{				
				personaje->activarAccion(SALTARDIAGONAL_DER);				
			}
		}
		//+ABAJO = ABAJO DERECHA
		else if (personaje->Abajo){
			personaje->activarAccion(AGACHARSE);			
		}
		//+PATADA BAJA+MIRA DERECHA = TRABA
		else if (personaje->PatadaBaja && !personaje->mirarDerecha){			
			personaje->activarAccion(TRABA);
			personaje->PatadaBaja = false;			
		}
		else if (personaje->PatadaAlta && !personaje->mirarDerecha){
			personaje->activarAccion(ROUNDKICK);
		//CAMINAR DERECHA
		}else {			
			personaje->activarAccion(CAMINAR_DERECHA);			
		}
//IZQUIERDA
	} else if (personaje->Izquierda) {
		//+ARRIBA = SALTO DIAGONAL IZQUIERDA
		if (personaje->Arriba){
			if (personaje->PiniaAlta or personaje->PiniaBaja){				
				personaje->activarAccion(PINIASALTANDODIAGONAL);
				personaje->PiniaAlta = false;
				personaje->PiniaBaja = false;
			}
			else if (personaje->PatadaAlta or personaje->PatadaBaja){								
				personaje->activarAccion(PATADASALTANDODIAGONAL);
				personaje->PatadaAlta = false;
				personaje->PatadaBaja = false;
			}else{				
				personaje->activarAccion(SALTARDIAGONAL_IZQ);					
			}
		}		
		//+PATADA BAJA+MIRA DERECHA = TRABA
		else if (personaje->PatadaBaja && personaje->mirarDerecha){
			personaje->activarAccion(TRABA);
			personaje->PatadaBaja = false;			
		}else if (personaje->PatadaAlta && personaje->mirarDerecha){
			personaje->activarAccion(ROUNDKICK);
			personaje->PatadaAlta = false;			
		//+ABAJO = ABAJO IZQUIERDA
		}else if (personaje->Abajo){			
			personaje->activarAccion(AGACHARSE);			
		}
		//CAMINAR IZQUIERDA
		else {			
			personaje->activarAccion(CAMINAR_IZQUIERDA);			
		}
	//ARRIBA--> SALTA
	} else if (personaje->Arriba){
		if (personaje->PatadaAlta or personaje->PatadaBaja){			
			personaje->activarAccion(PATADASALTANDOVERTICAL);
			personaje->PatadaAlta = false;
			personaje->PatadaBaja = false;
		}else if(personaje->PiniaAlta or personaje->PiniaBaja){			
			personaje->activarAccion(PINIASALTANDOVERTICAL);
			personaje->PiniaAlta = false;
			personaje->PiniaBaja = false;
		}else{
			// Sólo va a ser saltar vertical porque sino hubiera entrado arriba y no sería un else.			
			personaje->activarAccion(SALTAR);			
		}
	//ABAJO -->AGACHARSE
	} else if (personaje->Abajo){
		// Sólo va a ser agacharse en el lugar porque sino hubiera entrado arriba y no sería un else.
		//+CUBRIR (OK LA VARIABLE NO DEBIERA LLAMARSE CUBRIRALTO, PERO QUEDA ASI)
		if(personaje->CubrirAlto){			
			personaje->activarAccion(CUBRIRBAJO);
		//+PATADA BAJA	
		}else if(personaje->PatadaBaja){														
			personaje->activarAccion(PATADABAJAAGACHADO);
			personaje->PatadaBaja = false;								
		//+PATADA ALTA
		}else if(personaje->PatadaAlta){			
			personaje->activarAccion(PATADAALTAAGACHADO);
			personaje->PatadaAlta = false;
		//+PINIA ALTA = GANCHO
		}else if (personaje->PiniaAlta){
			personaje->activarAccion(GANCHO);
			personaje->PiniaAlta = false;
		}else if(personaje->PiniaBaja){
			personaje->activarAccion(PINIAAGACHADO);
			personaje->PiniaBaja = false;
		}else{			
			personaje->activarAccion(AGACHARSE);			
		}
		
	}
	//PINIA BAJA
	else if (personaje->PiniaBaja){		
		personaje->activarAccion(PINIABAJA);
		personaje->PiniaBaja = false;
		//Una vez que la ejecuto, la desactivo, sino loopea.			
	//PINIA ALTA
	}else if (personaje->PiniaAlta){		
		personaje->activarAccion(PINIAALTA);
		personaje->PiniaAlta = false;
	//CUBRIRSE
	}else if (personaje->CubrirAlto){		
		personaje->activarAccion(CUBRIRALTO);
		/// Problema aca, a veces se queda trabado.
		//personaje->CubrirAlto = false;
	//ARROJAR ARMA
	}else if (personaje->ArrojarArma){		
		cout<<"ArrojarArma"<<endl; ///			
		//Una vez que la ejecuto, la desactivo, sino loopea.
		personaje->ArrojarArma = false;
		personaje->Arrojar();
	//PATADA BAJA
	}else if (personaje->PatadaBaja){
		//+IZQUIERDA = TRABA
		if(personaje->Izquierda){			
			puts("Traba a Implementar");
			personaje->activarAccion(TRABA);
			personaje->PatadaBaja = false;			
		}else{			
			personaje->activarAccion(PATADABAJA);
			personaje->PatadaBaja = false;
			//Una vez que la ejecuto, la desactivo, sino loopea.
		}
	//PATADA ALTA
	}else if (personaje->PatadaAlta){		
	personaje->activarAccion(PATADAALTA);
	personaje->PatadaAlta = false;			
	///MAXI QUE ES ESTO?
	/// MAGIA.	
	}	
    // ARROJABLE
    else if (arrojandoPk){        
		cout<<"ArrojarArma"<<endl; ///					
		personaje->ArrojarArma = false;
		personaje->Arrojar();
    }
	//MILE
	else if (golpeandoPJalta){		
		personaje->activarAccion(PINIAALTA);
		personaje->PiniaAlta = false;
	} else if (golpeandoPJbaja){		
		personaje->activarAccion(PINIABAJA);
		personaje->PiniaBaja = false;
	}else {		
		if (personaje->nroAccionActual == AGACHARSE){
			personaje->activarAccion(PARARSE);
		} else {
			personaje->activarAccion(QUIETO);
		}
	}		
}        

};  //FIN CLASE JUEGO

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
