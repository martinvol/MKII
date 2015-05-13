#include <sstream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "../Escenario/Capa.hpp"
#include "../Logger/Logger.hpp"
#include "Parser.hpp"
#include "jsoncpp/json/json.h"

#define IMAGEN_DEFAULT "resources/background/defaultcapa.png"
#define DEFAULT_X_LOGICO 0

using namespace std;

/*********************************************************************
 * 
 * 					CONSTRUCTOR Y DESTRUCTOR
 * 
 *********************************************************************/
 
 // El constructor no lo hacemos porque se supone que siempre después de
 // crearlo se llama a set_values inmediatamente, antes de llamar a
 // cualquier otra función.

Parser::~Parser(){
	for (unsigned int i = 0; i < this->capas_vector.size(); i++)
		delete this->capas_vector[i];
	capas_vector.clear();
    
    for (unsigned int i = 0; i < this->joysticks->size(); i++){
        delete joysticks->at(i);
    }
    delete joysticks;
}



/*********************************************************************
 * 
 * 								DEMAS
 * 
 *********************************************************************/

bool exists_test(const std::string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

void Parser::set_values (char* my_file) {
    logger = Logger::instance();
    logger->log_debug("Inicializando parser");

    logger->log_debug(std::string("Intenta cargar configuraciones desde ") +  my_file);

    Json::Value root;
    Json::Reader reader;

    if (my_file == NULL){
            logger->log_error("Problema con el archivo, probablemente no existe");
            cargarDefault();
    }
    
    std::ifstream test(my_file, std::ifstream::binary);

    if (!test.is_open()){
        logger->log_error("Problema con el archivo, probablemente no existe");
        cargarDefault();

    }

    else {

        bool parsingSuccessful = reader.parse(test, root, false);

        if (parsingSuccessful){

            const Json::Value escenario = root["escenario"];

            escenario_ancho = cargarValidar(escenario, 1500, "ancho", "Usando ancho default de 1500");

            if (escenario_ancho > 2000 || escenario_ancho < 1000){
                logger->log_error("El ancho del escenario es inválido, se usará el ancho por defautl, 1500");
                escenario_ancho = 1500;
            }

            escenario_alto = cargarValidar(escenario, 200, "alto", "Usando ancho default de 200");

            if (escenario_alto > (3./4.)*escenario_ancho || escenario_ancho < (1/4)){
                logger->log_error("El alto del escenario es inválido, se usará el ancho por default, la mitad del ancho");
                escenario_alto = escenario_ancho/2.;
            }

            escenario_ypiso = cargarValidar(escenario, 0, "ypiso", "Usando ypiso default de 0");


            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = cargarValidar(ventana, 800, "anchopx", "Usando anchopx default de 800px");
            if (ventana_anchopx > 1200 || ventana_anchopx<400){
                logger->log_error("El ancho de la ventana es inválido, se usará el ancho por defautl, 800px.");
                ventana_anchopx = 800;
            }

            ventana_altopx = cargarValidar(ventana, 416, "altopx", "Usando altopx default de 416px");
            if (ventana_altopx > 1000 || ventana_anchopx< 200){
                logger->log_error("El alto de la ventana es inválido, se usará el ancho por defautl, 600px.");
                ventana_altopx = 600;
            }


            ventana_ancho = cargarValidar(ventana, 600, "ancho", "Usando ancho default de 600px");
            if (ventana_ancho > 700 || ventana_ancho < 200){
                logger->log_error("El alto de la ventana es inválido, se usará el ancho por defautl, 600px.");
                ventana_ancho = 600;
            }
            
            margen = cargarValidar(ventana, 70, "margen", "Usando marge default de 70%");
            if (margen > 100){
                logger->log_error("El margen no puede ser mayor que 100, se usará el ancho por defautl, 70%.");
                margen = 70;
            }

            const Json::Value personaje = root["personaje"];

            this->velocidad_arma = cargarValidar(personaje, 10, "velocidad_arma", "La velocidad (personaje) default de 10");

            personaje_ancho = cargarValidar(personaje, 100, "ancho", "Usando ancho (personaje) default de 100");
            if (personaje_ancho >(3./4)*ventana_ancho || personaje_ancho < 0){
                logger->log_error("El ancho del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_ancho = ventana_ancho/4.;
            }

            personaje_alto = cargarValidar(personaje, 100, "alto", "Usando alto (personaje) default de 100");
            if (personaje_alto >(3./4)*escenario_alto || personaje_alto < (1./8)*escenario_alto){
                logger->log_error("El alto del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_alto = escenario_alto/4.;
            }


            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                try{
                    sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
                } catch(const runtime_error& error) {
                    logger->log_error("El path del personaje es un número, no un string");
                    sprites_map[id] = "resources/jugador/SubZero/";
                }
            }

            const Json::Value capas = root["capas"];
            int cantidad_de_capas = 0;

            for ( int index = 0; index < capas.size(); ++index ){
                
                string nombre_archivo;

                cantidad_de_capas++;

                if (!capas[index].isMember("imagen_fondo")){
                    logger->log_error("Esta capa no tiene el valor imagen_fondo, se cargará la capa por default");
                }
                
                try {
                    nombre_archivo = capas[index].get("imagen_fondo", IMAGEN_DEFAULT).asString(); // este default hay que ponerlo bien
                } catch(const runtime_error& error){
                    logger->log_error("Hay un número en el nombre de la capa");
                    nombre_archivo = "";

                }
                

                
                if (!exists_test(nombre_archivo)){
                    logger->log_error("La capa no fue encontrada, cargando capa por default");
                    nombre_archivo = IMAGEN_DEFAULT;
                }
                
                logger->log_debug("Intentando cargar capa '" + nombre_archivo + "'");

                float ancho_logico_capa = cargarValidar(capas[index], 700, "anchoLogico","Ancho lógico de capa no encontrado, se toma 700 por default");

                if (ancho_logico_capa < ventana_ancho){
                    logger->log_error("La capa no puede tener un ancho lógico menor menor al de la ventana, se usa como ancho de esta el ancho de la ventana");
                    ancho_logico_capa = ventana_ancho;

                }

                Capa *temp = new Capa(
                    nombre_archivo, ancho_logico_capa, DEFAULT_X_LOGICO, 
                    NULL,
                    escenario_ancho,
                    ventana_ancho
                );

                capas_vector.push_back(temp);
            }

            if (cantidad_de_capas == 0){
                logger->log_error("No se encontró ninguna capa");
                logger->log_warning("Se carga una capa por default, con el ancho de la ventana/2");
                Capa *temp = new Capa(
                    IMAGEN_DEFAULT, ventana_ancho/2, DEFAULT_X_LOGICO, 
                    NULL,
                    escenario_ancho,
                    ventana_ancho
                );
                capas_vector.push_back(temp);
            }

            personaje1_nombre = personaje.get("nombre", "Personaje 1").asString();
            personaje_zindex = cargarValidar(personaje, 0, "zindex", "Usando zindex (personaje) default de 0");

            if (personaje_zindex < 0){
                logger->log_error("Z-Index del personaje no puede ser negativo, usando Z maximo posible");
                personaje_zindex = capas.size();
            }


            if ((escenario_ypiso + personaje_alto > escenario_alto*0.8) || escenario_ypiso<0){
                logger->log_error("El piso es inválido, se pone 0 por default");
                escenario_ypiso = 0;
            }

            color_inicio = cargarValidar(personaje, 150, "inicio", "Usando inicio (color) default de 150");

            color_fin = cargarValidar(personaje, 200, "fin", "Usando inicio (color) default de 200");

            if (((int) color_fin)%360 < ((int) color_inicio)%360){
                logger->log_error("El fin no puede ser menor que el inicio (color), fin=inicio + 50");
                color_fin = color_inicio + 50;

            }

            color_offset = cargarValidar(personaje, 200, "offset", "Usando inicio (color) default de 200");


            Json::Value joystick;

            joysticks = new std::vector<unordered_map <string, int>*>();

            for (unsigned int i = 0; i<2; i++){

                char result[10] = "joystick"; 
                char buf[2];

                sprintf(buf, "%d", i+1);
                result[8] = buf[0];

                joystick = root[result];
                unordered_map <string, int>* joystick_map = new unordered_map<string, int>();
        
                (*joystick_map)["pinia_baja"] = (int) cargarValidar(joystick, 0, "pinia_baja", "Usando botón default 0");
                (*joystick_map)["pinia_alta"] = (int) cargarValidar(joystick, 3, "pinia_alta", "Usando botón default 3");
                
                (*joystick_map)["cubrirse"] = (int) cargarValidar(joystick, 1, "cubrirse", "Usando botón default 1");
    
                (*joystick_map)["patada_baja"] = (int) cargarValidar(joystick, 2, "patada_baja", "Usando botón default 2");
                (*joystick_map)["patada_alta"] = (int) cargarValidar(joystick, 5, "patada_alta", "Usando botón default 5");
                
                (*joystick_map)["arrojar_arma"] = (int) cargarValidar(joystick, 4, "arrojar_arma", "Usando botón default 2");
                (*joystick_map)["arrojar_arma_baja"] = (int) cargarValidar(joystick, 50, "arrojar_arma_baja", "Usando botón default 50");
                (*joystick_map)["arrojar_arma_alta"] = (int) cargarValidar(joystick, 50, "arrojar_arma_alta", "Usando botón default 50");

                joysticks->push_back(joystick_map);
            }

            const Json::Value personaje2 = root["personaje2"];

            personaje2_nombre = personaje2.get("nombre", "Personaje 2").asString();

            personaje2_ancho = cargarValidar(personaje2, 100, "ancho", "Usando ancho (personaje2) default de 100");
            if (personaje2_ancho >(3./4)*ventana_ancho || personaje2_ancho < 0){
                logger->log_error("El ancho del personaje2 es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje2_ancho = ventana_ancho/4.;
            }

            personaje2_alto = cargarValidar(personaje2, 100, "alto", "Usando alto (personaje2) default de 100");
            if (personaje2_alto >(3./4)*escenario_alto || personaje2_alto < (1./8)*escenario_alto){
                logger->log_error("El alto del personaje2 es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje2_alto = escenario_alto/4.;
            }


            const Json::Value sprites2 = personaje2["sprites"];

            for (auto const& id : sprites2.getMemberNames()) {
                try{
                    sprites_map[id] = sprites2.get(id, "Esto nunca se va a mostrar").asString();
                } catch(const runtime_error& error) {
                    logger->log_error("El path del personaje2 es un número, no un string");
                    sprites_map[id] = "resources/jugador/SubZero/";
                }
            }

            personaje2_zindex = cargarValidar(personaje2, 0, "zindex", "Usando zindex (personaje2) default de 0");

            if (personaje2_zindex < 0){
                logger->log_error("Z-Index del personaje2 no puede ser negativo, usando Z maximo posible");
                personaje2_zindex = capas.size();
            }


        } else {
            logger->log_error("Error de sytaxis en el archivo");
            logger->log_error(reader.getFormatedErrorMessages());
            cargarDefault();
        }
    }
}

float Parser::cargarValidar(Json::Value objetoJson, float valorDefault, char* clave, char* mensaje){
    float result = 0;
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    
    try {
        result = objetoJson.get(clave, valorDefault).asFloat();
    }
    catch (const runtime_error& error){
        std::ostringstream buff;
        buff << valorDefault;
        logger->log_error("Se esperaba un número en " + std::string(clave) + ", se usá el valor por default" + buff.str());
        result = valorDefault;
    }
    return result;
}

bool Parser::cargarValidarBool(Json::Value objetoJson, bool valorDefault, char* clave, char* mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asBool();
}


void Parser::cargarDefault(){
    logger = Logger::instance();
    logger->log_debug("Cargando configuración default");
    Parser::set_values("resources/default.json");
}
