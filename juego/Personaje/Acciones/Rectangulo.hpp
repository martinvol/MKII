#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Rectangulo{

	public:

		Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo);
		float empieza_alto, empieza_ancho;
		float porcentaje_alto, porcentaje_ancho;
		bool ataque, bloqueo;
		SDL_Rect *sdl_rec = NULL;
		void generar_rectanguloSDL(float x_fisico, float y_fisico, float ancho_fisico, float alto_fisico, SDL_Renderer* renderer, bool espejado);
		~Rectangulo();
};