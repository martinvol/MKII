#ifndef FUNCIONESDIBUJAR_H_INCLUDED
#define FUNCIONESDIBUJAR_H_INCLUDED
using namespace std;

// os puede ser la consola o un archivo (lo piden).
void ImprimirError(ostream &os, const string &msg);

SDL_Texture* CargarTextura(const string &file, SDL_Renderer *ren);

//Usa coordenadas fisicas
void DibujarTextura(SDL_Texture *tex, SDL_Renderer *ren, int x_fisico, int y_fisico, int w, int h);

void DibujarTextura(SDL_Texture *tex, SDL_Renderer *ren, int x_fisico, int y_fisico);

int InicializarSDL();

#endif // FUNCIONESDIBUJAR_H_INCLUDED
