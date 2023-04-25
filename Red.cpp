#include "Red.h"

Red::Red()
{
    string Eleccion;
    do
    {
        cout << "Desea crear una red? (S o N)" << endl;
        cin >> Eleccion;
    }while(Eleccion!="S" and Eleccion!="N");
    if(Eleccion=="N")
    {
        CargarRed();
    }
    else if(Eleccion=="S")
    {
        RedAleatiora=true;
        GenerarRed();
    }
    ActualizarTabla();
}

void Red::CargarRed()
{
    ifstream Archivo;
    try
    {
        string NombreRed;
        cout << "Ingrese el nombre de la red que desea abrir (0 para default)" << endl;
        cin >> NombreRed;
        if(NombreRed!="0")
        {
            NombreRed+=".txt";
        }
        else
        {
            NombreRed="Enrutadores.txt";
        }
        Archivo.open(NombreRed, ios::in);
        if(Archivo.fail())
        {
            throw 0;
        }
        while(!Archivo.eof())
        {
            string Linea;
            getline(Archivo, Linea);

            if(Linea.size()==0 and !(Archivo.eof()-1))
            {
                throw 1;
            }
            vector<vector<string>> Informacion=Dividir(Linea); //Informacion[0]=Contiguos, Informacion[1]=Costes
            string Nombre;
            vector<string> Contiguos=Informacion[0], Costes=Informacion[1];

            int Primer=Linea.find(";");
            for(int i=0; i<Primer; i++)
            {
                Nombre.push_back(Linea[i]);
            }
            Enrutadores[Nombre]=Enrutador(Nombre, Contiguos, Costes);
        }
        RedActiva=true;
    }
    catch (int Errores)
    {
        switch (Errores)
        {
        case 0:
            cout << "No se pudo abrir el archivo" << endl;
            break;
        case 1:
            cout << "El archivo se encuentra vacio" << endl;
            break;
        }
    }
    Archivo.close();
}

void Red::GuardarRed()
{
    ofstream Archivo;
    try
    {
        string NombreRed;
        cout << "Ingrese el nombre con el que desea guardar su red" << endl;
        cin >> NombreRed;
        NombreRed+=".txt";
        Archivo.open(NombreRed, ios::out);
        if(Archivo.fail())
        {
            throw 0;
        }
        auto Pos=1;
        for(auto Enrutador : Enrutadores)
        {
            class Enrutador EnrutadorActual=Enrutador.second;
            Archivo<<EnrutadorActual.NombreEnrutador<<";";
            int i=1;
            for(string NombreContiguo : EnrutadorActual.NombresContiguos)
            {
                Archivo << NombreContiguo;
                if(i==int(EnrutadorActual.NombresContiguos.size()))
                {
                    Archivo << ";";
                    break;
                }
                Archivo <<"-";
                i++;
            }
            i=1;
            for(string CosteContiguo : EnrutadorActual.CostesContiguos)
            {
                Archivo << CosteContiguo;
                if(i==int(EnrutadorActual.CostesContiguos.size()))
                {
                    if(Pos!=int(Enrutadores.size()))
                    {
                        Archivo << endl;
                    }
                    break;
                }
                Archivo <<"-";
                i++;
            }
            Pos++;
        }
    }
    catch (int Error)
    {
        switch (Error)
        {
        case 0:
            cout << "No se pudo abrir el archivo" << endl;
            break;
        }
    }

    Archivo.close();
}

