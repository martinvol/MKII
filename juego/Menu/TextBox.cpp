#include "TextBox.hpp"

#include <SDL2/SDL_image.h>

#define FONT_SIZE 20
#define PATH_FONDO_TEXTBOX "resources/menu/blanco.png"
#define PATH_SELECCIONADO "resources/menu/opcion_transparent.png"
#define EXTRA 3

TextBox::TextBox(int x, int y, int ancho, int alto, const string& font_path, SDL_Renderer* renderer){
	this->x = x;
	this->y = y;
	this->ancho = ancho;
	this->alto = alto;
	
	TTF_Init();
	texto = "";
	texto_mostrado = "";
	font = TTF_OpenFont(font_path.c_str(), FONT_SIZE);
	
	int w, h;
	TTF_SizeText(font,"a",&w,&h);
	cant_max_chars = ancho / w;
	
	this->renderer = renderer;
	this->fondo = IMG_LoadTexture(renderer, PATH_FONDO_TEXTBOX);
	this->seleccion = IMG_LoadTexture(renderer, PATH_SELECCIONADO);
	this->seleccionado = false;
	this->textura = NULL;
	this->superficie = NULL;
}

TextBox::~TextBox(){
	if (textura != NULL) borrarTexturas();
	SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(seleccion);
	TTF_CloseFont(font);
}

void TextBox::fueSeleccionado(){
	this->seleccionado = true;
}

void TextBox::fueDeseleccionado(){
	this->seleccionado = false;
}

void TextBox::agregarCaracter(char c){
	texto.push_back(c);
	texto_mostrado.push_back(c);
	if (texto_mostrado.length() > cant_max_chars){
		//Borro el primer caracter antes mostrado.
		texto_mostrado.erase(0,1);
	}
}

void TextBox::agregarTexto(string text){
	texto += text;
	texto_mostrado += text;
	if (texto_mostrado.length() > cant_max_chars){
		texto_mostrado = texto_mostrado.substr(texto_mostrado.length() - cant_max_chars);
	}
}

void TextBox::sacarCaracter(){
	if (texto.length() > 0){
		texto.pop_back();
		sacarCaracterMostrado();
	}
}

void TextBox::sacarCaracterMostrado(){
	texto_mostrado.pop_back();
	if (texto.length() > texto_mostrado.length()) texto_mostrado = texto.substr(texto.length() - texto_mostrado.length() - 1);
}

string TextBox::obtenerTexto(){
	return texto;
}

size_t TextBox::obtenerLargoTexto(){
	return texto.length();
}

void TextBox::borrarTexturas(){
	SDL_FreeSurface(superficie);
	SDL_DestroyTexture(textura);
}

void TextBox::Dibujarse(){
	if (textura != NULL) borrarTexturas();
	// En NEGRO
	SDL_Color color = { 0, 0, 0, 0xFF };
	if( texto_mostrado.length() > 0 ) {
		superficie = TTF_RenderText_Solid(font, texto_mostrado.c_str(), color);
	} else {
		superficie = TTF_RenderText_Solid(font, " ", color);
	}
	textura = SDL_CreateTextureFromSurface(renderer, superficie);
	SDL_Rect destino;
	destino.x = this->x;
	destino.y = this->y;
	destino.w = this->ancho;
	destino.h = this->alto;
	SDL_RenderCopy(renderer, fondo, NULL, &destino);
	destino.x = this->x;
	destino.y = this->y;
	destino.h = this->alto;
	int ancho_texto;
	SDL_QueryTexture(textura, NULL, NULL, &ancho_texto, NULL);
	destino.w = ancho_texto;
	SDL_RenderCopy(renderer, textura, NULL, &destino);
	destino.x = this->x - EXTRA;
	destino.y = this->y - EXTRA;
	destino.w = this->ancho + 2*EXTRA;
	destino.h = this->alto + 2*EXTRA;
	if (seleccionado) SDL_RenderCopy(renderer, seleccion, NULL, &destino);
}
