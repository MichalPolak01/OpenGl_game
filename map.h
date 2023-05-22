/*****************************************************************//**
 * \file   map.h
 * \brief  Przechowuje deklaracjê plików nag³ówkowych i funkcji s³u¿¹cej do odczytywanie tablicy dwuwymiarowej z pliku txt.
 * 
 * \author Micha³ Polak
 * \date   May 2023
 *********************************************************************/

#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <windows.h>
#include <iostream>

using namespace std;

bool ReadMapFromFile(const string&, int, int**);

#endif // MAP_H