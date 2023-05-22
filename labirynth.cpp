/*****************************************************************//**
 * \file   labirynth.cpp
 * \brief  Przechowuje funkcje oraz zmienne u�ywane do rysowania labiryntu.
 * 
 * \author Micha� Polak
 * \date   May 2023
 *********************************************************************/

#include "labirynth.h"
#include "map.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/**
 * \brief Ustawienie pozycji �wiat�a w przestrzeni 3D.
 */
GLfloat Light_Pos[] = { 0.0f, -20.0f, 80.0f, 0.0f };

/**
 * \brief Ustawienie koloru dyfuzyjnego �wiat�a.
 */
GLfloat Light_Dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Okre�lenie koloru �wiat�a otoczenia.
 */
GLfloat Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Okre�lenie koloru dyfuzyjnego.
 */
GLfloat Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };

/**
 * \brief Okre�lenie koloru odbicia lustrzanego dla pow. obiekt�w u�ytych do wygenerowania efekt�w �wietlnych na scenie.
 */
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Identyfikatory tekstur.
 */
GLuint textureBox, textureGround, textureFinish;

/**
 * \brief Zmienna informuj�ca czy klawisz m jest wci�ni�ty.
 */
int mapFlag = 0;

/**
 * \brief K�t obrotu kamery.
 */
float angle = 3.0f;

/**
 * \brief Zmienne reprezentuj�ce aktualny kierunek kamery.
 */
float lx = 0.00f, lz = 1.0f;

/**
 * \brief Zmienne reprezentuj�ce pozycj� kamery.
 */
float x = -10.0f, y = 1.0f, z = 1.0f;

/**
 * \brief Zmienne pomocnicze do kamery.
 */
float tempX, tempZ, tempY, tempLx, tempLz;

/**
 * \brief Zmienne dotycz�ce mapy.
 */
int MAX;
int **Map;

/**
 * \brief Uruchomienie gry.
 * 
 * \param level - Poziom w grze.
 */
void startGame(int level) {
    // Odczytanie tekstur z plik�w
    string fileName = "vtr.bmp";
    loadTexture(fileName, textureBox);

    string fileName2 = "grass.png";
    loadTexture(fileName2, textureGround);

    string fileName3 = "finish.png";
    loadTexture(fileName3, textureFinish);

    // Wyb�r mapy
    string mapFileName;
    switch (level)
    {    
    case 1:
        MAX = 33;
        Map = new int* [MAX];

        for (int i = 0; i < MAX; i++) {
            Map[i] = new int[MAX];
        }
        x = 10.0f, y = 1.0f, z = -14.5f;
        angle = 3.0f;
        lx = 0.00f, lz = 1.0f;

        mapFileName = "map33.txt";
        break;
    case 2:
        MAX = 33;
        Map = new int* [MAX];

        for (int i = 0; i < MAX; i++) {
            Map[i] = new int[MAX];
        }
        x = -10.0f, y = 1.0f, z = -14.5f;
        angle = 3.0f;
        lx = 0.00f, lz = 1.0f;

        mapFileName = "map33v2.txt";
        break;
    case 3:
        MAX = 43;
        Map = new int* [MAX];

        for (int i = 0; i < MAX; i++) {
            Map[i] = new int[MAX];
        }
        x = 15.0f, y = 1.0f, z = -19.5f;
        angle = 3.0f;
        lx = 0.00f, lz = 1.0f;

        mapFileName = "map43.txt";
        break;
    case 4:
        MAX = 63;
        Map = new int* [MAX];

        for (int i = 0; i < MAX; i++) {
            Map[i] = new int[MAX];
        }
        x = 23.0f, y = 1.0f, z = -29.5f;
        angle = 3.0f;
        lx = 0.00f, lz = 1.0f;

        mapFileName = "map63.txt";
        break;
    }

    // Odczytanie mapy z pliku
    ReadMapFromFile(mapFileName, MAX, Map);

    // Wy�wietlanie
    glutDisplayFunc(Display);
    Init();
    glutIdleFunc(Display);

    // Obs�uga klawiatury
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(Keyboard);

    // inicjalizacja testu g��boko�ci OpenGL 
    glEnable(GL_DEPTH_TEST);
}

/**
 * \brief Inicjalizacja parametr�w.
 * 
 */
