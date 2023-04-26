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
        for(auto &[Clave, Valor] : Enrutadores)
        {
            Enrutador EnrutadorActual=Valor;
            Archivo<<EnrutadorActual.NombreEnrutador<<";";
            int i=1;
            for(string &NombreContiguo : EnrutadorActual.NombresContiguos)
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
            for(string &CosteContiguo : EnrutadorActual.CostesContiguos)
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
        /*for(auto Enrutador : Enrutadores)
        {
            class Enrutador EnrutadorActual=Enrutador.second;
            Archivo<<EnrutadorActual.NombreEnrutador<<";";
            int i=1;
            for(string &NombreContiguo : EnrutadorActual.NombresContiguos)
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
            for(string &CosteContiguo : EnrutadorActual.CostesContiguos)
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
        }*/
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
        //Se eligen los nombres sin repetición del abecedario
        string Nombre=ABC[rand()%ABC.size()];
        auto Pos=ABC.begin();
        for(const string &Letra: ABC)
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
    for(auto &[Clave, Valor]: NuevaRed)
    {
        int CantidadConexiones;
        do
        {
            CantidadConexiones=rand()%CantidadElementos;
        }while(CantidadConexiones==0);
        vector<string> _Nombres=Nombres;
        vector<string>::iterator Posicion=_Nombres.begin();
        for(string &_Nombre: _Nombres)
        {
            if(_Nombre==Clave)
            {
                break;
            }
            Posicion++;
        }
        _Nombres.erase(Posicion);
        if(Valor.NombresContiguos.size()!=0)
        {
            CantidadConexiones-=Valor.CostesContiguos.size();
            if(CantidadConexiones<=0)
                break;
            int i=Valor.CostesContiguos.size();
            do
            {
                Posicion=_Nombres.begin();
                for(string& _Nombre : Valor.NombresContiguos)
                {
                    for(string &Nombre2 : _Nombres)
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
            Valor.AgregarEnrutador(false, NombreNuevaConexion, CosteNuevaConexion);

            Posicion=_Nombres.begin();
            for(string &_Nombre : _Nombres)
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
        NuevaRed[Clave]=Valor;
        vector<string> Costes=Valor.CostesContiguos;

        Posicion=Costes.begin();
        for(string &_Nombre : Valor.NombresContiguos)
        {
            bool Incluido=false;
            for(string &_Nombre2 : NuevaRed[_Nombre].NombresContiguos)
            {
                if(_Nombre2==_Nombre)
                {
                    Incluido=true;
                    break;
                }
            }
            if(!Incluido)
            {
                NuevaRed[_Nombre].AgregarEnrutador(false, Clave,*Posicion);
            }
            Posicion++;
        }
    }
    /*for(auto _Enrutador: NuevaRed)
    {
        int CantidadConexiones;
        do
        {
            CantidadConexiones=rand()%CantidadElementos;
        }while(CantidadConexiones==0);
        vector<string> _Nombres=Nombres;
        vector<string>::iterator Posicion=_Nombres.begin();
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
    }*/
    for(auto &[Clave, Valor] : NuevaRed)
    {
        vector<string> Nombres=Valor.NombresContiguos, Costes=Valor.CostesContiguos;
        vector<string> Unicos;
        vector<string> CosteUnicos;
        auto Pos=Costes.begin();
        for(string &Nombre : Nombres)
        {
            bool i=false;
            for(string &Unico : Unicos)
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
        NuevaRed[Clave].NombresContiguos=Unicos;
        NuevaRed[Clave].CostesContiguos=CosteUnicos;
    }
    /*
    for(auto &_Enrutador : NuevaRed)
    {
        vector<string> Nombres=_Enrutador.second.NombresContiguos, Costes=_Enrutador.second.CostesContiguos;
        vector<string> Unicos;
        vector<string> CosteUnicos;
        auto Pos=Costes.begin();
        for(string &Nombre : Nombres)
        {
            bool i=false;
            for(string &Unico : Unicos)
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
    */
    RedActiva=true;
    Enrutadores=NuevaRed;
    ActualizarTabla();
}

void Red::ActualizarTabla()
{
    map<string, vector<vector<string>>> Tabla;
    //Se buscan posibles enrutadores dañados
    map<string, Enrutador>::iterator Pos=Enrutadores.begin();
    for(auto &[Clave, Valor] : Enrutadores)
    {
        if(Valor.NombresContiguos.size()==0)
        {
            Enrutadores.erase(Pos++);
        }
    }
    for(auto &[Clave, Valor] : Enrutadores)
    {
        //Inicializacion de las variables para este enrutador
        string NodoPrincipal=Clave;
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
            vector<string> NodoAdyacenteActual={};
            auto PosicionCoste=CostesNodosAdyacentes.begin();
            for(string &NombreAdyacente : NodosAdyacentes)
            {
                if(NodosEtiquetados.find(NombreAdyacente)==NodosEtiquetados.end() or NodosEtiquetados.find(NombreAdyacente)==NodosEtiquetados.begin())
                {
                    vector<vector<string>> Valores=Nodos[NodoActual];
                    string CosteAcumulado=Valores[1][0];
                    CosteAcumulado=to_string(ConvertirANumero(CosteAcumulado)+ConvertirANumero(*PosicionCoste));
                    string _NodoActual=NombreAdyacente;
                    NodoAdyacenteActual.push_back(NodoActual);
                    if(Nodos.find(_NodoActual)!=Nodos.end()) //Revisar que la etiqueta que se le ponga sea la que tenga menor acumulado
                    {
                        vector<vector<string>> InformacionActual=Nodos[_NodoActual];
                        vector<string> _CosteAcumulado, _NombreProbable;
                        for(string &Caracter : InformacionActual[1])
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
            int ValorSiguiente=2147483647;
            for(auto &[Clave, Valor] : Nodos)
            {
                if(NodosEtiquetados.find(Clave)==NodosEtiquetados.end())
                {
                    vector<vector<string>> Informacion=Valor;
                    vector<string> NombresNuevos, ValoresNuevos;
                    NombresNuevos=Informacion[0];
                    ValoresNuevos=Informacion[1];
                    for(string &Valor : ValoresNuevos)
                    {
                        if(ConvertirANumero(Valor)<ValorSiguiente)
                        {
                            NodoActual=Clave;
                            ValorSiguiente=ConvertirANumero(Valor);
                        }
                    }
                }
            }
            Completo=(Nodos.size()>=Enrutadores.size())? true: false;
        }
        // Corriendo el algoritmo que organiza las rutas
        vector<string> Nombres, Rutas, CostesMinimos;
        for(auto &[Clave_temp, Valor_temp] : Nodos)
        {
            vector<vector<string>> Informacion=Valor_temp;
            string NombreDelSiguienteMapa, ValorMinimo, Ruta;
            Ruta+=Informacion[0][0];
            NombreDelSiguienteMapa=Informacion[0][0];
            ValorMinimo=Informacion[1][0];
            while(NombreDelSiguienteMapa!=Clave)
            {
                vector<vector<string>> LugarActual=Nodos[NombreDelSiguienteMapa];
                for(vector<string> &Dato: LugarActual)
                {
                    Ruta+=Dato[0];
                    NombreDelSiguienteMapa=Dato[0];
                    break;
                }
            }
            Nombres.push_back(Clave_temp);
            Rutas.push_back(Ruta);
            CostesMinimos.push_back(ValorMinimo);
        }
        Tabla[NodoPrincipal]={Nombres,Rutas,CostesMinimos};
    }
    for(auto &[Clave, Valor] : Tabla)
    {
        int i=0;
        for(string &_Enrutador: Valor[0])
        {
            vector<string> Dato={Valor[1][i], Valor[2][i++]};
            Enrutadores[Clave].Tabla[_Enrutador]=Dato;
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
                for(string &Equipo : EquiposCo)
                {
                    NuevoEnrutador.AgregarEnrutador(false, Equipo, CosteEq[i]);
                    i++;
                }
                auto Exito=Enrutadores.insert({NEP, NuevoEnrutador});//Par Mapa-Bool
                if(!Exito.second)
                {
                    cout << "El enrutador ingresado ya se encuentra en la red" << endl;
                    NEC="0";
                }
                else // Función para ingresar los datos de los enrutadores adyacentes
                {
                    i=0;
                    for(string &enrutador : EquiposCo)
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
                for(auto &[Clave, Valor]: Enrutadores) // Busqueda entre los enrutadores
                {
                    Valor.EliminarEnrutador(false, Opcion);
                    Valor.Tabla.erase(Valor.Tabla.find(Opcion));
                    Enrutadores[Clave]=Valor;
                }
                cout << "Se ha eliminado exitosamente" << endl;
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
    for(auto &[Clave, Valor] : Enrutadores)
    {
        cout << Clave << endl;
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
    for (auto &[Clave, Valor]: Enrutadores)
    {
        cout << "Los costes de enrutadores adyacentes a " << Clave << " son: " << endl;
        vector<string> Nombres=Valor.NombresContiguos;
        vector<string> Costes=Valor.CostesContiguos;
        int i=0;
        for(string &Nombre : Nombres)
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
        for(auto &[Clave, Valor]: _Tabla)
        {
            cout << Clave << " : " << Valor[0] << " con coste total " << Valor[1] << endl;;
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
