#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;

bool ValidarNumero(string Numero);

int ConvertirANumero(string SNumero);

void GenerarNombre(string *Nombre);

vector<vector<string>> Dividir(string Linea, char Divisor=';');

#endif // AUXILIAR_H
