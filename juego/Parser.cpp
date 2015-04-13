#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "Capa.hpp"
#include "Parser.hpp"
#include "Logger.hpp"
#include "jsoncpp/json/json.h"

#define IMAGEN_DEFAULT "resources/background/defaultcapa.png"
#define DEFAULT_X_LOGICO 0

#define DEFAULT_ESCENARIO_ANCHO 1500
#define DEFAULT_ESCENARIO_ALTO 200
#define DEFAULT_ESCENARIO_YPISO 0
#define DEFAULT_VENTANA_ANCHO_FISICO 800
#define DEFAULT_VENTANA_ALTO_FISICO 416
#define DEFAULT_VENTANA_ANCHO_LOGICO 600
#define DEFAULT_MARGEN 70
#define DEFAULT_PERSONAJE_ANCHO 100
#define DEFAULT_PERSONAJE_ALTO 100
#define DEFAULT_PERSONAJE_ZINDEX 0
#define DEFAULT_CAPA_ANCHO 700




using namespace std;

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

    logger->log_debug(std::string("Intenta cargar Parseriguraciones desde ") +  my_file);

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

            escenario_ancho = cargarValidar(escenario, DEFAULT_ESCENARIO_ANCHO, "ancho", "Usando ancho default de " + to_string(DEFAULT_ESCENARIO_ANCHO));

            if (escenario_ancho > 2000 || escenario_ancho < 1000){
                logger->log_error("El ancho del escenario es inválido, se usará el ancho por defautl, " + to_string(DEFAULT_ESCENARIO_ANCHO));
                escenario_ancho = DEFAULT_ESCENARIO_ANCHO;
            }

            escenario_alto = cargarValidar(escenario, DEFAULT_ESCENARIO_ALTO, "alto", "Usando ancho default de " + to_string(DEFAULT_ESCENARIO_ALTO));

            if (escenario_alto > (3./4.)*escenario_ancho || escenario_ancho < (1/4)){
                logger->log_error("El alto del escenario es inválido, se usará el ancho por defaulT, la mitad del ancho");
                escenario_alto = escenario_ancho/2.;
            }

            escenario_ypiso = cargarValidar(escenario, DEFAULT_ESCENARIO_YPISO, "ypiso", "Usando ypiso default de " + to_string(DEFAULT_ESCENARIO_YPISO));


            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = cargarValidar(ventana, DEFAULT_VENTANA_ANCHO_FISICO, "anchopx", "Usando anchopx default de " + to_string(DEFAULT_VENTANA_ANCHO_FISICO) + "px");
            if (ventana_anchopx > 1200 || ventana_anchopx<400){
                logger->log_error("El ancho de la ventana es inválido, se usará el ancho por defautl, " + to_string(DEFAULT_VENTANA_ANCHO_FISICO) + "px.");
                ventana_anchopx = DEFAULT_VENTANA_ANCHO_FISICO;
            }

            ventana_altopx = cargarValidar(ventana, DEFAULT_VENTANA_ALTO_FISICO, "altopx", "Usando altopx default de " + to_string(DEFAULT_VENTANA_ALTO_FISICO) + "px");
            
            if (ventana_altopx > 1000 || ventana_anchopx< 200){
                logger->log_error("El alto de la ventana es inválido, se usará el alto por defautl, " + to_string(DEFAULT_VENTANA_ALTO_FISICO) + "px.");
                ventana_altopx = DEFAULT_VENTANA_ALTO_FISICO;
            }


            ventana_ancho = cargarValidar(ventana, DEFAULT_VENTANA_ANCHO_LOGICO, "ancho", "Usando ancho default de " + to_string(DEFAULT_VENTANA_ANCHO_LOGICO) + "px");
            
            if (ventana_altopx > 700 || ventana_anchopx < 200){
                logger->log_error("El alto de la ventana es inválido, se usará el ancho por defautl, " + to_string(DEFAULT_VENTANA_ANCHO_LOGICO) + "px.");
                ventana_ancho = DEFAULT_VENTANA_ANCHO_LOGICO;
            }
            
            margen = cargarValidar(ventana, DEFAULT_MARGEN, "margen", "Usando marge default de " + to_string(DEFAULT_MARGEN) + "%");
            if (margen > 100){
                logger->log_error("El margen no puede ser mayor que 100, se usará el ancho por defautl, " + to_string(DEFAULT_MARGEN) + "%.");
                margen = DEFAULT_MARGEN;
            }

            const Json::Value personaje = root["personaje"];

            personaje_ancho = cargarValidar(personaje, DEFAULT_PERSONAJE_ANCHO, "ancho", "Usando ancho (personaje) default de " + to_string(DEFAULT_PERSONAJE_ANCHO));
            if (personaje_ancho >(3./4)*ventana_ancho || personaje_ancho < 0){
                logger->log_error("El ancho del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_ancho = ventana_ancho/4.;
            }

            personaje_alto = cargarValidar(personaje, DEFAULT_PERSONAJE_ALTO, "alto", "Usando alto (personaje) default de " + to_string(DEFAULT_PERSONAJE_ALTO));
            if (personaje_alto >(3./4)*escenario_alto || personaje_alto < (1./8)*escenario_alto){
                logger->log_error("El alto del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_alto = escenario_alto/4.;
            }


            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
            }

            const Json::Value capas = root["capas"];
            
            for ( int index = 0; index < capas.size(); ++index ){
                
                string nombre_archivo;

                if (!capas[index].isMember("imagen_fondo")){
                    logger->log_error("Esta capa no tiene el valor imagen_fondo, se cargará la capa por default");
                }
                
                nombre_archivo = capas[index].get("imagen_fondo", IMAGEN_DEFAULT).asString(); // este default hay que ponerlo bien
                logger->log_debug("Intentando cargar capa '" + nombre_archivo + "'");
                
                if (!exists_test(nombre_archivo)){
                    logger->log_error("La capa no fue encontrada, cargando capa por default");
                    nombre_archivo = IMAGEN_DEFAULT;
                }

                Capa* temp = new Capa(
                    nombre_archivo,
                    cargarValidar(capas[index], DEFAULT_CAPA_ANCHO, "anchoLogico","Ancho lógico de capa no encontrado, se toma " + to_string(DEFAULT_CAPA_ANCHO) + " por default"),
                    //capas[index].get("anchoLogico", 0).asFloat(),
                    DEFAULT_X_LOGICO, 
                    NULL,
                    escenario_ancho, ventana_ancho);

                capas_vector.push_back(temp);
            }

            personaje_mirar_derecha = cargarValidarBool(personaje, true, "mirar_derecha", "El personaje no tiene mirar_derecha, se carga por default derecha");

            personaje_zindex = cargarValidar(personaje, DEFAULT_PERSONAJE_ZINDEX, "zindex", "Usando zindex (personaje) default de " + to_string(DEFAULT_PERSONAJE_ZINDEX));

            if (personaje_zindex < 0){
                logger->log_error("Z-Index del personaje no puede ser negativo, usando Z maximo posible");
                personaje_zindex = capas.size();
            }

        } else {
            logger->log_error("Error de sytaxis en el archivo");
            logger->log_error(reader.getFormatedErrorMessages());
            cargarDefault();
        }
    }
}

float Parser::cargarValidar(Json::Value objetoJson, float valorDefault, char* clave, std::string mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asFloat();
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
