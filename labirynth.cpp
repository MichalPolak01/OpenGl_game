/*****************************************************************//**
 * \file   labirynth.cpp
 * \brief  Przechowuje funkcje oraz zmienne u¿ywane do rysowania labiryntu.
 * 
 * \author Micha³ Polak
 * \date   April 2023
 *********************************************************************/

#include "labirynth.h"
#include "map.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/**
 * \brief Ustawienie sta³ej PI.
 */
#define PI  3.14159265359f

/**
 * \brief Ustawienie pozycji œwiat³a w przestrzeni 3D.
 */
GLfloat Light_Pos[] = { 0.0f, -20.0f, 80.0f, 0.0f };

/**
 * \brief Ustawienie koloru dyfuzyjnego œwiat³a.
 */
GLfloat Light_Dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Okreœlenie koloru œwiat³a otoczenia.
 */
GLfloat Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Okreœlenie koloru dyfuzyjnego.
 */
GLfloat Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };

/**
 * \brief Okreœlenie koloru odbicia lustrzanego dla pow. obiektów u¿ytych do wygenerowania efektów œwietlnych na scenie.
 */
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
 * \brief Identyfikatory tekstur.
 */
GLuint textureBox, textureGround, textureFinish;

/**
 * \brief Zmienna informuj¹ca czy klawisz m jest wciœniêty.
 */
int mapFlag = 0;

/**
 * \brief K¹t obrotu kamery.
 */
float angle = 3.0f;

/**
 * \brief Zmienne reprezentuj¹ce aktualny kierunek kamery.
 */
float lx = 0.00f, lz = 1.0f;

/**
 * \brief Zmienne reprezentuj¹ce pozycjê kamery.
 */
float x = -10.0f, y = 1.0f, z = 1.0f;

/**
 * \brief Zmienne pomocnicze do kamery.
 */
float tempX, tempZ, tempY, tempLx, tempLz;

/**
 * \brief Zmienne dotycz¹ce mapy.
 */
int MAX;
int **Map;

string mapName;

/**
 * \brief Uruchomienie gry.
 * 
 * \param level - Poziom w grze.
 */
void startGame(int level) {
    // Odczytanie tekstur z plików
    string fileName = "vtr.bmp";
    loadTexture(fileName, textureBox);

    string fileName2 = "grass.png";
    loadTexture(fileName2, textureGround);

    string fileName3 = "finish.png";
    loadTexture(fileName3, textureFinish);

    // Wybór mapy
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
        mapName = "Mapa 1";
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
        mapName = "Mapa 2";
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
        mapName = "Mapa 3";
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
        mapName = "Mapa 4";
        break;
    }

    // Odczytanie mapy z pliku
    ReadMapFromFile(mapFileName, MAX, Map);

    // Wyœwietlanie
    glutDisplayFunc(Display);
    Init();
    glutIdleFunc(Display);

    // Obs³uga klawiatury
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(Keyboard);

    // inicjalizacja testu g³êbokoœci OpenGL 
    glEnable(GL_DEPTH_TEST);
}

/**
 * \brief Inicjalizacja parametrów.
 * 
 */
void Init()
{
    // KOLOR NIEBA
    glClearColor(0.420f, 0.604f, 0.922f, 1.0f);
    // W³¹czenie Ÿród³a œwiat³a, ustawienie w³aœciwoœci materia³u (kolor, po³ysk, odbicie)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);

    //utworzenie listy wyœwietlania kwadratowych obiektów
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

    //ustawienie pozycji kamery i kierunku patrzenia na scenê
    gluLookAt(x, y, z,
        x + lx, 1.0f, z + lz,
        0.0f, 1.0f, 0.0f);

    //obrócenie widoku o 90 stopni wokó³ osi X
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
    glMatrixMode(GL_MODELVIEW);
}

/**
 * \brief Wyœwietlanie grafiki.
 * 
 */
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie buforu
    Reload_TableView(); //ustawienie perspektywy, pozycji kamery i kierunku patrzenia
    glLoadIdentity(); // reset wszystkich przekszta³ceñ

    glEnable(GL_LIGHTING); //w³¹czenie oœwietlenia
    glCallList(1); //wywo³anie listy rysunkowej sk³adaj¹cej siê z kostek na mapie gry
    glDisable(GL_LIGHTING); //wy³¹czenie oœwietlenie

    if (mapFlag == 1)
    {
        glColor3f(0.0f, 0.0f, 1.0f); // Ustawia kolor na niebieski
        glLoadIdentity();
        glTranslatef(tempX, -tempZ, 0.45f);

        // Rysowanie ko³a
        drawCircle(0.5f, 100);

        glFlush();
    }

    information();
    glutSwapBuffers(); //podmienienie buforów
}




