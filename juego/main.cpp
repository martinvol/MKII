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

#define QUIETO 0
#define CAMINAR_DERECHA 1
#define CAMINAR_IZQUIERDA 2
#define SALTAR 3
#define SALTODIAGONAL 4
#define MOVER_PIXELES parser->ventana_anchopx/parser->personaje_ancho
#define FRAMERATE 60
#define JOYSTICK_DEAD_ZONE 8000

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
    double t = 1.0;
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

    

    unsigned int ANCHO_FISICO, ALTO_FISICO;
    unsigned int AnchoLogico, AltoLogico;

    Parser* parser;
    float x_logico_personaje;
    float borde_izquierdo_logico_pantalla;

    Personaje* personajeJuego, personajeJuego2;
    BarraDeVida* barraDeVida1, barraDeVida2;
    ConversorDeCoordenadas* conv;
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
		this->conv = new ConversorDeCoordenadas(parser->ventana_altopx, parser->ventana_anchopx,
                             parser->escenario_alto, parser->ventana_ancho, borde_izquierdo_logico_pantalla);
		this->ventana = new Ventana("Mortal Kombat 3 Ultimate", parser->ventana_anchopx, parser->ventana_altopx, parser->margen);
        
        renderer = SDL_CreateRenderer(ventana->window, -1, SDL_RENDERER_SOFTWARE);
        under = loadTexture("resources/background/p_under.png", renderer);
        cargar_capas();
        
        Personaje* personaje = new Personaje(new CoordenadaLogica(1,1),"Subzero",renderer, parser);
        this->personajeJuego = personaje;
        
        // fin de las Parseriguraciones
        
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void configurar(){
        this->parser = new Parser();
        cargar_configuracion(this->parser);
		
        //Izquierda
        barraDeVida1.Inicializar(0, parser->ventana_anchopx/2, parser->ventana_altopx, renderer, true);
       //Derecha
        barraDeVida2.Inicializar(parser->ventana_anchopx/2, parser->ventana_anchopx, parser->ventana_altopx, renderer, false);

		//~ personajeJuego2 = new PersonajeMock(parser->personaje_mirar_derecha, parser->escenario_ancho);

		director = new Director(this->escenario, this->ventana, this->conversor, this->personajeJuego, this->personajeJuego2, barraDeVida1, barraDeVida2);

        Player1 = SDL_JoystickOpen(0);
        SDL_SetHint("SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", "1");

    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void cargar_capas(){

        for (unsigned int i = 0; i < parser->capas_vector.size(); i++){
            //~ parser->capas_vector[i]->ren = renderer;
			//~ escenario->AgregarCapa(parser->capas_vector[i]);
            escenario->AgregarCapa( // esto no deberÃ­a estar asÃ­, tendria que andar la lÃ­nea de arriba, pero estuve luchando y no la hago andar (maxi)
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
            Controlador(&evento);       	//Controlador
			if (!pausa) this->director->actualizar();	//Modelo
            DibujarTodo();              	//Vista

            SDL_FlushEvent(SDL_KEYDOWN);

            timerFps = SDL_GetTicks() - timerFps;
            if(timerFps < 1000/FRAMERATE){
                SDL_Delay((1000/FRAMERATE) - timerFps);
            }
        }

    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void reiniciarJuego(){
        logger->log_debug("Tengo que cambiar las Parseriguraciones");
        terminar_juego();
        cargar_configuracion(this->parser);
        this->personajeJuego = new Personaje(new CoordenadaLogica(1,1),"Subzero",renderer, parser);
        cargar_capas();
        ventana->cambiarTamano(parser->ventana_anchopx, parser->ventana_altopx);
		barraDeVida1.Inicializar(0, parser->ventana_anchopx/2, parser->ventana_altopx, renderer, true);
        barraDeVida2.Inicializar(parser->ventana_anchopx/2, parser->ventana_anchopx, parser->ventana_altopx, renderer, false);
    };
//----------------------------------------------------------------
//----------------------------------------------------------------
    void terminar_juego(){
        //~ delete this->escenario;
        SDL_JoystickClose(Player1);
        SDL_DestroyTexture(under);
        delete this->director; // o sólo cambiar cosas?
        //~ delete this->personajeJuego;
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
                this->personajeJuego->Dibujarse(
                    (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    (posicionPJ_Piso+(parser->personaje_ancho))*(parser->ventana_altopx/parser->escenario_alto), 
                    (parser->ventana_altopx/parser->escenario_alto)*parser->personaje_alto,
                    (parser->ventana_anchopx/parser->ventana_ancho)*parser->personaje_ancho);
            }
        }

        if (escenario->capas.size()==0 || parser->personaje_zindex >= (escenario->capas.size())){
            // Si no hay capas;
            this->personajeJuego->Dibujarse(
                    (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho,
                    (posicionPJ_Piso+(parser->personaje_ancho))*(parser->ventana_altopx/parser->escenario_alto), 
                    (parser->ventana_altopx/parser->escenario_alto)*parser->personaje_alto,
                    (parser->ventana_anchopx/parser->ventana_ancho)*parser->personaje_ancho);
        }
        //cout <<"x_logico personaeje " << x_logico_personaje << "\n"; ///
        //cout <<"borde izquierdo pantalla " << borde_izquierdo_logico_pantalla << "\n"; ///


       /* printf("X logico personaje %f \n", x_logico_personaje);
        printf("Donde dibujo el personaje %f \n", (x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho);
*/

        barraDeVida1.Dibujarse();
        barraDeVida2.Dibujarse();

        // CoordenadaFisica* c = conv->aFisica(new CoordenadaLogica(parser->personaje_ancho, parser->personaje_alto));
        if (pausa){
            SDL_Rect pantalla = {0,0,parser->ventana_anchopx,parser->ventana_altopx};
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 150 );
            SDL_RenderFillRect( renderer, &pantalla );
        }
        SDL_RenderPresent(renderer);
};

//~ enum Estados{
      //~ Quieto_State,
      //~ SaltoDiagonal_State,
      //~ SaltoVertical_State,
      //~ Caminando_State
   //~ } estadoPersonaje1;


    void Controlador(SDL_Event *evento){
        SDL_PollEvent( evento );
        
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
		if (nada) seMuevePersonaje(jugador1, Nada);
    
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
                    if (pausa) this->director->pausar();
                    else this->director->despausar();
                }
                if(evento->key.keysym.sym == SDLK_a && !pausa)  {
                    barraDeVida1.Aliviar(20);
                    barraDeVida2.Aliviar(20);
                }
                if(evento->key.keysym.sym == SDLK_c && !pausa)  {
                    if (cansandoPJ == false){
                        barraDeVida1.Cansar(50);
                        barraDeVida2.Cansar(50);
                        cansandoPJ = true;
                    }
                }
                if((evento->key.keysym.sym == SDLK_d && !pausa))  {
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
    };


    //~ void ActualizarModelo(){
      //~ /*Quieto_State,      SaltoDiagonal_State,      SaltoVertical_State,
      //~ Caminando_State */
    //~ //Arriba_PRESIONADO, Izq_PRESIONADO, Der_PRESIONADO, erre_PRESIONADO;
    //~ if(saltando || saltoDiagonalIZQ || saltoDiagonalDER){
            //~ if(posicionPJ_Piso > parser->escenario_ypiso){
                //~ posicionPJ_Piso = parser->escenario_ypiso;
                //~ saltando = saltoDiagonalIZQ = saltoDiagonalDER = false;
                //~ //Despues de caer vuelve a quieto.
                //~ estadoPersonaje1 = Quieto_State;
                //~ this->personajeJuego->definir_imagen(this->mileTmp,QUIETO);
                //~ t = 1.0;
            //~ }else{
                //~ //Vo = 10px/t ; g =  6px/t*t
                //~ t+=0.05;
                //~ posicionPJ_Piso -= MOVER_PIXELES*10*t; //Vo *t
                //~ posicionPJ_Piso += MOVER_PIXELES*6*t*t; // -g *t * t
                //~ //this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
                //~ if(saltoDiagonalIZQ){
                    //~ if (x_logico_personaje - MOVER_PIXELES >= 0) x_logico_personaje -= 2*MOVER_PIXELES;
                    //~ 
                    //~ if ((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho < ANCHO_FISICO*(100-parser->margen)/200)
                    //~ borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - 2*MOVER_PIXELES;
//~ 
                 //~ }else if(saltoDiagonalDER){
                    //~ if (x_logico_personaje <= conf->escenario_ancho - conf->personaje_ancho) x_logico_personaje += 2*MOVER_PIXELES;
                    //~ 
                    //~ if ((borde_izquierdo_logico_pantalla + MOVER_PIXELES + parser->ventana_ancho < parser->escenario_ancho)
                    //~ &&((x_logico_personaje + (parser->personaje_ancho) - borde_izquierdo_logico_pantalla)> (parser->ventana_anchopx- parser->ventana_anchopx*(100-parser->margen)/200)))
                        //~ borde_izquierdo_logico_pantalla += 2*MOVER_PIXELES;
                //~ }
//~ 
            //~ }
        //~ }
//~ 
        //~ switch(estadoPersonaje1){
    //~ //QUIETO
            //~ case Quieto_State:
                //~ //Quieto --> salto Diag Der
                //~ if(Arriba_PRESIONADO && Der_PRESIONADO){
                    //~ estadoPersonaje1 = SaltoDiagonal_State;
                    //~ saltoDiagonalDER = true;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
//~ 
                    //~ scrollearDerecha = true;
                    //~ scrollearIzquierda = false;
                    //~ break;
                //~ }
                //~ //Quieto -->salto diag izq
                //~ if(Arriba_PRESIONADO && Izq_PRESIONADO){
                    //~ estadoPersonaje1 = SaltoDiagonal_State;
                    //~ saltoDiagonalIZQ = true;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
                    //~ scrollearIzquierda = true;
                    //~ scrollearDerecha = false;
                    //~ break;
                //~ }
                //~ //Quieto --> camina izq.
                //~ if(Izq_PRESIONADO){
                    //~ estadoPersonaje1 = Caminando_State;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,CAMINAR_IZQUIERDA);
//~ 
                    //~ scrollearIzquierda = true;
                    //~ scrollearDerecha = false;
                    //~ break;
                //~ }
                //~ // Quieto --> camina der
                //~ if (Der_PRESIONADO){
                    //~ estadoPersonaje1 = Caminando_State;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,CAMINAR_DERECHA);
                    //~ scrollearDerecha = true;
                    //~ scrollearIzquierda = false;
                    //~ break;
                //~ }
                //~ // Quieto --> salta vertical
                //~ if (Arriba_PRESIONADO){
                    //~ estadoPersonaje1 = SaltoVertical_State;
                    //~ saltando = true;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,SALTAR);
//~ 
                    //~ scrollearDerecha = false;
                    //~ scrollearIzquierda = false;
                    //~ break;
                //~ }
                //~ // Quieto -->Quieto
                //~ this->personajeJuego->definir_imagen(this->mileTmp,QUIETO);
                //~ break;
    //~ //CAMINANDO
            //~ case Caminando_State:
                //~ //Camino --> Salto diagonal der
                //~ if (Der_PRESIONADO && Arriba_PRESIONADO){
                    //~ estadoPersonaje1 = SaltoDiagonal_State;
                    //~ saltoDiagonalDER = true;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
                //~ //Camino --> Salto diagonal izq
                //~ }else if(Izq_PRESIONADO && Arriba_PRESIONADO){
                    //~ estadoPersonaje1 = SaltoDiagonal_State;
                    //~ saltoDiagonalIZQ = true;
//~ 
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
                //~ //Camino --> sigo caminando
                //~ }else if (Der_PRESIONADO){
                    //~ estadoPersonaje1 = Caminando_State;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,CAMINAR_DERECHA);
                    //~ scrollearDerecha = true;
                    //~ scrollearIzquierda = false;
                //~ //Camino --> sigo caminando
                //~ }else if (Izq_PRESIONADO){
                    //~ estadoPersonaje1 = Caminando_State;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,CAMINAR_IZQUIERDA);
                    //~ scrollearIzquierda = true;
                    //~ scrollearDerecha = false;
                //~ //Camino y no hay accion--> quieto
                //~ }else{
                    //~ estadoPersonaje1 = Quieto_State;
                    //~ this->personajeJuego->definir_imagen(this->mileTmp,QUIETO);
                //~ }
                //~ break;
    //~ //SALTANDO_VERTICAL
            //~ case SaltoVertical_State:
                //~ estadoPersonaje1 = SaltoVertical_State;
                //~ //this->personajeJuego->definir_imagen(this->mileTmp,SALTAR);
                //~ scrollearDerecha = false;
                //~ scrollearIzquierda = false;
                //~ break;
    //~ //SALTANDO_DIAGONAL
            //~ case SaltoDiagonal_State:
                //~ estadoPersonaje1 = SaltoDiagonal_State;
                //~ //this->personajeJuego->definir_imagen(this->mileTmp,SALTODIAGONAL);
                //~ 
                //~ break;
            //~ default:
                //~ this->personajeJuego->definir_imagen(this->mileTmp,QUIETO);
        //~ }
        //~ if (scrollearIzquierda){
                //~ if (x_logico_personaje >= 0 && ((x_logico_personaje - MOVER_PIXELES)>=0)) x_logico_personaje -= MOVER_PIXELES;
                //~ if ((x_logico_personaje - borde_izquierdo_logico_pantalla)*conv->factor_ancho < ANCHO_FISICO*(100-parser->margen)/200)
                //~ {
                    //~ //x_logico_personaje = x_logico_personaje + MOVER_PIXELES;
                    //~ borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                    //~ if (borde_izquierdo_logico_pantalla<0 && (borde_izquierdo_logico_pantalla - MOVER_PIXELES)<0){
                        //~ borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;
                        //~ this->personajeJuego->definir_imagen(this->mileTmp, QUIETO);
                    //~ }
                //~ }
                //~ // mover+= 5;
//~ 
//~ 
            //~ } else if (scrollearDerecha){
                 //~ if (x_logico_personaje <= parser->escenario_ancho - parser->personaje_ancho) x_logico_personaje += MOVER_PIXELES;
                 //~ if ((x_logico_personaje + (parser->personaje_ancho) - borde_izquierdo_logico_pantalla)> (parser->ventana_anchopx -parser->ventana_anchopx*(100-parser->margen)/200))
                //~ {
                    //~ //x_logico_personaje = x_logico_personaje - MOVER_PIXELES;
                    //~ borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla + MOVER_PIXELES;
//~ 
                    //~ if (borde_izquierdo_logico_pantalla + (parser->ventana_ancho) >= parser->escenario_ancho){
                        //~ borde_izquierdo_logico_pantalla = borde_izquierdo_logico_pantalla - MOVER_PIXELES;
                        //~ this->personajeJuego->definir_imagen(this->mileTmp,QUIETO);
                    //~ }
                //~ }
            //~ }
    //~ };
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
