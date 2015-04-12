#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "Capa.h"
#include "parser.h"
#include "logger.h"
#include "jsoncpp/json/json.h"

using namespace std;


void Conf::set_values (char* my_file) {
    logger = Logger::instance();
    logger->log_debug("Inicializando parser");

    logger->log_debug(std::string("Intenta cargar configuraciones desde ") +  my_file);

    Json::Value root;
    Json::Reader reader;

    if (my_file == NULL){
            cargarDefault();
    }
    
    std::ifstream test(my_file, std::ifstream::binary);

    if (!test.is_open()){
        cargarDefault();

    }

    else {

        bool parsingSuccessful = reader.parse(test, root, false);

        if (parsingSuccessful){

            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = cargarValidar(ventana, 800, "anchopx", "Usando anchopx default de 800px");
            ventana_altopx = cargarValidar(ventana, 416, "altopx", "Usando altopx default de 416px");

            ventana_ancho = cargarValidar(ventana, 600, "ancho", "Usando ancho default de 600px");
            margen = cargarValidar(ventana, 80, "margen", "Usando marge default de 80%");

            const Json::Value escenario = root["escenario"];

            escenario_ancho = cargarValidar(escenario, 2000, "ancho", "Usando ancho default de 2000");
            escenario_alto = cargarValidar(escenario, 200, "alto", "Usando ancho default de 200");
            escenario_ypiso = cargarValidar(escenario, 0, "ypiso", "Usando ypiso default de 0");


            const Json::Value personaje = root["personaje"];

            personaje_ancho = cargarValidar(personaje, 100, "ancho", "Usando ancho (personaje) default de 100");
            personaje_alto = cargarValidar(personaje, 100, "alto", "Usando alto (personaje) default de 100");
            personaje_zindex = cargarValidar(personaje, 0, "zindex", "Usando zindex (personaje) default de 0");

            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
            }

            const Json::Value capas = root["capas"];
            
            for ( int index = 0; index < capas.size(); ++index ){
                Capa *temp = new Capa(
                    capas[index].get("imagen_fondo", "default").asString(), 
                    capas[index].get("anchoLogico", 0).asFloat(),
                    capas[index].get("xlogico", 0).asFloat(), 
                    NULL,
                    NULL);

                capas_vector.push_back(temp);
            }

        } else {
            puts("Error de sytaxis en el archivo");
            cout << reader.getFormatedErrorMessages() << endl;
        }
    }
}

float Conf::cargarValidar(Json::Value objetoJson, float valorDefault, char* clave, char* mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("ventana no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asFloat();
}


void Conf::cargarDefault(){
    logger->log_error("Problema con el archivo, probablemente no existe");
    Conf::set_values("pruebas/json/test.json");
    logger->log_debug("Cargo configuración default");
}