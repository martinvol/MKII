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
		int x, y, ancho, alto, cant_max_chars;
		SDL_Renderer* renderer;
		SDL_Texture* textura;
		SDL_Surface* superficie;
	
		void borrarTexturas();
	
	public:
		TextBox(int x, int y, int ancho, int alto, const string& path, SDL_Renderer* renderer);
		~TextBox();
		
		void agregarCaracter(char c);
		void agregarTexto(string text);
		void sacarCaracter();
		string obtenerTexto();
		size_t obtenerLargoTexto();
		
		void Dibujarse();

};

#endif //TEXTBOX_H_INCLUDED
