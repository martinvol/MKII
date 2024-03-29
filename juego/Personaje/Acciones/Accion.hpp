#ifndef ACCION_H_INCLUDED
#define ACCION_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include "../../LTexture.hpp"

#include "../../Coordenadas/CoordenadaLogica.hpp"
#include "../../Logger/Logger.hpp"
#include "Rectangulo.hpp"

typedef enum accion_posible {QUIETO, CAMINAR_DERECHA, CAMINAR_IZQUIERDA, 	//Listo
							SALTAR, SALTARDIAGONAL_IZQ, SALTARDIAGONAL_DER, //Listo						
							AGACHARSE, PARARSE,	//Listo
							
							PINIAALTA, PINIABAJA, //Listo 
							MIRARDERECHA, MIRARIZQUIERDA,	//Falta MIRARIZQUIERDA
							// Y FALTARIA LA VERSION PARA AGACHADO (pero es secundario)
							PATADAALTA, PATADABAJA, PATADABAJAAGACHADO, PATADAALTAAGACHADO, // Listo
							
							ARROJARARMA, 	//Listo
							CUBRIRALTO, CUBRIRBAJO, // listo
							TRABA, 	//Listo																
							PINIAAGACHADO, GANCHO, PINIASALTANDOVERTICAL, PINIASALTANDODIAGONAL, // Ponerlos lindos
							PATADASALTANDOVERTICAL, PATADASALTANDODIAGONAL, // Ponerlos Lindos
							ROUNDKICK, // Arreglar el handling de eventos.
							TROPEZAR, // Al recibir una patada voladora. FALTA Y NO ESTA
							CAERPORGANCHO, LEVANTARSEDELGANCHO, //FALTA
							CAERPORTRABA, LEVANTARSEDELATRABA,
							RECIBIRGOLPEALTO, RECIBIRGOLPEBAJO, RECIBIRGOLPEAGACHADO,
							GANAR, MORIR, DIZZY, DESAPARECER,
							HACER_FATALITY } accion_posible; //FALTA
							/* PINIASALTANDOVERTICAL y PINIASALTANDODIAGONAL tienen los mismos sprites
							 * pero PINIASALTANDODIAGONAL conserva la 'inercia', o sea sigue 
							 * la parabola.
							 * 
							 * TROPEZAR lo hace caminar para atras
							 * 
							 * CAERPORGANCHO tambien lo hace volar 
							 * 
							 * */

using namespace std;

class Accion{
	
	private: 
		int cuentaArchivos(string ruta);
	public:
	
		bool dibuje_rectangulos=false;
		bool saque_vida = false;
		
		/* Constantes de desplazamiento. */
		float despl_x;
		float despl_y;
		float h_max;
		
		int ciclos;
		bool direccionDerecha;
		float intervalo = 0;
		float tinicial = 0;
		int accionNro;
		int cantModos;
		string ruta;
		SDL_Renderer* renderer;
		vector<SDL_Texture*> imagenes;
		vector<SDL_Texture*> imagenesEspejadas;
		int modoActual;
		
		Logger* logger;
		bool secuenciaInversa;

		Mix_Chunk* sonido = NULL;

		Accion(int nroAccion, string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset); //constructor

		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
		void setDireccionDerecha();
		void setDireccionIzquierda();
		void setInvertirSecuencia();
		void setDanio(int danio);
	
		SDL_Texture* getImagenActual(bool derecha);
		int hue_init, hue_fin, hue_offset;
		
		int getModoActual();
	
		bool esUltimoModo();
		void cambiarModo();
		
		/* Devuelve una NUEVA coordenada. */
		virtual CoordenadaLogica* execute(CoordenadaLogica* coord_personaje){return NULL;}
		
		/* Devuelve true si puede ser interrumpida por alguna OTRA acción.
		 * Es decir, se supone que no le va a llegar la misma acción que es. */
		virtual bool permiteAccion(accion_posible nuevaAccion){return true;}

		std::vector<Rectangulo*>* rectangulos = NULL;

		~Accion();
		virtual void resetear();
		void cambiarModoInversamente();
		float porcentajeDeDanio = 0;
		void gritar();
		void iniciarSonido();
};

#endif
