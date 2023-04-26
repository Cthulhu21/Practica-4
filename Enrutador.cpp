#include "Enrutador.h"

Enrutador::Enrutador(string _Nombre, vector<string> Contiguos, vector<string> _CostesContiguos)
{
    NombreEnrutador=_Nombre;
    NombresContiguos=Contiguos;
    CostesContiguos=_CostesContiguos;
}

void Enrutador::AgregarEnrutador(bool Manual=false, string NombreNuevo="", string CosteNuevo="")
{
    if(Manual)
    {
        cout << "Ingrese el nombre del enrutador contiguo (0 para cancelar)" << endl;
        string Nombre;
        cin >> Nombre;
        if(Nombre!="0")
        {
            cout << "Ingrese el coste " << endl;
            string Coste;
            cin >> Coste;
            if(ValidarNumero(Coste))
            {
                NombresContiguos.push_back(Nombre);
                CostesContiguos.push_back(Coste);
            }
        }
    }
    else
    {
        NombresContiguos.push_back(NombreNuevo);
        CostesContiguos.push_back(CosteNuevo);
    }
}

void Enrutador::EliminarEnrutador(bool Manual=false, string _Nombre="")
{
    if(Manual)
    {
        cout << "Ingrese el nombre del enrutador a eliminar (0 para cancelar)" << endl;
        string Nombre;
        cin >> Nombre;
        if(Nombre!="0")
        {
            auto PosNombre= NombresContiguos.begin();
            auto PosCoste= CostesContiguos.begin();
            for(string &NombreContiguo : NombresContiguos)
            {
                if(NombreContiguo==Nombre)
                {
                    NombresContiguos.erase(PosNombre);
                    CostesContiguos.erase(PosCoste);
                    break;
                }
                PosNombre++;
                PosCoste++;
            }
        }
    }
    else
    {
        auto PosNombre= NombresContiguos.begin();
        auto PosCoste= CostesContiguos.begin();
        for(string &NombreContiguo : NombresContiguos)
        {
            if(NombreContiguo==_Nombre)
            {
                NombresContiguos.erase(PosNombre);
                CostesContiguos.erase(PosCoste);
                break;
            }
            PosNombre++;
            PosCoste++;
        }
    }
}

void Enrutador::ModificarCoste(string _Nombre, string Valor)
{
    int i=0;
    bool Dentro=false;
    for(string &Nombre : NombresContiguos)
    {
        if(Nombre==_Nombre)
        {
            CostesContiguos[i]=Valor;
            Dentro=true;
            break;
        }
        i++;
    }
    if(!Dentro)
    {
        NombresContiguos.push_back(_Nombre);
        CostesContiguos.push_back(Valor);
    }
}