void Red::GenerarRed()
{
    srand(time(NULL));
    int CantidadElementos;
    do
    {
        CantidadElementos=rand()%26+1;
    }while(CantidadElementos==1);

    map<string, Enrutador> NuevaRed;
    vector<string> Nombres;
    vector<string> ABC;
    for(int i=0; i<26; i++)
    {
        string Letra;
        char _Letra=i+65;
        Letra.push_back(_Letra);
        ABC.push_back(Letra);
    }
    for(int i=0; i<CantidadElementos; i++)
    {
        string Nombre=ABC[rand()%ABC.size()];
        auto Pos=ABC.begin();
        for(string Letra: ABC)
        {
            if(Letra==Nombre)
            {
                break;
            }
            Pos++;
        }
        ABC.erase(Pos);
        NuevaRed[Nombre]=Enrutador(Nombre);
        Nombres.push_back(Nombre);
    }
    for(auto _Enrutador : NuevaRed)
    {
        int CantidadConexiones;
        do
        {
            CantidadConexiones=rand()%CantidadElementos;
        }while(CantidadConexiones==0);
        vector<string> _Nombres=Nombres;
        auto Posicion=_Nombres.begin();
        for(string _Nombre: _Nombres)
        {
            if(_Nombre==_Enrutador.first)
            {
                break;
            }
            Posicion++;
        }
        _Nombres.erase(Posicion);
        if(_Enrutador.second.NombresContiguos.size()!=0)
        {
            CantidadConexiones-=_Enrutador.second.CostesContiguos.size();
            if(CantidadConexiones<=0)
                break;
            int i=_Enrutador.second.CostesContiguos.size();
            do
            {
                Posicion=_Nombres.begin();
                for(string _Nombre : _Enrutador.second.NombresContiguos)
                {
                    for(string Nombre2 : _Nombres)
                    {
                        if(Nombre2==_Nombre)
                        {
                            _Nombres.erase(Posicion);
                            i--;
                            break;
                        }
                        Posicion++;
                    }
                }
            }while(i!=0);
        }
        while(CantidadConexiones>0)
        {
            string NombreNuevaConexion=_Nombres[rand()%_Nombres.size()];
            string CosteNuevaConexion=to_string(rand()%26+1);
            _Enrutador.second.AgregarEnrutador(false, NombreNuevaConexion, CosteNuevaConexion);

            Posicion=_Nombres.begin();
            for(string _Nombre : _Nombres)
            {
                if(_Nombre==NombreNuevaConexion)
                {
                    break;
                }
                Posicion++;
            }
            _Nombres.erase(Posicion);
            CantidadConexiones--;
        }
        NuevaRed[_Enrutador.first]=_Enrutador.second;
        vector<string> Costes=_Enrutador.second.CostesContiguos;

        Posicion=Costes.begin();
        for(string _Nombre : _Enrutador.second.NombresContiguos)
        {
            bool Incluido=false;
            for(string _Nombre2 : NuevaRed[_Nombre].NombresContiguos)
            {
                if(_Nombre2==_Nombre)
                {
                    Incluido=true;
                    break;
                }
            }
            if(!Incluido)
            {
                NuevaRed[_Nombre].AgregarEnrutador(false, _Enrutador.first,*Posicion);
            }
            Posicion++;
        }
    }

    for(auto _Enrutador : NuevaRed)
    {
        vector<string> Nombres=_Enrutador.second.NombresContiguos, Costes=_Enrutador.second.CostesContiguos;
        vector<string> Unicos;
        vector<string> CosteUnicos;
        auto Pos=Costes.begin();
        for(string Nombre : Nombres)
        {
            bool i=false;
            for(string Unico : Unicos)
            {
                if(Unico==Nombre)
                {
                    i=true;
                }
            }
            if(!i)
            {
                Unicos.push_back(Nombre);
                CosteUnicos.push_back(*Pos);
            }
            Pos++;
        }
        NuevaRed[_Enrutador.first].NombresContiguos=Unicos;
        NuevaRed[_Enrutador.first].CostesContiguos=CosteUnicos;
    }
    RedActiva=true;
    Enrutadores=NuevaRed;
    ActualizarTabla();
}

