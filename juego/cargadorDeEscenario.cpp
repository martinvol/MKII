#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
using namespace std;

void CargarEscenarioDefault(vector<SDL_Texture*> &texturas, SDL_Renderer *ren){
    cout <<"Cargando default"<<endl;
    //HAY QUE PREGUNTAR COMO SEGUIR
}
//--------------------------------------------------------------------
void CargarEscenario(string nombre, vector <SDL_Texture*> &texturas, SDL_Renderer *ren){
    cout<<"Archivo a buscar: "+ nombre<<endl;
    //bool ArchivoExiste = CargarArchivo("configuracion.json");

    //Hardcodeo 01
    bool ArchivoExiste;
    ArchivoExiste = false;
    //fin hardcodeo 01

    if(!ArchivoExiste){
        cout<<"No existe el .JSon"<<endl;
        CargarEscenarioDefault(texturas, ren);
        return;
    }
    cout<<"Archivo encontrado"<<endl;
    //Lo de Martin.
    // FuncionMartin(nombre, texturas)
    //con el nombre, me carga los punteros a las texturas en el vector.
}

