#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Accion.hpp"
#include <vector>
#include <string>
#include <thread>         
#include <chrono> 
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/   
 
int cuentaArchivos(string ruta){
	
	/* Con un puntero a DIR abriremos el directorio */
	DIR *dir;
	/* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
	struct dirent *ent;
	
	dir = opendir (ruta.c_str());
	Logger* logger = Logger::instance();
	/* Miramos que no haya error */
	if (dir == NULL){
		logger->log_error("No se puede abrir el directorio del Personaje");
		exit(EXIT_FAILURE);
		return 0;
	}

	int i = 0;
	while ((ent = readdir (dir)) != NULL){
	  if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
			  i++; 
	  }
	}
	
	closedir (dir);

	return i;
}

/***********************************************************************
 * 
 * 						SETERS(?)
 *
 **********************************************************************/  

void Accion::setRutaArchivo(const string directorio){
	this->ruta = directorio;
}
void Accion::setCantModos(){
	this->cantModos = cuentaArchivos(this->ruta);
}
void Accion::setModoActual(int modo){
	this->modoActual = modo;
}
void Accion::setRenderer(SDL_Renderer* ren){
	this->renderer = ren;
}
void Accion::setAccionNro(int nroAccion){
	this->accionNro = nroAccion;
}
/**Guarda en un vector el conjunto de imagenes
 * correspondientes a la propia accion
 * */
void Accion::setImagenes (){
	
	string numeroImagen, rutaCompleta; 
	
	int numero;
	
	for (int i = 0; i<this->cantModos; i++){
		SDL_Texture* imagen;
		numero = i+1;
		numeroImagen = to_string(numero);
		rutaCompleta = this->ruta+"/"+numeroImagen+".png";
		imagen = IMG_LoadTexture (this->renderer,rutaCompleta.c_str());
		if(imagen == NULL){
			this->logger->log_debug("IMG_LoadTexture error: " + (string)(SDL_GetError()));
			//cout<<"error en: "<<numeroImagen<<endl;
		}
		this->imagenes.push_back(imagen);
	
	}
}


/***********************************************************************
 * 
 * 						GETTERS
 * 
 **********************************************************************/  
 
/**Devuelve un puntero de tipo SDL_Texture
 * que corresponde a la imagen equivalente al
 * modo actual en el que se encuentra la accion
 * */
SDL_Texture* Accion::getImagenActual(){
	return this->imagenes[this->modoActual];
} 
//~ SDL_Texture* Accion::getImagenNro(int numeroDeSprite){
	//~ return this->imagenes[numeroDeSprite];
//~ }
int Accion::getModoActual(){
	return this->modoActual;
}
/***********************************************************************
 * 
 * 						CONSTRUCTOR
 * 
 **********************************************************************/  

/**Constructor de la clase Accion
 * Recibe por parametro el numero identificador de la accion
 * un booleano que indica si la accion actual puede ser interrumpida.
 * y un puntero al Renderer.
 * */
Accion::Accion(int nroAccion, string ruta, SDL_Renderer* ren, Parser* parser){
	this->lastTime = 0;
	this->parser = parser;
	this->logger =  Logger::instance();
	//cout<<"CONSTRUCTOR ACCION NRO: "<<nroAccion<<endl;
	setAccionNro(nroAccion);
	setRutaArchivo(ruta+to_string(nroAccion));
	setRenderer(ren);
	setCantModos();
	setImagenes();
	setModoActual(0);	
	
}

/***********************************************************************
 * 
 * 						DEMAS
 * 
 **********************************************************************/  

/**Se destruye el vector, liberando la memoria 
 * ocupada por las imagenes guardadas en el vector.
 * */
Accion::~Accion(){
	
	for (int i = 0; i < this->cantModos; i++){
			SDL_DestroyTexture(imagenes[i]);
	}
}

/**Devuelve true si el entero pasado por parametro 
 * (que representa una accion determinada)
 * es la misma que el correspondiente a la accion actual.
 * false, en caso contrario
 * */
bool Accion::esDistintaA(int nroAccion){
	//cout<<"nroAccionACtual"<<this->accionNro<<"nueva"<<nroAccion<<endl;
	if (this->accionNro != nroAccion)
		return true;
	return false;
}

/**Devuelve true si se alcanzo el ultimo 
 * modo posible
 * false, en caso contrario.
 * */
bool Accion::esUltimoModo(){
	if (this->modoActual == (this->cantModos-1)){
		return true;
	}
	return false;
}
		
/**Aumenta en uno el modo Actual
 * si llega al ultimo modo posible
 * retorna al modo 0.
 * */ 	
void Accion::cambiarModo(){
	if (esUltimoModo()){
		setModoActual(0);	
	}
	else{
		setModoActual(this->modoActual+1);
	}
}

void Accion::execute(float tmp){}

