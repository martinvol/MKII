#ifndef SALTARVERTICAL_H_INCLUDED
#define SALTARVERTICAL_H_INCLUDED

#include "Accion.hpp"

/* La idea para el salto es que tiene que llegar a H_MAX.
 * Por lo que, aunque el escenario no lo deje, la acción termina cuando
 * se "sube y baja" H_MAX. Por lo que contamos la "distancia virtual"
 * recorrida.
 * Sabiendo la cantidad de imágenes que tiene el sprite, se divide la 
 * misma cantidad de "distancia" a cada imagen, la primera mitad como
 * imágenes de subida y la otra mitad como de bajada; excepto que sean
 * impares, donde la imagen de la mitad se usa como última de subida y
 * última de bajada.
 * Entonces mientras se está en la distancia que le corresponde a cada
 * imagen, se muestra esa imagen. */


class SaltarVertical:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		
		float dist_virtual;
		float delta;
		bool impar;
		bool alcanzo_max;
		
		SaltarVertical(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(3,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
			this->alcanzo_max = false;
			this->contadorDeLoops=0;
			contador= 0;
			dist_virtual=0;
			if (cantModos%2==1){
				// cantidad impar de imágenes.
				delta = 2.0 * h_max / (cantModos+1);
				impar = true;
			} else {
				delta = 2.0 * h_max/cantModos;
				impar = false;
			}

			/// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
			rectangulos->push_back(new Rectangulo(10, 45, 10, 22, false, false)); // Cabeza
			rectangulos->push_back(new Rectangulo(20, 0, 80, 100, false, false)); // torso
			//rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
		}
		
		void resetear();
		void cambiarModo();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
