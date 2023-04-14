#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#include <windows.h>
#include <iostream>

#define MAX 24
#define WIDTH 960
#define HEIGHT 540

//ustawienie koloru t³a (RGBA)
GLfloat BGColor[] = { 0.420f, 0.604f, 0.922f, 1.0f };
//ustawienie pozycji œwiat³a w przestrzeni 3D
GLfloat Light_Pos[] = { 0.0f, -20.0f, 80.0f, 0.0f };
//ustawienie koloru dyfuzyjnego œwiat³a
GLfloat Light_Dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//okreœlenie koloru œwiat³a otoczenia
GLfloat Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//okreœlenie koloru dyfuzyjnego
GLfloat Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
//okreœlenie koloru odbicia lustrzanego dla pow. obiektów u¿ytych do wygenerowania efektów œwietlnych na scenie
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

int mapFlag = 0;

// angle of rotation for the camera direction
float angle = 3.0f;
// actual vector representing the camera's direction
float lx = 0.04f, lz = 1.0f;
// position of the camera
float x = -10.0f, y = 1.0f, z = 0.0f;

// Mapa
int Map[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
// Mapa tylko œciany boczne
int Map2[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

//inicjacja parametrów
void Init()
{
    glClearColor(0.710f, 0.325f, 0.494f, 1.000f);
    //w³¹czenie Ÿród³a œwiat³a, ustawienie w³aœciwoœci materia³u (kolor, po³ysk, odbicie)
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
            if (Map[i][j] == 0) {
                glLoadIdentity();
                glTranslatef(-11.5f + j, -11.5f + i, 0.5f);
                glutSolidCube(1.0);
                glLoadIdentity();
                glTranslatef(-11.5f + j, -11.5f + i, 1.5f);
                glutSolidCube(1.0);
            }
        }
    }
    glEndList();
}

//resetowanie widoku kamery i projekcji na scenie
void Reload_TextView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

//aktualizowanie widoku kamery i projekcji na scenie
void Reload_TableView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //resetowanie macierzy

    float ratio = WIDTH * 1.0f / HEIGHT;

    gluPerspective(45.0, ratio, 0.5, 100);  //ustawienie widzenia perspektywicznego

    //ustawienie pozycji kamery i kierunku patrzenia na scenê
    gluLookAt(x, y, z,
        x + lx, 1.0f, z + lz,
        0.0f, 1.0f, 0.0f);

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); //obrócenie widoku o 90 stopni wokó³ osi X
    glMatrixMode(GL_MODELVIEW);
}

//wyœwietlanie grafiki
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie buforu
    Reload_TableView(); //ustawienie perspektywy, pozycji kamery i kierunku patrzenia
    glLoadIdentity(); // reset wszystkich przekszta³ceñ
    glColor3fv(BGColor); //ustawienie pod³o¿a
    glBegin(GL_QUADS); //rysowanie pod³o¿a
    glVertex2f(-12.0f, -12.0f); //podanie wierzcho³ków
    glVertex2f(12.0f, -12.0f);
    glVertex2f(12.0f, 12.0f);
    glVertex2f(-12.0f, 12.0f);
    glEnd();
    glEnable(GL_LIGHTING); //w³¹czenie oœwietlenia
    glCallList(1); //wywo³anie listy rysunkowej sk³adaj¹cej siê z kostek na mapie gry
    glDisable(GL_LIGHTING); //wy³¹czenie oœwietlenie
    //Reload_TextView(); //ustawienie perspektywy do wyœwietlenia tekstu
    //glColor3f(1.0f, 1.0f, 1.0f); //ustawienie koloru tekstu na bia³y 
    glutSwapBuffers(); //podmienienie buforów
}

//obs³uga specjalnych klawiszy klawiatury i zmiana po³o¿enia kamery na scenie
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
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
        x += lx * 0.1f;
        z += lz * 0.1f;
        printf("key up\n");
        break;
    case GLUT_KEY_DOWN:
        x -= lx * 0.1f;
        z -= lz * 0.1f;
        printf("key down\n");
        break;
    }

    printf("x: %f, z: %f, lx: %f, lz: %f \n", x, z, lx, lz);
}

//obs³uga wciœniêtych klawiszy klawiatury, wciœnieêcie m zmienia pozycjê kamery
void Keyboard(GLubyte key, int xx, int yy)
{
    switch (key) {
    case 'm':
        if (mapFlag == 0)
        {
            y = 40.0f;
            printf("key m\n");
            mapFlag = 1;
        }
        else {
            y = 1.0f;
            printf("key m\n");
            mapFlag = 0;
        }
        break;
    }
}



int main(int argc, char** argv)
{
    // Tworzenie okna
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition((1920 - WIDTH) / 2, (1080 - HEIGHT) / 2 );
    glutCreateWindow("Labirynt");

    // Wyœwietlanie
    glutDisplayFunc(Display);
    Init();
    glutIdleFunc(Display);
    
    // Obs³uga klawiatury
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(Keyboard);

    // inicjalizacja testu g³êbokoœci OpenGL 
    glEnable(GL_DEPTH_TEST);

    // Pêtla przetwarzania wydarzeñ GLUT
    glutMainLoop();
    
    return 1;
}