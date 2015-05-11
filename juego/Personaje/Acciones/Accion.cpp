#include <dirent.h>

#include "Accion.hpp"

using namespace std;

/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/   
 
int Accion::cuentaArchivos(string ruta){
	
	DIR *dir;
	struct dirent *ent;
	
	dir = opendir (ruta.c_str());
	Logger* logger = Logger::instance();
	
	if (dir == NULL){
		string ruta = "resources/Default/"+to_string(this->accionNro);
		logger->log_error("No se puede abrir el directorio del Personaje");
		dir = opendir (ruta.c_str());
		this->ruta = ruta;
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
	this->cantModos = this->cuentaArchivos(this->ruta);
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
	bool ninguna = true;
	int numero;
	
	for (int i = 0; i<this->cantModos; i++){
		LTexture* imagen = new LTexture(this->win, this->renderer);
		numero = i+1;
		numeroImagen = to_string(numero);
		rutaCompleta = this->ruta+"/"+numeroImagen+".png";
		imagen->loadFromFile(rutaCompleta, this->hue_init, this->hue_fin, this->hue_offset);
		// Aca falta cambiar la forma de chequeo: la misma LTexture
		// podria tener un logger para solucionar esto *Manuel*
		if(imagen == NULL){
			this->logger->log_debug("IMG_LoadTexture error: " + (string)(SDL_GetError()));
			//cout<<"error en: "<<numeroImagen<<endl;
		}
		else{ninguna = false;}
		this->imagenes.push_back(imagen->mTexture);
		delete imagen;
	
	}
	if (ninguna){
		setRutaArchivo("resources/Default/"+to_string(this->accionNro));
		logger->log_debug("La carpeta ubicada en: "+ruta+" no contiene imagenes. \n \t\t\t Se paso por default: resources/Default/");
		setCantModos();
		setImagenes();
	}
}

void Accion::setInvertirSecuencia(){
	this->secuenciaInversa = true;
	if(this->cantModos>0)
		this->setModoActual(this->cantModos-1);
};

void Accion::setDireccionDerecha(){
	this->direccionDerecha = true;
}

void Accion::setDireccionIzquierda(){
	this->direccionDerecha = false;
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
 * 						CONSTRUCTOR Y DESTRUCTOR
 * 
 **********************************************************************/  

/**Constructor de la clase Accion
 * Recibe por parametro el numero identificador de la accion
 * un booleano que indica si la accion actual puede ser interrumpida.
 * y un puntero al Renderer.
 * */
Accion::Accion(int nroAccion, string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset){
	
	this->hue_init = hue_init;
	this->hue_fin = hue_fin;
	this->hue_offset = hue_offset;
	this->win = win;
	this->logger =  Logger::instance();
	this->secuenciaInversa = false;
	setAccionNro(nroAccion);
	setRutaArchivo(ruta+to_string(nroAccion));
	setRenderer(ren);
	setCantModos();
	if (this->cantModos == 0){
		//Se cargan imagenes desde una carpeta por defecto.
		setRutaArchivo("resources/Default/"+to_string(nroAccion));
		logger->log_debug("La carpeta ubicada en: "+ruta+" no contiene imagenes. \n \t\t\t Se paso por default: resources/Default/");
		setCantModos();
	}
	setImagenes();
	setModoActual(0);
	this->despl_x = despl_x;
	this->despl_y = despl_y;
	this->h_max = h_max;

	this->direccionDerecha = true;
	
	this->rectangulos = new vector<Rectangulo*>();
	//cout <<" "<< this->rectangulos->size() << " " << nroAccion << '\n';
}

/**Se destruye el vector, liberando la memoria 
 * ocupada por las imagenes guardadas en el vector.
 * */
Accion::~Accion(){	
	for (int i = 0; i < this->cantModos; i++){
		
		SDL_DestroyTexture(imagenes[i]);
	}
}

/***********************************************************************
 * 
 * 						DEMAS
 * 
 **********************************************************************/  

void Accion::resetear(){
	this->modoActual = 0;
	this->secuenciaInversa = false;
}

/**Devuelve true si se alcanzo el ultimo 
 * modo posible
 * false, en caso contrario.
 * */
bool Accion::esUltimoModo(){
	if(this->cantModos == 1){
		return true;
	}
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
	if (esUltimoModo())
		setModoActual(0);	
	
	else
		setModoActual(this->modoActual+1);
}
