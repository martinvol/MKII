#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


#include <unordered_map>
#include "jsoncpp/json/json.h"

#include "../Escenario/Capa.hpp"
#include "../Logger/Logger.hpp"
#include "../Personaje/TomaData.hpp"

class Parser {
	private:
		float cargarValidar(Json::Value, float, char*, char*);
        bool cargarValidarBool(Json::Value, bool, char*, char*);
		Logger *logger;

    public:
		// El constructor no lo hacemos porque se supone que siempre después de
		// crearlo se llama a set_values inmediatamente, antes de llamar a
		// cualquier otra función.
		~Parser();

        void set_values (char* file_name);
        void cargarDefault();
        int ventana_anchopx, ventana_altopx;
        float ventana_ancho;
        float escenario_ancho, escenario_alto, escenario_ypiso;
        float personaje_ancho, personaje_alto, personaje_zindex;
        float personaje2_ancho, personaje2_alto, personaje2_zindex;
        float velocidad_arma;
        
        float tiempoTotal;

        float tiempo_max_boton, maximos_botones;
        int errores_maximo;

        float color_inicio, color_fin, color_offset;
        float margen;
        std::vector<Capa*> capas_vector;
        std::vector<TomaData*>* tomas;
        unordered_map <string, string> sprites_map, sprites_map2;
        bool valido = false; // Si este valor es falso el archivo se cargó mal

        string personaje1_nombre, personaje2_nombre;

        std::vector<unordered_map <string, int>*>* joysticks;
};

#endif // PARSER_H_INCLUDED
