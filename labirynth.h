/*****************************************************************//**
 * \file   labirynth.h
 * \brief  Przechowuje deklaracjê plików nag³ówkowych, funkcji i sta³ych u¿ywanych do obs³ugi labiryntu.
 * 
 * \author Micha³ Polak
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
 * Zdefiniowanie wielkoœci okna.
 */
#define  WIDTH 960
#define  HEIGHT 540

void Display();
void Init();
void processSpecialKeys(int, int, int);
void Keyboard(GLubyte , int , int );


void startGame(int);
void drawCube();
void drawGround(GLuint&);
void drawCircle(float, int);
void loadTexture(const string&, GLuint&);
void showNotification(float, float, string);
void information();

#endif // !LABIRYNTH_H