void Init()
{
    // KOLOR NIEBA
    glClearColor(0.420f, 0.604f, 0.922f, 1.0f);
    // W��czenie �r�d�a �wiat�a, ustawienie w�a�ciwo�ci materia�u (kolor, po�ysk, odbicie)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);

    //utworzenie listy wy�wietlania kwadratowych obiekt�w
    glNewList(1, GL_COMPILE);
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (Map[i][j] == 0)
            {
                glLoadIdentity();
                glTranslatef(((MAX - 1.0f) / 2.0f) - i, j - ((MAX - 1.0f) / 2.0f), 0.5f);
                drawCube();

                glLoadIdentity();
                glTranslatef(((MAX - 1.0f) / 2.0f) - i, j - ((MAX - 1.0f) / 2.0f), 1.5f);
                drawCube();
            }
            else if (Map[i][j] == 1)
            {
                glLoadIdentity();
                glTranslatef(((MAX - 1.0f) / 2.0f) - i, j - ((MAX - 1.0f) / 2.0f), -0.5f);

                drawGround(textureGround);
            }
            else if (Map[i][j] == 4)
            {
                glLoadIdentity();
                glTranslatef(((MAX - 1.0f) / 2.0f) - i, j - ((MAX - 1.0f) / 2.0f), -0.5f);

                drawGround(textureFinish);
            }
        }
    }
    glEndList();
}

/**
 * \brief Resetowanie widoku kamery i projekcji na scenie.
 * 
 */
void Reload_TextView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

/**
 * \brief Aktualizowanie widoku kamery i projekcji na scenie.
 * 
 */
void Reload_TableView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //resetowanie macierzy

    float ratio = WIDTH * 1.0f / HEIGHT;

    //ustawienie widzenia perspektywicznego
    gluPerspective(45.0, ratio, 0.5, MAX * 4);  

    //ustawienie pozycji kamery i kierunku patrzenia na scen�
    gluLookAt(x, y, z,
        x + lx, 1.0f, z + lz,
        0.0f, 1.0f, 0.0f);

    //obr�cenie widoku o 90 stopni wok� osi X
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
    glMatrixMode(GL_MODELVIEW);
}

/**
 * \brief Wy�wietlanie grafiki.
 * 
 */
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie buforu
    Reload_TableView(); //ustawienie perspektywy, pozycji kamery i kierunku patrzenia
    glLoadIdentity(); // reset wszystkich przekszta�ce�

    glEnable(GL_LIGHTING); //w��czenie o�wietlenia
    glCallList(1); //wywo�anie listy rysunkowej sk�adaj�cej si� z kostek na mapie gry
    glDisable(GL_LIGHTING); //wy��czenie o�wietlenie

    Reload_TextView(); //ustawienie perspektywy do wy�wietlenia tekstu
    glColor3f(1.0f, 1.0f, 1.0f); //ustawienie koloru tekstu na bia�y
    showNotification(10.0f, 10.0f, "m - mapa");
    glutSwapBuffers(); //podmienienie bufor�w
}

/**
 * \brief Sprawdzanie czy kamera nie wchodzi w �cian�.
 * 
 * \param valX - pozycja kamery w p�aszczy�nie x po nast�pnym kroku.
 * \param valZ - pozycja kamery w p�aszczy�nie z po nast�pnym kroku.
 * \return - Zwraca warto�� FALSE, je�eli kamera wchodzi w �cian� lub true, je�eli nie.
 */
bool isWall(float valX, float valZ)
{
    int wallX0 = static_cast<int>(-valX + ((MAX - 1) / 2));
    int wallZ0 = static_cast<int>(-valZ + ((MAX - 1) / 2));

    int wallX = static_cast<int>(-valX + ((MAX - 1) / 2) - 0.15);
    int wallZ = static_cast<int>(-valZ + ((MAX - 1) / 2) - 0.15);

    int wallX2 = static_cast<int>(-valX + ((MAX - 1) / 2) + 1.15);
    int wallZ2 = static_cast<int>(-valZ + ((MAX - 1) / 2) + 1.15);

    printf("valX: %d, valZ: %d, map: %d, valX: %d\n", wallX, wallZ, Map[wallX][wallZ], int(valX));

    if (Map[wallX][wallZ] == 0 || Map[wallX2][wallZ] == 0 || Map[wallX][wallZ2] == 0 || Map[wallX2][wallZ2] == 0 ||
        Map[wallX0][wallZ0] == 0 || Map[wallX0][wallZ0 + 1] == 0 || Map[wallX0 + 1][wallZ0] == 0 || Map[wallX0 + 1][wallZ0 + 1] == 0)
    {
        return FALSE;
    }
    return TRUE;
}

