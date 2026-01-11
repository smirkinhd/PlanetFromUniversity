#include "framework.h"
#include <SDL.h>
#include <iostream>
#include <gl/GL.h>
#include <glut.h>
#include <GL/GLU.h>
#include <Glaux.h>
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib") 
#define MAX_LOADSTRING 100
// Инициализация SDL и OpenGL
SDL_Window* window = NULL;
SDL_GLContext context;

int display_w = 1280;
int display_h = 960;

GLuint texture[2];

void LoadGLTextures()
{
    // Загрузка картинки
    AUX_RGBImageRec* texture1, * texture2; //для хранения изображения в формате BMP 
    texture1 = auxDIBImageLoadA("texture.bmp");
    texture2 = auxDIBImageLoadA("texture2.bmp");
    // Создание текстуры1
    glGenTextures(2, &texture[0]); //создание двух идентификаторов текстур в памяти видеокарты идентификаторы сохраняются в массиве texture
    glBindTexture(GL_TEXTURE_2D, texture[0]); //уст. текущую текстурную единицу и связ. ее с соответ. ид. текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//параметры текстуры 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data); //загрузка ее данных в видеопамять

    //текстура2
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture2->sizeX, texture2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2->data);

    free(texture1);//освобождаем память
    free(texture2);
}


void init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display_w, display_h, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    LoadGLTextures();
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45, ((double)display_w / (double)display_h), 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0, 0.0, -5);
}

// Переменные для управления камерой
float camera_x = 0;
float camera_y = 0;
float camera_z = 0;

GLuint texture_id[2];


// Основной цикл программы
void main_loop()
{
    // Обработка событий

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
            break;
            // Обработка событий клавиатуры для управления камерой
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                camera_x += 0.01;
                break;
            case SDLK_RIGHT:
                camera_x -= 0.01;
                break;
            case SDLK_UP:
                camera_y -= 0.01;
                break;
            case SDLK_DOWN:
                camera_y += 0.01;
                break;
            case SDLK_PAGEUP:
                camera_z += 0.01;
                break;
            case SDLK_PAGEDOWN:
                camera_z -= 0.01;
                break;
            case SDLK_SPACE:
                camera_x = 0;
                camera_z = 0;
                camera_y = 0;
                glTranslatef(camera_x, camera_y, camera_z);
                break;
            }
            break;
        }
    }
    // Обновление положения камеры
    glTranslatef(camera_x, camera_y, camera_z);
    // Очистка экрана
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    GLUquadricObj* quadric_obj;
    quadric_obj = gluNewQuadric();
    gluQuadricTexture(quadric_obj, GL_TRUE);
    gluQuadricDrawStyle(quadric_obj, GLU_FILL);
    glTranslatef(2.0 * cos(SDL_GetTicks() / 1000.0), 0.0, 2.0 * sin(SDL_GetTicks() / 1000.0));
    gluSphere(quadric_obj, 1, 30, 30);
    gluDeleteQuadric(quadric_obj);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    GLUquadricObj* quadric_obj1;
    quadric_obj1 = gluNewQuadric();
    gluQuadricTexture(quadric_obj1, GL_TRUE);
    gluQuadricDrawStyle(quadric_obj1, GLU_FILL);
    glTranslatef(0.0, 2.0 * sin(SDL_GetTicks() / 1000.0), -2.0 * cos(SDL_GetTicks() / 1000.0));
    gluSphere(quadric_obj1, 0.6, 30, 30);
    gluDeleteQuadric(quadric_obj1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    SDL_GL_SwapWindow(window);
    SDL_Delay(10);
}

int main(int argc, char** argv)
{
    init();
    while (true)
    {
        main_loop();
    }
    return 0;
}