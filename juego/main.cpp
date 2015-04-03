#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "BarrasDeVida.h"
#include "Capa.h"
#include "Escenario.h"
#include "parser.h"

#include "logger.h"

#include "ConversorDeCoordenadas.h"

using namespace std;


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

    Conf *conf;

    Juego(int argc_, char* argv_[]){
        argc = argc_;
        argv = argv_;
        this->escenario = new Escenario();        
    };
    int jugar(){

        if (InicializarSDL() != 0) return 1;
        renderer = SDL_CreateRenderer(NULL, -1, 0);
        configurar();
//Dibujarse(int x, int y, int alto, int ancho){


        // (escenario->capas[4])->Dibujarse(15+moverSZ ,ALTO_FISICO-170); // ESTA LINEA NO LA PUESO MOVER A LOOP!!!

    game_loop();

       /* fondo->Dibujarse(0 ,0 ,ALTO_FISICO,ANCHO_FISICO);
        columnasMuyLejos->Dibujarse(0.5*mover ,0);
        ColumnasLejos->Dibujarse(mover,0);
        piso->Dibujarse(0,ALTO_FISICO-46);
        Sz->Dibujarse(15+moverSZ ,ALTO_FISICO-170);*/

    // LIBERAR RECURSOS
    terminar_juego();

    terminar_sdl();
    return 0;

    };

    void cargar_configuracion(){
        this->conf = new Conf();
        conf->set_values(argv[1]);
        // Se settean configuraciones (con el json)
        // Esto tiene que cambiarse cuando se aprieta la letra R


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

        //SDL_Window* ventana = NULL;

        window = SDL_CreateWindow("Mortal Kombat 3 Ultimate", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        cargar_capas();
	
        //Izquierda
        barraDeVida1.Inicializar(0, ANCHO_FISICO/2, ALTO_FISICO, renderer, true);
       //Derecha
        barraDeVida2.Inicializar(ANCHO_FISICO/2, ANCHO_FISICO, ALTO_FISICO, renderer, false);

    }
    void cargar_capas(){
        for (unsigned int i = 0; i < conf->capas_vector.size(); i++){
            conf->capas_vector[i]->ren = renderer;
            

            // escenario->AgregarCapa(conf->capas_vector[i]); Por algún motivo esto no anda

            escenario->AgregarCapa( // esto no debería estar así, tendria que andar la línea de arriba, pero estuve luchando y no la hago andar (maxi)
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
	mover = 5;
        moverSZ= 1;
        SDL_Event evento;
        while (!salir){
            SDL_PollEvent( &evento );
            switch(evento.type){
                case SDL_QUIT:
                    salir = true;
                    break;
                case SDL_KEYDOWN:
                    if (evento.key.keysym.sym == SDLK_RIGHT)  {
                        scrollearDerecha = true;
                    	scrollearIzquierda = false;
                    }
                    if ((evento.key.keysym.sym == SDLK_LEFT) && (mover <0) )  {
                        scrollearIzquierda = true;
                    	scrollearDerecha = false;
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
                    if (evento.key.keysym.sym == SDLK_ESCAPE)  salir = true;
                    if (evento.key.keysym.sym == SDLK_r){
                        reiniciarJuego();
                    }
                    break;
		case SDL_KEYUP:
	                if((evento.key.keysym.sym == SDLK_d))  {
	                    golpeandoPJ = false;
	                }
			if((evento.key.keysym.sym == SDLK_c))  {
	                    cansandoPJ = false;
	                }
                break;
           }

	   if ( scrollearIzquierda && mover <0)
           	mover+= 5;

           if (scrollearDerecha && abs(mover)<500)
           	mover-= 5;

            //Limpio y dibujo
            SDL_RenderClear(renderer);


            //fondo
            (escenario->capas[0])->Dibujarse(0,0, ALTO_FISICO,ANCHO_FISICO);
            //CML
            (escenario->capas[1])->Dibujarse2(-mover ,0);
            //CL
            (escenario->capas[2])->Dibujarse2(-mover,0);
            //piso
            (escenario->capas[3])->Dibujarse2(-mover,0);
            //Sz
            //(escenario->capas[4])->Dibujarse2(15+moverSZ ,ALTO_FISICO-170); 
	
	    barraDeVida1.Dibujarse();
	    barraDeVida2.Dibujarse();
            
            SDL_RenderPresent(renderer);
        }
    };
    void reiniciarJuego(){
        puts("Tengo que cambiar las configuraciones");
        terminar_juego();
        cargar_configuracion();
        cargar_capas();
        SDL_SetWindowSize(window, ANCHO_FISICO, ALTO_FISICO); // Dani se encarga de poner esto en su objeto

    };
    void terminar_juego(){
        escenario->Borrar();
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
    /*Hola hola;
    return 0;*/

}
