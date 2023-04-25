#include "Auxiliar.h"

bool ValidarNumero(string Numero)
{
    for (int i=0; i<int(Numero.size());i++)
    {
        if (isdigit(Numero[i])!=true)
        {
            return false;
        }
    }
    return true;
}

int ConvertirANumero(string SNumero)
{
    int Numero=0;
    int Pot=SNumero.size()-1;
    for(int i=0;i<int(SNumero.size());i++, Pot--)
    {
        Numero+=(int(SNumero[i])-48)*pow(10,Pot);
    }
    return Numero;
}

void GenerarNombre(string *Nombre)
{
    srand(time(NULL));
    char Letra=rand()%26+65;
    *Nombre=char(Letra);
}

vector<vector<string>> Dividir(string Linea, char Divisor)
{
    vector<vector<string>> Informacion{{},{}};
    int PosGeneral=0;
    int Primer=Linea.find(Divisor);
    string SegundaInformacion;
    for(int i=Primer+1; i<int(Linea.size()); i++)
    {
        SegundaInformacion.push_back(Linea[i]);
    }
    int Segundo=SegundaInformacion.find(Divisor);

    string Nombre;
    for(int i=0; i<Segundo; i++)
    {
        if(SegundaInformacion[i]!='-')
        {
            Nombre.push_back(SegundaInformacion[i]);
            if(i+1==Segundo)//Salva guarda de 1 elemento
            {
                Informacion[PosGeneral].push_back(Nombre);
            }
        }
        else
        {
            Informacion[PosGeneral].push_back(Nombre);
            Nombre.clear();
        }
    }
    PosGeneral++;
    string Coste;
    for(int i=Segundo+1; i<int(SegundaInformacion.size()); i++)
    {
        if(SegundaInformacion[i]!='-')
        {
            Coste.push_back(SegundaInformacion[i]);
            if(i+1==int(SegundaInformacion.size()))
            {
                Informacion[PosGeneral].push_back(Coste);
            }
        }
        else
        {
            Informacion[PosGeneral].push_back(Coste);
            Coste.clear();
        }
    }
    return Informacion;
}