/**
* \brief Obs�uga specjalnych klawiszy.
*
* \param key - kod wci�ni�tego klawisza specjalnego.
* \param xx  - pozycja kursora myszy w osi X.
* \param yy  - pozycja kursora myszy w osi Y.
*/
void processSpecialKeys(int key, int xx, int yy) {
    // Zmiana po�o�enia kamery na scenie
    if (mapFlag != 1)
    {
        switch (key)
        {
            case GLUT_KEY_LEFT:
                angle -= 0.1f;
                lx = sin(angle);
                lz = -cos(angle);
                printf("key left\n");
                break;
            case GLUT_KEY_RIGHT:
                angle += 0.1f;
                lx = sin(angle);
                lz = -cos(angle);
                printf("key right\n");
                break;
            case GLUT_KEY_UP:
                if (isWall(x + lx * 0.1f, z + lz * 0.1f)) {
                    x += lx * 0.1f;
                    z += lz * 0.1f;
                    printf("key up\n");
                }
                break;
            case GLUT_KEY_DOWN:
                if (isWall(x - lx * 0.1f, z - lz * 0.1f)) {
                    x -= lx * 0.1f;
                    z -= lz * 0.1f;
                    printf("key down\n");
                }
                break;
        }
    }

    printf("x: %f, z: %f, lx: %f, lz: %f map: %d\n", x, z, lx, lz, Map[int(ceil(x)) + 12][int(ceil(z)) + 12]);
}



/**
 * \brief Obs�uga wci�ni�tych klawiszy.
 * 
* \param key - kod wci�ni�tego klawisza.
* \param xx  - pozycja kursora myszy w osi X.
* \param yy  - pozycja kursora myszy w osi Y.
 */
void Keyboard(GLubyte key, int xx, int yy)
{
    switch (key) {
    //  Wci�niecie m - rzut kamery na map� z g�ry.
    case 'm':
        if (mapFlag == 0)
        {
            tempX = x;
            tempY = y;
            tempZ = z;
            tempLx = lx;
            tempLz = lz;

            lx = 0.00f;
            lz = 1.0f;

            y = MAX * 1.4f;
            x = 0.0f;
            z = 0.0f;

            printf("key m\n");
            mapFlag = 1;
        }
        else {
            x = tempX;
            y = tempY;
            z = tempZ;
            lx = tempLx;
            lz = tempLz;

            printf("key m\n");
            mapFlag = 0;
        }
        break;
    case '1':
        if (mapFlag != 1)
        {
            for (int i = 0; i < MAX; i++)
                delete[] Map[i];

            delete[] Map;
            startGame(1);
        }
        
        break;
    case '2':
        if (mapFlag != 1)
        {
            for (int i = 0; i < MAX; i++)
                delete[] Map[i];

            delete[] Map;
            startGame(2);
        }
        break;
    case '3':
        if (mapFlag != 1)
        {
            for (int i = 0; i < MAX; i++)
                delete[] Map[i];

            delete[] Map;
            startGame(3);
        }
        break;
    case '4':
        if (mapFlag != 1)
        {
            for (int i = 0; i < MAX; i++)
                delete[] Map[i];

            delete[] Map;
            startGame(4);
        }
        break;
    }
}

/**
 * \brief Rysowanie sze�cianu.
 * 
 */
void drawCube() {

    // Rozpocz�cie nak�adania tekstury
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBox);
    glBegin(GL_QUADS);

    //front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    //right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    //back
    glNormal3f(0.0f, 0.0f, -1.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    //left
    glNormal3f(-1.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    //top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    //bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Zako�czenie nak�adania tekstury
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Funkcja rysuj�ca pod�o�e
/**
 * \brief Rysowanie pod�o�a.
 * 
 */
void drawGround(GLuint &texture)
{
    // Rozpocz�cie nak�adania tekstury
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);    
    
    // Zako�czenie nak�adania tekstury
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/**
 * \brief Wczytywanie tekstur.
 * 
 * \param fileName - nazwa pliku z tekstur�.
 * \param texture - identyfikator tekstury.
 */
void loadTexture(const string &fileName, GLuint &texture) {
    
    // Wczytaj tekstur� z pliku
    string filePath = "Textures/" + fileName;
    int width, height, channels;
    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb);

    // Utw�rz identyfikator tekstury
    glGenTextures(1, &texture);

    // Ustaw bie��c� tekstur�
    glBindTexture(GL_TEXTURE_2D, texture);

    // Przeka� dane tekstury do OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Ustaw parametry tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Zwalnianie pami�ci zaalokowanej dla wczytanego obrazu
    stbi_image_free(image);
}

void showNotification(float x, float y, string text)
{
    glRasterPos2f(x, y);
    const char* txt = text.c_str();
    while (*txt != 0)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *txt);
        txt++;
    }
}