void Red::ActualizarTabla()
{
    map<string, vector<vector<string>>> Tabla;
    //Se buscan posibles enrutadores dañados
    auto Pos=Enrutadores.begin();
    for(auto EnrutadorActual : Enrutadores)
    {
        if(EnrutadorActual.second.NombresContiguos.size()==0)
        {
            Enrutadores.erase(Pos);
        }
        Pos++;
    }
    /*for(auto EnrutadorActual :Enrutadores)
    {
        bool Terminado=false;

        string NombreNodoPrincipalActual=EnrutadorActual.first;
        map<string, pair<string, int>> Nodos;
        map<string, bool> NodosEtiquetados;

        Nodos[NombreNodoPrincipalActual]={{NombreNodoPrincipalActual},0};
        NodosEtiquetados[NombreNodoPrincipalActual]=true;

        while(!Terminado)
        {
            vector<string> NombresNodosAdyacentes=Enrutadores[NombreNodoPrincipalActual].NombresContiguos;
            vector<string> CostesNodosAdyacentes=Enrutadores[NombreNodoPrincipalActual].CostesContiguos;
            string SiguienteNodo;
            int i=0;
            for(string NombreNodo: NombresNodosAdyacentes)
            {

                if(NodosEtiquetados.find(NombreNodo)==NodosEtiquetados.end())
                {
                    int CosteAcumulado=Nodos[NombreNodo].second;
                    CosteAcumulado+=ConvertirANumero(CostesNodosAdyacentes[i]);
                    SiguienteNodo=NombreNodo;
                    if(Nodos.find(SiguienteNodo)!=Nodos.end())
                    {
                        pair<string, int> Informacion=Nodos[SiguienteNodo];
                        string Nombre=Informacion.first;
                        int Coste=Informacion.second;
                        if(Coste>CosteAcumulado)
                        {
                            Coste=CosteAcumulado;
                            Nombre=NombreNodoPrincipalActual;
                            Nodos[NombreNodo]={Nombre,Coste};
                        }
                    }
                    else
                    {
                        Nodos[NombreNodo]={NombreNodo, CosteAcumulado};
                    }
                }
                i++;
            }
            NodosEtiquetados[NombreNodoPrincipalActual]=true;
            int ValorSiguiente=INT_MAX;
            for(auto Nodo : Nodos)
            {
                string Nombre=Nodo.first;
                if(NodosEtiquetados.find(Nombre)==NodosEtiquetados.end())
                {
                    pair<string, int> Informacion=Nodo.second;
                    string NuevoNombre=Informacion.first;
                    int NuevoCoste=Informacion.second;
                    if(NuevoCoste<ValorSiguiente)
                    {
                        NombreNodoPrincipalActual=Nodo.first;
                        ValorSiguiente=NuevoCoste;
                    }
                }
            }
            Terminado=(Nodos.size()>=Enrutadores.size())? true : false;
        }
        vector<string> Nombres, Rutas, CostesMinimos;
        for(auto Nodo : Nodos)
        {
            string Ruta{};
            string NombreSiguiente=get<0>(Nodo.second);
            int ValorMinimo=get<1>(Nodo.second);
            Ruta+=NombreSiguiente;
            while(NombreSiguiente!=EnrutadorActual.first)
            {
                string Nombre=get<0>(Nodos[NombreSiguiente]);
                Ruta+=Nombre;
                NombreSiguiente=Nombre;
            }
            Nombres.push_back(Nodo.first);
            Rutas.push_back(Ruta);
            CostesMinimos.push_back(to_string(ValorMinimo));
        }
        Tabla[NombreNodoPrincipalActual]={Nombres,Rutas,CostesMinimos};
        for(auto TablaActual : Tabla)
        {
            int i=0;
            for(string _Enrutador: TablaActual.second[0])
            {
                vector<string> Dato={TablaActual.second[1][i], TablaActual.second[2][i++]};
                Enrutadores[TablaActual.first].Tabla[_Enrutador]=Dato;
            }
        }*/
    for(auto EnrutadorActual : Enrutadores)
    {
        //Inicializacion de las variables para este enrutador
        string NodoPrincipal=EnrutadorActual.first;
        //Comienzo de los nodos y la informacion que se guardará en la tabla
        map<string, vector<vector<string>>> Nodos;
        Nodos[NodoPrincipal]={{NodoPrincipal},{"0"}};
        bool Completo=false;
        string NodoActual=NodoPrincipal;
        map<string, bool> NodosEtiquetados;
        NodosEtiquetados[NodoActual]=true;
        //Corriendo el algoritmo para buscar los caminos mas cortos
        while(Completo==false)
        {
            vector<string> NodosAdyacentes=Enrutadores[NodoActual].NombresContiguos;
            vector<string> CostesNodosAdyacentes=Enrutadores[NodoActual].CostesContiguos;
            vector<string> NodoAdyacenteActual;
            auto PosicionCoste=CostesNodosAdyacentes.begin();
            string _NodoSiguiente;
            for(auto NombreAdyacente : NodosAdyacentes)
            {
                if(NodosEtiquetados.find(NombreAdyacente)==NodosEtiquetados.end() or NodosEtiquetados.find(NombreAdyacente)==NodosEtiquetados.begin())
                {
                    string CosteAcumulado;
                    auto Valores=Nodos[NodoActual];
                    for(auto Caracter : Valores[1])// Busqueda del coste acumulado
                    {
                        CosteAcumulado=Caracter;
                    }
                    CosteAcumulado=to_string(ConvertirANumero(CosteAcumulado)+ConvertirANumero(*PosicionCoste));
                    string _NodoActual=NombreAdyacente;
                    NodoAdyacenteActual.push_back(NodoActual);
                    if(Nodos.find(_NodoActual)!=Nodos.end()) //Revisar que la etiqueta que se le ponga sea la que tenga menor acumulado
                    {
                        vector<vector<string>> InformacionActual=Nodos[_NodoActual];
                        vector<string> _CosteAcumulado, _NombreProbable;
                        for(auto Caracter : InformacionActual[1])
                        {
                            if(ConvertirANumero(Caracter)>ConvertirANumero(CosteAcumulado))
                            {
                                _CosteAcumulado.push_back(CosteAcumulado);
                                _NombreProbable.push_back(NodoActual);
                                Nodos[NombreAdyacente]={_NombreProbable,_CosteAcumulado};
                            }
                        }
                    }
                    else
                    {
                        Nodos[_NodoActual]={{NodoAdyacenteActual},{CosteAcumulado}};
                    }
                }
                NodoAdyacenteActual.clear();
                PosicionCoste++;
            }
            NodosEtiquetados[NodoActual]=true;
            // Cambiar el nodo actual;
            int ValorSiguiente=INFINITY;
            for(auto Nodo : Nodos)
            {
                if(NodosEtiquetados.find(Nodo.first)==NodosEtiquetados.end())
                {
                    vector<vector<string>> Informacion=Nodo.second;
                    vector<string> NombresNuevos, ValoresNuevos;
                    NombresNuevos=Informacion[0];
                    ValoresNuevos=Informacion[1];
                    for(auto Valor : ValoresNuevos)
                    {
                        if(ConvertirANumero(Valor)<ValorSiguiente)
                        {
                            NodoActual=Nodo.first;
                            ValorSiguiente=ConvertirANumero(Valor);
                        }
                    }
                }
            }
            Completo=(Nodos.size()>=Enrutadores.size())? true: false;
        }
        // Corriendo el algoritmo que organiza las rutas
        vector<string> Nombres, Rutas, CostesMinimos;
        for(auto Mapa : Nodos)
        {
            vector<vector<string>> Informacion=Nodos[Mapa.first];
            string NombreDelSiguienteMapa, ValorMinimo, Ruta;
            for(auto Valor : Informacion[0])
            {
                Ruta+=Valor;
                NombreDelSiguienteMapa=Valor;
            }
            for(auto Valor : Informacion[1])
            {
                ValorMinimo=Valor;
            }
            while(NombreDelSiguienteMapa!=EnrutadorActual.first)
            {
                vector<vector<string>> LugarActual=Nodos[NombreDelSiguienteMapa];
                for( auto Dato: LugarActual)
                {
                    for(auto Caracter: Dato)
                    {
                        Ruta+=Caracter;
                        NombreDelSiguienteMapa=Caracter;
                    }
                    break;
                }
            }
            Nombres.push_back(Mapa.first);
            Rutas.push_back(Ruta);
            CostesMinimos.push_back(ValorMinimo);
        }
        Tabla[NodoPrincipal]={Nombres,Rutas,CostesMinimos};
    }
    for(auto TablaActual : Tabla)
    {
        int i=0;
        for(string _Enrutador: TablaActual.second[0])
        {
            vector<string> Dato={TablaActual.second[1][i], TablaActual.second[2][i++]};
            Enrutadores[TablaActual.first].Tabla[_Enrutador]=Dato;
        }
    }
}

