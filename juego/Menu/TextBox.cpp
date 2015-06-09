#include "TextBox.hpp"

#define FONT_SIZE 20


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
	this->textura = NULL;
	this->superficie = NULL;
}

TextBox::~TextBox(){
	borrarTexturas();
	TTF_CloseFont(font);
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
		texto_mostrado.pop_back();
	}
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
	SDL_Color color = { 0, 0, 0, 0xFF };
	if( texto_mostrado != "" ) {
		superficie = TTF_RenderText_Solid(font, texto_mostrado.c_str(), color);
	} else {
		superficie = TTF_RenderText_Solid(font, " ", color);
	}
	textura = SDL_CreateTextureFromSurface(renderer, superficie);
}
