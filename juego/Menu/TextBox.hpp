#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <string>
#include <SDL2/SDL_ttf.h>

using namespace std;

class TextBox {
	private:
		string texto;
		string texto_mostrado;
		TTF_Font* font;
		int cant_max_chars;
		SDL_Renderer* renderer;
		SDL_Texture* textura, *fondo, *seleccion;
		SDL_Surface* superficie;
		bool seleccionado;
		
		void borrarTexturas();
		void sacarCaracterMostrado();
	
	public:
		int x, y, ancho, alto;
	
		TextBox(int x, int y, int ancho, int alto, const string& path, SDL_Renderer* renderer);
		~TextBox();
		
		void agregarCaracter(char c);
		void agregarTexto(string text);
		void sacarCaracter();
		string obtenerTexto();
		size_t obtenerLargoTexto();
		
		void fueSeleccionado();
		void fueDeseleccionado();
		void Dibujarse();

};

#endif //TEXTBOX_H_INCLUDED
