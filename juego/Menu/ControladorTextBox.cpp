#include <locale>
#include "ControladorTextBox.hpp"

ControladorTextBox::ControladorTextBox(TextBox* textbox, Uint32 idVentana){
	this->textbox = textbox;
	resetear();
	this->idVentana = idVentana;
	SDL_StartTextInput();
}

void ControladorTextBox::resetear(){
	this->en_textbox = false;
}

ControladorTextBox::~ControladorTextBox(){
	SDL_StopTextInput();
	// No debe destruir el textbox
}

void ControladorTextBox::procesarEvento(SDL_Event* evento){
	//~ SDL_EnableUNICODE(SDL_ENABLE);
	switch (evento->type){
		case SDL_KEYDOWN:
			//Borrar
			if( evento->key.keysym.sym == SDLK_BACKSPACE)
			{
				if (en_textbox) textbox->sacarCaracter();
			}
			//Copiar
			else if( evento->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
			{
				if (en_textbox) SDL_SetClipboardText(textbox->obtenerTexto().c_str());
			}
			//Pegar
			else if( evento->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
			{
				if (en_textbox) textbox->agregarTexto(SDL_GetClipboardText());
			}
			break;
		case SDL_TEXTINPUT:
			//No copiar y pegar
			if( !(
					( ( evento->text.text[ 0 ] == 'c' || evento->text.text[ 0 ] == 'C' ) || 
					( evento->text.text[ 0 ] == 'v' || evento->text.text[ 0 ] == 'V' ) ) && 
					SDL_GetModState() & KMOD_CTRL )
			) {
				if (en_textbox) textbox->agregarTexto(evento->text.text);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
				if (evento->button.windowID == this->idVentana){
					if (
						(evento->button.x >= textbox->x && evento->button.x <= (textbox->x + textbox->ancho))
						&&
						(evento->button.y >= textbox->y && evento->button.y <= (textbox->y + textbox->alto))
					){
						en_textbox = true;
						textbox->fueSeleccionado();
					} else {
						en_textbox = false;
						textbox->fueDeseleccionado();
					}
				}
				break;
		default:
			;
	}
}
