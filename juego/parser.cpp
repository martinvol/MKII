#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "Capa.h"
#include "parser.h"

// This is the JSON header
#include "jsoncpp/json/json.h"

using namespace std;

void Conf::set_values (char* file_name) {
    Json::Value root;
    Json::Reader reader;

    if (file_name == NULL){
            puts("No hay archivo");
            return;
    }
    
    std::ifstream test(file_name, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(test, root, false);

    if (parsingSuccessful){

        const Json::Value ventana = root["ventana"];
        
        ventana_anchopx = ventana.get("anchopx", 0).asFloat();
        ventana_altopx = ventana.get("altopx", 0).asFloat();
        ventana_ancho = ventana.get("ancho", 0).asFloat();
        margen = ventana.get("margen", 0).asFloat();

        const Json::Value escenario = root["escenario"];
        escenario_ancho = escenario.get("ancho", 0).asFloat();
        escenario_alto = escenario.get("alto", 0).asFloat();
        escenario_ypiso = escenario.get("ypiso", 0).asFloat();


        const Json::Value personaje = root["personaje"];
        personaje_ancho = personaje.get("ancho", 0).asFloat();
        personaje_alto = personaje.get("alto", 0).asFloat();
        personaje_zindex = personaje.get("zindex", 0).asFloat();

        const Json::Value sprites = personaje["sprites"];

        for (auto const& id : sprites.getMemberNames()) {
            //std::cout << id << std::endl;
            sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
        }

        const Json::Value capas = root["capas"];
        
        for ( int index = 0; index < capas.size(); ++index ){
/*            std::cout << capas[index].get("imagen_fondo", "default").asString();
            std::cout << "\n";
            printf("capa: %f \n", capas[index].get("ancho", 0).asFloat());
*/
            Capa *temp = new Capa(
                capas[index].get("imagen_fondo", "default").asString(), 
                capas[index].get("anchoLogico", 0).asFloat(),
                capas[index].get("xlogico", 0).asFloat(), 
                NULL);

            capas_vector.push_back(temp);
        }

   
        valido = true;
    } else {
        puts("Problema con el archivo, probablemente no existe");
    }
}


/*int main_(int argc, char* argv[]) {
    Conf conf;
    conf.set_values(argv[1]);

    printf ("conf.ventana_anchopx: %f \n", conf.ventana_anchopx);
    printf ("conf.ventana_altopx: %f \n", conf.ventana_altopx);
    printf ("conf.ventana_ancho: %f \n", conf.ventana_ancho);

    std::cout << '\n';

    printf ("conf.escenario_ancho: %f \n", conf.escenario_ancho);
    printf ("conf.escenario_alto: %f \n", conf.escenario_alto);
    printf ("conf.escenario_ypiso: %f \n", conf.escenario_ypiso);

    std::cout << '\n';

    printf ("conf.personaje_ancho: %f \n", conf.personaje_ancho);
    printf ("conf.personaje_alto: %f \n", conf.personaje_alto);
    printf ("conf.personaje_zindex: %f \n", conf.personaje_zindex);

    std::cout << '\n';

    for (std::vector<Capa>::iterator it = conf.capas_vector.begin() ; it != conf.capas_vector.end(); ++it){
        // std::cout << ' ' << conf.capas_vector.;
        std::cout << it->ubicacion << '\n';
        std::cout << it->anchoLogico << '\n';
        std::cout << it->x_logico << '\n';
    }

    for(auto it : conf.sprites_map) {
        std::cout << it.first << '\n';
        std::cout << it.second << '\n';  
    } 
    return 0;
}*/