/**
 * \brief Sprawdzanie czy kamera nie wchodzi w œcianê.
 * 
 * \param valX - pozycja kamery w p³aszczyŸnie x po nastêpnym kroku.
 * \param valZ - pozycja kamery w p³aszczyŸnie z po nastêpnym kroku.
 * \return - Zwraca wartoœæ FALSE, je¿eli kamera wchodzi w œcianê lub true, je¿eli nie.
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
* \brief Obs³uga specjalnych klawiszy.
*
* \param key - kod wciœniêtego klawisza specjalnego.
* \param xx  - pozycja kursora myszy w osi X.
* \param yy  - pozycja kursora myszy w osi Y.
*/
void processSpecialKeys(int key, int xx, int yy) {
    // Zmiana po³o¿enia kamery na scenie
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
 * \brief Obs³uga wciœniêtych klawiszy.
 * 
* \param key - kod wciœniêtego klawisza.
* \param xx  - pozycja kursora myszy w osi X.
* \param yy  - pozycja kursora myszy w osi Y.
 */
void Keyboard(GLubyte key, int xx, int yy)
{
    switch (key) {
    //  Wciœniecie m - rzut kamery na mapê z góry.
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
 * \brief Rysowanie szeœcianu.
 * 
 */
void drawCube() {

    // Rozpoczêcie nak³adania tekstury
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

    // Zakoñczenie nak³adania tekstury
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Rysowanie pod³o¿e
/**
 * \brief Rysowanie pod³o¿a.
 * 
 */
void drawGround(GLuint &texture)
{
    // Rozpoczêcie nak³adania tekstury
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
    
    // Zakoñczenie nak³adania tekstury
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
/**
 * \brief Rysowanie okrêgu.
 * 
 * \param radius - Promieñ okrêgu.
 * \param segments - Iloœæ segmentów, na które podzielone jest ko³o.
 */
void drawCircle(float radius, int segments)
{
    glLineWidth(3.5f); // Ustawienie gruboœci linii
    glBegin(GL_LINE_LOOP); // Rysowanie linii
    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(segments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

/**
 * \brief Wczytywanie tekstur.
 * 
 * \param fileName - nazwa pliku z tekstur¹.
 * \param texture - identyfikator tekstury.
 */
void loadTexture(const string &fileName, GLuint &texture) {
    
    // Wczytaj teksturê z pliku
    string filePath = "Textures/" + fileName;
    int width, height, channels;
    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb);

    // Utwórz identyfikator tekstury
    glGenTextures(1, &texture);

    // Ustaw bie¿¹c¹ teksturê
    glBindTexture(GL_TEXTURE_2D, texture);

    // Przeka¿ dane tekstury do OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Ustaw parametry tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Zwalnianie pamiêci zaalokowanej dla wczytanego obrazu
    stbi_image_free(image);
}

/**
 * \brief Informacje do wyœwietlenia.
 * 
 */
void information()
{
    Reload_TextView(); //ustawienie perspektywy do wyœwietlenia tekstu
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f); //ustawienie koloru tekstu na bia³y
    showNotification(-0.98f, 0.95f, "m - mapa");
    showNotification(-0.98f, 0.90f, "1 - zmiana mapy na 1");
    showNotification(-0.98f, 0.85f, "2 - zmiana mapy na 2");
    showNotification(-0.98f, 0.80f, "3 - zmiana mapy na 3");
    showNotification(-0.98f, 0.75f, "4 - zmiana mapy na 4");
    glColor3f(1.0f, 0.0f, 0.0f); //ustawienie koloru tekstu na bia³y
    showNotification(0.0f, 0.9f, mapName);
}

/**
 * \brief Poka¿ informacje.
 * 
 * \param x - Wspó³rzêdna x.
 * \param y - Wspó³rzêdna y. 
 * \param text - Tekst.
 */
void showNotification(float x, float y, string text)
{
    glRasterPos2f(x, y);
    const char* txt = text.c_str();
    while (*txt != 0)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *txt);
        txt++;
    }
}