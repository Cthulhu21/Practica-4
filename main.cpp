#include "Red.h"

int main()
{
    Red RedPrincipal;
    if(RedPrincipal.RedActiva)
    {
        string Opcion;
        do
        {
            do
            {
                cout << "Ingrese la opcion que desees" << endl;
                cout << "0. Salir" << endl;
                cout << "1. Agregar enrutadores nuevos" << endl;
                cout << "2. Eliminar enrutadores" << endl;
                cout << "3. Modificar costes entre enrutadores" << endl;
                cout << "4. Mirar tabla " << endl;
                cout << "5. Revisar coste optimo entre 2 enrutadores" << endl;
                cin >> Opcion;
            }while(!ValidarNumero(Opcion));
            switch (ConvertirANumero(Opcion))
            {
            case 1:
                system("cls");
                RedPrincipal.Agregar();
                system("cls");
                break;
            case 2:
                system("cls");
                RedPrincipal.Eliminar();
                system("cls");
                break;
            case 3:
                system("cls");
                RedPrincipal.Modificar();
                system("cls");
                break;
            case 4:
                system("cls");
                RedPrincipal.MostrarTabla();
                break;
            case 5:
                system("cls");
                RedPrincipal.CosteEntre();
                break;
            default:
                break;
            }
        }while(ConvertirANumero(Opcion)!=0);
        system("cls");
        RedPrincipal.GuardarRed();
    }
    return 0;
}