void Red::Agregar()
{
    vector<string> EquiposCo; // Lista con los nombres de los enrutadores contiguos
    string NEP; //Nombre del enrutador principal
    string NEC; // Nombre de enrutador contiguo
    string Coste; //Coste de enrutador contiguo
    vector<string> CosteEq; //Lista con los costes para llegar a los enrutadores contiguos
    do
    {
        cout << "Nombre del enrutador para anexar (0 para salir)" << endl;
        cin >> NEP;
        if(NEP!="0")
        {
            do
            {
                cout << "Nombre del enrutador contiguo (0 para guardar los enrutadores ingresados)" << endl;
                cin >>NEC;
                if(NEC!="0")
                {
                    do
                    {
                        cout << "Coste del enrutador contiguo" << endl;
                        cin >> Coste;
                    }while(!ValidarNumero(Coste));
                    EquiposCo.push_back(NEC);
                    CosteEq.push_back(Coste);
                }
            }while(NEC!="0");
            try
            {
                if(EquiposCo.size()==0)
                    throw 0;
                Enrutador NuevoEnrutador(NEP);
                int i=0;
                for(string Equipo : EquiposCo)
                {
                    NuevoEnrutador.AgregarEnrutador(false, Equipo, CosteEq[i]);
                    i++;
                }
                auto Exito=Enrutadores.insert({NEP, NuevoEnrutador});
                if(!Exito.second)
                {
                    cout << "El enrutador ingresado ya se encuentra en la red" << endl;
                    NEC="0";
                }
                else // Función para ingresar los datos de los enrutadores adyacentes
                {
                    i=0;
                    for(auto enrutador : EquiposCo)
                    {
                        Enrutadores[enrutador].AgregarEnrutador(false, NEP, CosteEq[i]);
                        i++;
                    }
                }
                EquiposCo.clear();
                CosteEq.clear();
            }
            catch(int Error)
            {
                switch (Error)
                {
                case 0:
                    cout << "Por favor, ingrese al menos un elemento para " << NEP << endl;
                    break;
                }
            }
        }
    } while (NEP != "0"); //Agregar enrutadores contiguos
    ActualizarTabla();
}

