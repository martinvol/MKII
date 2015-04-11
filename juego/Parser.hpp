#include <unordered_map>
#include "jsoncpp/json/json.h"

#include "Capa.hpp"
#include "Logger.hpp"

class Parser {
	private:
		float cargarValidar(Json::Value, float, char*, char*);
		Logger *logger;

    public:
        void set_values (char* file_name);
        float ventana_anchopx, ventana_altopx, ventana_ancho;
        float escenario_ancho, escenario_alto, escenario_ypiso;
        float personaje_ancho, personaje_alto, personaje_zindex;
        float margen;
        std::vector<Capa*> capas_vector;
        unordered_map <string, string> sprites_map;

        bool valido = false; // Si este valor es falso el archivo se cargó mal
};
