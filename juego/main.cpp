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
#include "Ventana.hpp"
#include "Director.hpp"
#include "CoordenadaLogica.hpp"

using namespace std;

#define MOVER_PIXELES parser->ventana_anchopx/parser->personaje_ancho
#define MOVER_PIXELES_VERTICAL 3*(parser->ventana_altopx/parser->personaje_alto)
#define FACTOR_SCROLL MOVER_PIXELES

#define FRAMERATE 40
#define JOYSTICK_DEAD_ZONE 8000
#define ALTURA_MAX_SALTO parser->personaje_alto + parser->escenario_ypiso
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
    bool golpeandoPJ = false;
    bool cansandoPJ = false;
    
    Sint16 presionado=0;
    SDL_Rect r;
    double t = 5.0;
	float mileTmp;
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

	Estado* estado;
    Personaje* personajeJuego, *personajeJuego2;
    BarraDeVida* barraDeVida1, *barraDeVida2;
    ConversorDeCoordenadas* conversor;
	Escenario* escenario;
	Ventana* ventana;
	Director* director;
    
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
		
		// Cargamos al personaje y ventana en el medio del mapa.
        // x_logico del extremo izquierdo.
        x_logico_personaje = (parser->escenario_ancho/2.) - (parser->personaje_ancho/2.);
        borde_izquierdo_logico_pantalla = (parser->escenario_ancho/2.) - (parser->ventana_ancho/2.);

		r = {0, 0, parser->ventana_anchopx, parser->ventana_altopx};

		this->escenario = new Escenario(parser->escenario_ancho, parser->escenario_alto);
		this->conversor = new ConversorDeCoordenadas(parser->ventana_altopx, parser->ventana_anchopx,
                             parser->escenario_alto, parser->ventana_ancho, borde_izquierdo_logico_pantalla);
		this->ventana = new Ventana("Mortal Kombat 3 Ultimate", parser->ventana_anchopx, parser->ventana_altopx, parser->margen);
        
        renderer = SDL_CreateRenderer(ventana->window, -1, SDL_RENDERER_SOFTWARE);
        under = loadTexture("resources/background/p_under.png", renderer);
        cargar_capas();
        
        estado = new Estado((string)(this->parser->sprites_map["personaje1"]), renderer, parser->personaje_alto, parser->escenario_alto, parser->personaje_ancho, parser->escenario_ancho);
        Personaje* personaje = new Personaje(new CoordenadaLogica(x_logico_personaje,parser->escenario_ypiso),"Subzero",renderer, parser->personaje_alto, parser->personaje_ancho, estado, parser->personaje_mirar_derecha);
        this->personajeJuego = personaje;
        
        // fin de las Parseriguraciones
        
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void configurar(){
        this->parser = new Parser();
        cargar_configuracion(this->parser);
		
        //Izquierda
        barraDeVida1 = new BarraDeVida(0, parser->ventana_anchopx/2, parser->ventana_altopx, renderer, true);
       //Derecha
        barraDeVida2 = new BarraDeVida(parser->ventana_anchopx/2, parser->ventana_anchopx, parser->ventana_altopx, renderer, false);

		//~ personajeJuego2 = new PersonajeMock(parser->personaje_mirar_derecha, parser->escenario_ancho);

		director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, this->personajeJuego2, barraDeVida1, barraDeVida2, FACTOR_SCROLL);

        Player1 = SDL_JoystickOpen(0);
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");

    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void cargar_capas(){

        for (unsigned int i = 0; i < parser->capas_vector.size(); i++){
            //~ parser->capas_vector[i]->ren = renderer;
			//~ escenario->AgregarCapa(parser->capas_vector[i]);
            escenario->AgregarCapa( // esto no debería estar así, tendria que andar la lí­nea de arriba, pero estuve luchando y no la hago andar (maxi)
                new Capa (parser->capas_vector[i]->ubicacion,
                parser->capas_vector[i]->anchoLogico,
                parser->capas_vector[i]->x_logico,
                renderer,
                this->parser->escenario_ancho,
                this->parser->ventana_ancho
                )
            );
            delete parser->capas_vector[i];
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
            
            this->mileTmp = timerFps;
            Controlador(&evento);       				//Controlador
			if (!pausa) this->director->actualizar();	//Modelo
            DibujarTodo();              				//Vista

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
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        delete this->parser;
        delete this->director; // o sólo cambiar cosas?
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

            if (i==parser->personaje_zindex){
                CoordenadaFisica* coord = conv->aFisica(personajeJuego->obtenerCoordenadaIzqSup());
                this->personajeJuego->Dibujarse(
                    coord->x_fisico,
                    coord->y_fisico,
                    //~ (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    //~ (parser->escenario_alto - posicionPJ_Piso - (parser->personaje_alto))*(parser->ventana_altopx/parser->escenario_alto),
                    (parser->ventana_altopx/parser->escenario_alto)*parser->personaje_alto,
                    (parser->ventana_anchopx/parser->ventana_ancho)*parser->personaje_ancho);
				delete coord;
            }
        }

        if (escenario->capas.size()==0 || parser->personaje_zindex >= (escenario->capas.size())){
            // Si no hay capas;
            CoordenadaFisica* coord = conv->aFisica(personajeJuego->obtenerCoordenadaIzqSup());
			this->personajeJuego->Dibujarse(
					coord->x_fisico,
					coord->y_fisico,
                    //~ (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    //~ (parser->escenario_alto - posicionPJ_Piso - (parser->personaje_alto))*(parser->ventana_altopx/parser->escenario_alto),
                    (parser->ventana_altopx/parser->escenario_alto)*parser->personaje_alto,
                    (parser->ventana_anchopx/parser->ventana_ancho)*parser->personaje_ancho);
			delete coord;
        }
		
        barraDeVida1.Dibujarse();
        barraDeVida2.Dibujarse();
        //this->timer->Dibujarse();
		
        // CoordenadaFisica* c = conv->aFisica(new CoordenadaLogica(parser->personaje_ancho, parser->personaje_alto));
        if (pausa){
            SDL_Rect pantalla = {0,0,parser->ventana_anchopx,parser->ventana_altopx};
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 150 );
            SDL_RenderFillRect( renderer, &pantalla );
        }
        SDL_RenderPresent(renderer);
};