void Red::Eliminar()
{
    string Opcion;
    bool Entrar=false;
    do
    {
        cout << "Ingrese el nombre del enrutador a eliminar (0 para salir)" << endl;
        MostrarNombres();
        cin >> Opcion;
        if(Opcion!="0")
        {
            Entrar=true;
            auto Exito=Enrutadores.erase(Opcion);
            if(!Exito)
            {
                cout << "Ingrese una de las opciones validas, o 0 para salir" << endl;
            }
            else
            {
                cout << "Se ha eliminado exitosamente" << endl;
                for(auto Enrutador: Enrutadores) // Busqueda entre los enrutadores
                {
                    Enrutador.second.EliminarEnrutador(false, Opcion);
                    Enrutador.second.Tabla.erase(Enrutador.second.Tabla.find(Opcion));
                    Enrutadores[Enrutador.first]=Enrutador.second;
                }
            }
        }
    }while(Opcion!="0");
    if(Entrar)
    {
        ActualizarTabla();
    }
}

void Red::MostrarNombres()
{
    for(auto EnrutadorActual : Enrutadores)
    {
        cout << EnrutadorActual.first << endl;
    }
}

void Red::Modificar()
{
    cout << "Aqui podra modificar el coste entre 2 enrutadores (0 para salir)" << endl;
    cout << "Si no hay un coste entre los enrutadores, entonces se agregara" << endl;
    if(Enrutadores.size()>=2)
    {
        string Primero, Segundo;
        bool salir=false;
        do
        {
            MostrarCostes();
            cout << "Ingrese el nombre del primer enrutador a modificar" << endl;
            cin >> Primero;
            (Primero=="0")? salir=true: salir=false;
            if(salir==false)
            {
                cout << "Ingrese el nombre del segundo enrutador" << endl;
                cin >> Segundo;
                (Segundo=="0" or salir==true)? salir=true: salir=false;
                if(!(Enrutadores.find(Primero)==Enrutadores.end() || Enrutadores.find(Segundo)==Enrutadores.end()))
                {
                    string NuevoValor;
                    do
                    {
                        cout << "Ingrese el nuevo valor" << endl;
                        cin >> NuevoValor;
                    }while(!ValidarNumero(NuevoValor));
                    Enrutadores[Primero].ModificarCoste(Segundo, NuevoValor);
                    Enrutadores[Segundo].ModificarCoste(Primero, NuevoValor);
                }
            }
        }while(!salir);
    }
    else
    {
        cout << "Su red tiene menos de 2 elementos, agregue otros y vuelva aqui si se equivoco en algun coste" << endl;
    }
    ActualizarTabla();
}

