#ifndef RED_H
#define RED_H

#include <Enrutador.h>
#include <fstream>

class Red
{
public:
    Red();
    bool RedActiva=false;
    void GuardarRed();
    void Agregar();
    void Eliminar();
    void Modificar();
    void MostrarTabla();
    void CosteEntre();
private:
    bool RedAleatiora=false;
    map<string, Enrutador> Enrutadores; // Clave: Nombre del enrutador. Valor: Lista de los enrutadores que están conectados a el
    void CargarRed();
    void ActualizarTabla();
    void GenerarRed();
    void MostrarNombres();
    void MostrarCostes();
};

#endif // RED_H
