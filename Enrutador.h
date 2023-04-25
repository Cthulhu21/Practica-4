#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include "Auxiliar.h"
#include <map>

using namespace std;

class Enrutador
{
public:
    Enrutador(string _Nombre="", vector<string> Contiguos={}, vector<string> _CostesContiguos={});
    vector<string> NombresContiguos{};
    vector<string> CostesContiguos{};
    string NombreEnrutador{};
    map<string, vector<string>> Tabla;
    void AgregarEnrutador(bool Manual, string NombreNuevo, string CosteNuevo);
    void EliminarEnrutador(bool Manual, string Nombre);
    void ModificarCoste(string Nombre, string Valor);
};

#endif // ENRUTADOR_H