void Red::MostrarCostes()
{
    for (auto Enrutador: Enrutadores)
    {
        cout << "Los costes de enrutadores adyacentes a " << Enrutador.first << " son: " << endl;
        vector<string> Nombres=Enrutador.second.NombresContiguos;
        vector<string> Costes=Enrutador.second.CostesContiguos;
        int i=0;
        for(string Nombre : Nombres)
        {
            cout << Nombre << " :" << Costes[i++] << endl;
        }
    }
}

void Red::MostrarTabla()
{
    string Opcion;
    cout << "La lista de enrutadares es: " << endl;
    MostrarNombres();
    cout << "Ingrese el nombre del enrutador del cual quiere ver su tabla (Uno diferente para salir)" << endl;
    cin >> Opcion;
    if(Enrutadores.find(Opcion)!=Enrutadores.end())
    {
        cout << "Los caminos mas cortos de " << Opcion << " son:" <<endl;
        map<string, vector<string>> _Tabla=Enrutadores[Opcion].Tabla;
        for(auto Elemento: _Tabla)
        {
            cout << Elemento.first << " : " << Elemento.second[0] << " con coste total " << Elemento.second[1] << endl;;
        }
    }
}

void Red::CosteEntre()
{
    MostrarNombres();
    string Primero, Segundo;
    cout << "Ingrese el nombre del primer enrutador (0 Para salir)" << endl;
    cin >> Primero;
    if(Primero!="0")
    {
        if(Enrutadores.find(Primero)!=Enrutadores.end())
        {
            cout << "Ingrese el nombre del segundo enrutador" << endl;
            cin >> Segundo;
            if(Enrutadores.find(Segundo)!=Enrutadores.end())
            {
                map<string, vector<string>> Informacion=Enrutadores[Primero].Tabla;
                cout << "El costre entre " << Primero << " y " << Segundo << " es de: " << Informacion[Segundo][1];
                /*vector<string> Costes=Informacion[Segundo], Rutas=Informacion[Segundo][1], Nombres=Informacion[Segundo][0];
                int i=0;
                for(auto Nombre: Nombres)
                {
                    if(Nombre==Segundo)
                    {
                        cout << "El costre entre " << Primero << " y " << Segundo << " es de: " << Costes[i] << endl;
                        cout << "Y tiene por ruta: " << Rutas[i] << endl;
                        break;
                    }
                    i++;
                }*/
            }
            else
            {
                cout << "El enrutador ingresado no se encuenta registrado" << endl;
            }
        }
        else
        {
            cout << "El enrutador ingresado no se encuenta registrado" << endl;
        }
    }
}