void Controlador(SDL_Event *evento){
	while(SDL_PollEvent( evento )) {

        //Ahora anda este tambien.
        /*if (evento->type == SDL_JOYBUTTONDOWN){
            ;
        }*/
        
		if(usandoJoystick){
			
			bool nada = false;
			
			x_Joystick = SDL_JoystickGetAxis(Player1, 0);
			y_Joystick = SDL_JoystickGetAxis(Player1, 1);
			
			if( x_Joystick < -JOYSTICK_DEAD_ZONE ){
				//  x = -1;
				this->director->seMuevePersonaje(jugador1, Izquierda);

			}else if( x_Joystick > JOYSTICK_DEAD_ZONE ){
				//  x =  1;
				this->director->seMuevePersonaje(jugador1, Derecha);
			}else{
				//  x = 0;
				nada = true;
			}

			//Vertical
			if( y_Joystick < -JOYSTICK_DEAD_ZONE ){
				//  y = -1;
				this->director->seMuevePersonaje(jugador1, Arriba);
			}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){
				//y =  1;
				//Abajo_PRESIONADO = true;
				;
			}else{
				//yDir = 0;
				nada = true;
			}
			if (nada) this->director->seMuevePersonaje(jugador1, Nada);
		
		} else {
			//-----------------------------------------
			//----------EVENTOS NO-JOYSTICK------------
			//-----------------------------------------
			bool nada1 = true;
			bool nada2 = true;
			
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
					if (nada1 && !pausa) this->director->seMuevePersonaje(jugador1, Nada);
					//-----------------------------------------
					//-----------------------------------------
					if (evento->key.keysym.sym == SDLK_p)  {
						//~ cambiarModo = true;
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
					//~ if((evento->key.keysym.sym == SDLK_p) && (cambiarModo))  {
						//~ cambiarModo = false;
						//~ pausa = !pausa;
					//~ }
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
		}
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
