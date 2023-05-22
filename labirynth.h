/*****************************************************************//**
 * \file   labirynth.h
 * \brief  Przechowuje deklaracj� plik�w nag��wkowych, funkcji i sta�ych u�ywanych do obs�ugi labiryntu.
 * 
 * \author Micha� Polak
 * \date   May 2023
 *********************************************************************/

#ifndef LABIRYNTH_H
#define LABIRYNTH_H

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#include <iostream>

using namespace std;

/**
 * Zdefiniowanie wielko�ci okna.
 */
#define  WIDTH 960
#define  HEIGHT 540

void Display();
void Init();
void processSpecialKeys(int, int, int);
void Keyboard(GLubyte , int , int );

void loadTexture(const string&, GLuint&);
void drawCube();
void drawGround(GLuint&);
void startGame(int);
void showNotification(float, float, string);

#endif // !LABIRYNTH_H