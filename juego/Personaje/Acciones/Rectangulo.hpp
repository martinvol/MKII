#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Rectangulo{

	public:

		Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque);
		
		float empieza_alto, empieza_ancho;
		float porcentaje_alto, porcentaje_ancho;
		bool ataque;
		SDL_Rect *sdl_rec = NULL;
		// acá puede ir una función que devuelva el rectangulo en función del tamaño de personaje

		void generar_rectanguloSDL(float x_fisico, float y_fisico, float ancho_fisico, float alto_fisico, SDL_Renderer* renderer);
		~Rectangulo();
};