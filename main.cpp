

#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#include <windows.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MAX 25
#define WIDTH 960
#define HEIGHT 540

//ustawienie koloru t�a (RGBA)
GLfloat BGColor[] = { 0.420f, 0.604f, 0.922f, 1.0f };
//ustawienie pozycji �wiat�a w przestrzeni 3D
GLfloat Light_Pos[] = { 0.0f, -20.0f, 80.0f, 0.0f };
//ustawienie koloru dyfuzyjnego �wiat�a
GLfloat Light_Dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//okre�lenie koloru �wiat�a otoczenia
GLfloat Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//okre�lenie koloru dyfuzyjnego
GLfloat Diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
//okre�lenie koloru odbicia lustrzanego dla pow. obiekt�w u�ytych do wygenerowania efekt�w �wietlnych na scenie
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

int mapFlag = 0;

// angle of rotation for the camera direction
float angle = 3.0f;
// actual vector representing the camera's direction
float lx = 0.00f, lz = 1.0f;
// position of the camera
float x = -10.0f, y = 1.0f, z = 1.0f;

// Mapa
int Map3[MAX][MAX] = {
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



int Map[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
    { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
// Mapa tylko �ciany boczne
int Map0[MAX][MAX] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Tekstura


GLuint textureId;



//void loadTexture() {
//    // Wczytaj tekstur� z pliku
//    int width, height, channels;
//    unsigned char* image = stbi_load("pobrane.png", &width, &height, &channels, STBI_rgb);
//
//    // Utw�rz identyfikator tekstury
//    glGenTextures(1, &textureId);
//
//    // Ustaw bie��c� tekstur�
//    glBindTexture(GL_TEXTURE_2D, textureId);
//
//    // Przeka� dane tekstury do OpenGL
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//
//    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
//    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
//    //glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);
//    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
//    // I am cheating by using separate specular color, which requires OpenGL 1.2, but
//    // it gives nicer specular highlights on textured surfaces.
//
//
//    // Ustaw parametry tekstury
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    // Zwalnianie pami�ci zaalokowanej dla wczytanego obrazu
//    stbi_image_free(image);
//}

//void loadTexture() {
//    //unsigned int texture;
//    glGenTextures(1, &textureId);
//    glBindTexture(GL_TEXTURE_2D, textureId);
//    // set the texture wrapping/filtering options (on the currently bound texture object)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load and generate the texture
//    int width = 64, height =64, channels = 3;
//    unsigned char* data = stbi_load("bark.png", &width, &height, &channels, STBI_rgb);
//    if (data)
//    {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//        //glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);//GL_FASTEST);
//        //glGenerateMipmap(GL_TEXTURE_2D); // Generate mip mapping
//
//        //glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);
//}
//
//void loadTexture() {
//    GLuint loadBMP_custom(const char* imagepath);
//    GLuint image = loadBMP_custom("./my_texture.bmp");
//}

GLuint texture;

GLuint LoadTexture(const char* filename)
{
    GLuint texture;
    int width, height;
    unsigned char* data;

    FILE* file;
    file = fopen(filename, "rb");

    if (file == NULL) return 0;
    width = 256;
    height = 256;
    data = (unsigned char*)malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}


//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//
//    // Ustaw pozycj� i orientacj� kamery
//
//    // Prze��cz si� na tryb modelowania tekstur
//    glEnable(GL_TEXTURE_2D);
//
//    // Przygotuj do u�ycia tekstur�
//    glBindTexture(GL_TEXTURE_2D, textureId);
//
//    // Rysuj sze�cian z wybran� tekstur�
//    glutSolidCube(1.0);
//
//    // Wy��cz tryb modelowania tekstur
//    glDisable(GL_TEXTURE_2D);
//
//    glutSwapBuffers();
//}

//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Textured Cube");
//
//    // Inicjalizuj tekstur�
//    loadTexture();
//
//    // Ustawienia OpenGL
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);
//
//    // Ustaw funkcje wywo�ane
//    glutDisplayFunc(display);
//    glutMainLoop();
//
//    return 0;
//}






//inicjacja parametr�w
void Init()
{
    // KOLOR NIEBA
    glClearColor(0.710f, 0.325f, 0.494f, 1.000f);
    //w��czenie �r�d�a �wiat�a, ustawienie w�a�ciwo�ci materia�u (kolor, po�ysk, odbicie)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);

    //utworzenie listy wy�wietlania kwadratowych obiekt�w
    //glNewList(1, GL_COMPILE);
    //for (int i = 0; i < MAX; i++) {
    //    for (int j = 0; j < MAX; j++) {
    //        if (Map[i][j] == 0) {
    //            glLoadIdentity();
    //            glTranslatef(((MAX - 1) / 2) - i, j - ((MAX - 1) / 2), 0.5f);
    //            //glTranslatef(j, i, 0.5f);
    //            //glTranslatef(i - 11.5, j - 11.5, 0.5);
    //            //glutSolidCube(1.0);
    //            //glutWireCube(1);
    //            //glLoadIdentity();
    //            //glTranslatef(-11.5f + j, -11.5f + i, 1.5f);
    //            //glutSolidCube(1.0);

    //            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //            //glLoadIdentity();
    //           
    //            ////glLoadIdentity();

    //            // Ustaw pozycj� i orientacj� kamery

    //            // Prze��cz si� na tryb modelowania tekstur
    //            glEnable(GL_TEXTURE_2D);

    //            // Przygotuj do u�ycia tekstur�
    //            glBindTexture(GL_TEXTURE_2D, textureId);

    //            // Rysuj sze�cian z wybran� tekstur�
    //            glutSolidCube(1.0);

    //            // Wy��cz tryb modelowania tekstur
    //            glDisable(GL_TEXTURE_2D);

    //            //glutSwapBuffers();

    //        }
    //    }
    //}
    //glEndList();

    glNewList(1, GL_COMPILE);
    //for (int i = 0; i < MAX; i++) {
    //    for (int j = 0; j < MAX; j++) {
    //        if (Map[i][j] == 0) {
    //            glLoadIdentity();
    //            glTranslatef(((MAX - 1) / 2) - i, j - ((MAX - 1) / 2), 0.5f);

    glNormal3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (Map[i][j] == 0) {
                float x = (((MAX - 1) / 2) -i) * 1.0f;
                float y = (i - ((MAX - 1) / 2)) * 1.0f;


            glEnable(GL_TEXTURE_2D);

            // Przygotuj do u�ycia tekstur�
            glBindTexture(GL_TEXTURE_2D, texture);

            // Rysuj sze�cian z wybran� tekstur�

                glTexCoord2f(0.0f, 0.0f); // wsp�rz�dne tekstury
                glVertex3f(x - 1.0f, y - 1.0f, 1.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(x + 1.0f, y - 1.0f, 1.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(x + 1.0f, y + 1.0f, 1.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(x - 1.0f, y + 1.0f, 1.0);

                            // Wy��cz tryb modelowania tekstur
            glDisable(GL_TEXTURE_2D);
            }
        }
    }


    //            // Prze��cz si� na tryb modelowania tekstur
    //            glEnable(GL_TEXTURE_2D);

    //            // Przygotuj do u�ycia tekstur�
    //            glBindTexture(GL_TEXTURE_2D, texture);

    //            // Rysuj sze�cian z wybran� tekstur�
    //            //glutSolidCube(1.0);

    //            /// XXDDD

    //                //front
    //            glNormal3f(0.0f, 0.0f, 1.0f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinate
    //            glVertex3f(-1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(1.0f, 1.0f, 0.0f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(-1.0f, 1.0f, 0.0f);

    //            //right
    //            glNormal3f(1.0f, 0.0f, 0.0f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinates
    //            glVertex3f(1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(1.0f, -1.0f, -1.5f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(1.0f, 1.0f, -1.5f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(1.0f, 1.0f, 0.0f);

    //            //back
    //            glNormal3f(0.0f, 0.0f, -1.5f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinates
    //            glVertex3f(-1.0f, -1.0f, -1.5f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(1.0f, -1.0f, -1.5f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(1.0f, 1.0f, -1.5f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(-1.0f, 1.0f, -1.5f);

    //            //left
    //            glNormal3f(-1.0f, 0.0f, 1.0f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinates
    //            glVertex3f(-1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(-1.0f, -1.0f, -1.5f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(-1.0f, 1.0f, -1.5f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(-1.0f, 1.0f, 0.0f);

    //            //top
    //            glNormal3f(0.0f, 1.0f, 0.0f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinates
    //            glVertex3f(-1.0f, 1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(1.0f, 1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(1.0f, 1.0f, -1.5f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(-1.0f, 1.0f, -1.5f);

    //            //bottom
    //            glNormal3f(0.0f, -1.0f, 0.0f);
    //            glTexCoord2f(0.0f, 0.0f); //texture coordinates
    //            glVertex3f(-1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 0.0f);
    //            glVertex3f(1.0f, -1.0f, 0.0f);
    //            glTexCoord2f(1.0f, 1.0f);
    //            glVertex3f(1.0f, -1.0f, -1.5f);
    //            glTexCoord2f(0.0f, 1.0f);
    //            glVertex3f(-1.0f, -1.0f, -1.5f);



    //            // Wy��cz tryb modelowania tekstur
    //            glDisable(GL_TEXTURE_2D);
    //        }
    //    }
    //}
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

    //ustawienie pozycji kamery i kierunku patrzenia na scen�

    //printf("map: %d\n", Map[int(ceil(x)) + 12][int(ceil(z)) + 12]);

    //if (Map[int(ceil(x)) + 12][int(ceil(z)) + 12] == 1) {
        gluLookAt(x, y, z,
            x + lx, 1.0f, z + lz,
            0.0f, 1.0f, 0.0f);
 
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); //obr�cenie widoku o 90 stopni wok� osi X
    glMatrixMode(GL_MODELVIEW);
}

//wy�wietlanie grafiki
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie buforu
    Reload_TableView(); //ustawienie perspektywy, pozycji kamery i kierunku patrzenia
    glLoadIdentity(); // reset wszystkich przekszta�ce�
    glColor3fv(BGColor); //ustawienie pod�o�a
    glBegin(GL_QUADS); //rysowanie pod�o�a
    glVertex2f(-13.0f, -13.0f); //podanie wierzcho�k�w
    glVertex2f( 13.0f, -13.0f);
    glVertex2f( 13.0f,  13.0f);
    glVertex2f(-13.0f,  13.0f);
    glEnd();
    glEnable(GL_LIGHTING); //w��czenie o�wietlenia
    glCallList(1); //wywo�anie listy rysunkowej sk�adaj�cej si� z kostek na mapie gry
    glDisable(GL_LIGHTING); //wy��czenie o�wietlenie
    //Reload_TextView(); //ustawienie perspektywy do wy�wietlenia tekstu
    //glColor3f(1.0f, 1.0f, 1.0f); //ustawienie koloru tekstu na bia�y 
    glutSwapBuffers(); //podmienienie bufor�w
}

bool isWall(float valX, float valZ)
{
    // glTranslatef(11.5f - j, -11.5f + i, 0.5f);

    int wallX = int(-valX + 12);
    int wallZ = int(-valZ + 12);

    int wallX2 = int(-valX + 13);
    int wallZ2 = int(-valZ + 13);

    //int wallX = int(11.5 - valX);
    //int wallZ = int(-11.5 + valZ);

    if (wallX == 11) {
        wallX2 = wallX;
        //wallX += 1;
    }
    //if (wallZ > 12) {
    //    wallZ += 1;
    //}

    printf("valX: %d, valZ: %d, map: %d, valX: %d\n", wallX, wallZ, Map[wallX][wallZ], int(valX));
    //if (int(wallX) != 11) {
        if (Map[wallX][wallZ] == 0 || Map[wallX2][wallZ2] == 0)
        {
            return FALSE;
        }
    //}
}


//obs�uga specjalnych klawiszy klawiatury i zmiana po�o�enia kamery na scenie
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
            if (isWall(x + lx * 0.1, z + lz * 0.1)) {
                x += lx * 0.1f;
                z += lz * 0.1f;
                printf("key up\n");
            }
            break;
        case GLUT_KEY_DOWN:
            if (isWall(x - lx * 0.1, z - lz * 0.1)) {
                x -= lx * 0.1f;
                z -= lz * 0.1f;
                printf("key down\n");
            }
            break;
        }


    printf("x: %f, z: %f, lx: %f, lz: %f map: %d\n", x, z, lx, lz, Map[int(ceil(x))+12][int(ceil(z))+12]);
}

//obs�uga wci�ni�tych klawiszy klawiatury, wci�niecie m zmienia pozycj� kamery
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
    // Pr�ba
    printf("%d, %d , %d\n", Map[0][0], Map[4][3], Map[1][13]);

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (Map[i][j] == 0) {
                printf("0 ");
            }
            else {
                printf("  ");
            }
           
        }
        printf("\n");
    }


    // Tworzenie okna
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition((1920 - WIDTH) / 2, (1080 - HEIGHT) / 2 );
    glutCreateWindow("Labirynt");

    // Inicjalizuj tekstur�
    //loadTexture();
    texture = LoadTexture("vtr.bmp");

    // Wy�wietlanie
    glutDisplayFunc(Display);
    Init();
    glutIdleFunc(Display);
    


    // Obs�uga klawiatury
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(Keyboard);

    // inicjalizacja testu g��boko�ci OpenGL 
    glEnable(GL_DEPTH_TEST);


    // P�tla przetwarzania wydarze� GLUT
    glutMainLoop();
    
    return 1;
}